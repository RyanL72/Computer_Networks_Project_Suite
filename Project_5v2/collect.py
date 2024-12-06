import subprocess       # To execute shell commands
import csv              # For logging data into CSV files
import time             # To schedule measurements
from datetime import datetime   # To timestamp measurements
import re               # Regular expressions for parsing outputs
from concurrent.futures import ThreadPoolExecutor, as_completed
import threading        # for thread synchronization

# Define the list of servers for the top 5 universities in each region
servers = {
    'Northeast': [
        'www.nyu.edu',
        'www.harvard.edu',
        'www.mit.edu',
        'www.columbia.edu',
        'www.yale.edu'
    ],
    'Midwest': [
        'www.uchicago.edu',
        'www.umich.edu',
        'www.northwestern.edu',
        'www.uiuc.edu',
        'www.wustl.edu'
    ],
    'South': [
        'www.utexas.edu',
        'www.duke.edu',
        'www.vanderbilt.edu',
        'www.rice.edu',
        'www.unc.edu'
    ],
    'West': [
        'www.stanford.edu',
        'www.berkeley.edu',
        'www.ucla.edu',
        'www.caltech.edu',
        'www.usc.edu'
    ],
    'Mountain': [
        'www.unco.edu',
        'www.unm.edu',
        'www.unlv.edu',
        'www.colo.edu',
        'www.utah.edu'
    ]
}

# Initialize a lock for thread-safe file writing
log_lock = threading.Lock()

# Function to perform ping and parse the output
def perform_ping(server):
    try:
        # Execute the ping command with 10 packets
        ping_output = subprocess.check_output(['ping', '-c', '10', server], universal_newlines=True)
        
        # Use regular expressions to extract packet loss
        packet_loss_match = re.search(r'(\d+)% packet loss', ping_output)
        packet_loss = float(packet_loss_match.group(1)) if packet_loss_match else None
        
        # Use regular expressions to extract RTTs
        rtt_match = re.search(r'rtt min/avg/max/mdev = ([\d\.]+)/([\d\.]+)/([\d\.]+)/', ping_output)
        if rtt_match:
            rtt_min = float(rtt_match.group(1))
            rtt_avg = float(rtt_match.group(2))
            rtt_max = float(rtt_match.group(3))
        else:
            rtt_min = rtt_avg = rtt_max = None
        
        return packet_loss, rtt_min, rtt_avg, rtt_max
    except subprocess.CalledProcessError as e:
        # Handle the case where ping fails
        print(f"Ping failed for {server}: {e}")
        return None, None, None, None

# Function to perform traceroute and parse the output
def perform_traceroute(server):
    try:
        # Execute the traceroute command
        traceroute_output = subprocess.check_output(['traceroute', '-n', server], universal_newlines=True)
        
        # Split the output into lines
        lines = traceroute_output.strip().split('\n')
        
        # The number of hops is the number of lines minus 1 (excluding the header)
        hop_count = len(lines) - 1
        
        # Extract the path (list of IP addresses)
        path = []
        for line in lines[1:]:
            parts = line.split()
            if len(parts) >= 2:
                ip = parts[1]
                path.append(ip)
        
        return hop_count, path
    except subprocess.CalledProcessError as e:
        # Handle the case where traceroute fails
        print(f"Traceroute failed for {server}: {e}")
        return None, []

# Function to log data into a CSV file
def log_data(data):
    # Define the CSV file name
    filename = 'network_latency_data.csv'
    
    # Define the header
    header = ['Timestamp', 'Region', 'Server', 'IP Address', 'Packet Loss (%)',
              'RTT Min (ms)', 'RTT Avg (ms)', 'RTT Max (ms)', 'Hop Count', 'Path']
    
    with log_lock:
        # Check if the file exists to write the header
        try:
            with open(filename, 'x', newline='') as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow(header)
        except FileExistsError:
            pass  # File already exists
        
        # Write the data
        with open(filename, 'a', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(data)

# Function to perform all measurements for a server
def measure_server(timestamp, region, server):
    print(f"Measuring {server} in {region} at {timestamp}")
    
    # Perform ping
    packet_loss, rtt_min, rtt_avg, rtt_max = perform_ping(server)
    
    # Perform traceroute
    hop_count, path = perform_traceroute(server)
    
    # Resolve server IP address
    try:
        ip_address = subprocess.check_output(['dig', '+short', server], universal_newlines=True).strip()
    except subprocess.CalledProcessError:
        ip_address = 'N/A'
    
    # Prepare data for logging
    data = [
        timestamp,
        region,
        server,
        ip_address,
        packet_loss,
        rtt_min,
        rtt_avg,
        rtt_max,
        hop_count,
        ' -> '.join(path)
    ]
    
    # Log the data
    log_data(data)


def main():
    # Run the measurements continuously every 5 minutes
    while True:
        timestamp = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S')
        
        # Create a ThreadPoolExecutor
        with ThreadPoolExecutor(max_workers=25) as executor:
            futures = []
            for region, server_list in servers.items():
                for server in server_list:
                    # Submit the measure_server function to the executor
                    future = executor.submit(measure_server, timestamp, region, server)
                    futures.append(future)
            
            # Wait for all futures to complete (optional)
            for future in as_completed(futures):
                try:
                    future.result()
                except Exception as exc:
                    print(f"Generated an exception: {exc}")
        
        # Wait for 5 minutes before the next measurement
        print("Waiting for 5 minutes before the next measurement...\n")
        time.sleep(300)  # 300 seconds = 5 minutes

if __name__ == '__main__':
    main()
