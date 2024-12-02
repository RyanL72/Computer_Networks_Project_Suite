#!/bin/bash

# List of servers with their domain names and regions
declare -A servers
servers=(
    ["www.nyu.edu"]="Northeast"
    ["www.uchicago.edu"]="Midwest"
    ["www.utexas.edu"]="South"
    ["www.stanford.edu"]="West"
    ["www.unco.edu"]="Mountain"
)

# Output file
output_file="ping_results.csv"

# Write header to the output file if it doesn't exist
if [ ! -f "$output_file" ]; then
    echo "Timestamp,Server,Region,IP Address,Min RTT,Avg RTT,Max RTT,Packet Loss" > "$output_file"
fi

# Loop through each server
for server in "${!servers[@]}"
do
    region=${servers[$server]}
    timestamp=$(date -u +"%Y-%m-%dT%H:%M:%SZ")

    # Perform a ping test with 5 packets
    ping_output=$(ping -c 5 -q "$server")

    # Extract IP address
    ip_address=$(echo "$ping_output" | head -1 | grep -oP '\(\K[^\)]+')

    # Extract RTT stats (min, avg, max)
    rtt_stats=$(echo "$ping_output" | tail -1 | awk -F '/' '{print $4","$5","$6}')

    # Extract packet loss percentage
    packet_loss=$(echo "$ping_output" | grep -oP '\d+(?=% packet loss)')

    # Write data to the output file
    echo "$timestamp,$server,$region,$ip_address,$rtt_stats,$packet_loss" >> "$output_file"
done
