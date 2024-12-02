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

# Output directory
output_dir="traceroute_results"
mkdir -p "$output_dir"

# Loop through each server
for server in "${!servers[@]}"
do
    region=${servers[$server]}
    timestamp=$(date -u +"%Y-%m-%dT%H:%M:%SZ")

    # Perform traceroute
    traceroute_output=$(traceroute "$server")

    # Save the traceroute output to a file
    output_file="$output_dir/traceroute_${server//./_}_$(date -u +"%Y%m%dT%H%M%SZ").txt"
    {
        echo "Timestamp: $timestamp"
        echo "Server: $server"
        echo "Region: $region"
        echo ""
        echo "$traceroute_output"
    } > "$output_file"
done
