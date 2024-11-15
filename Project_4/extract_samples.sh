#!/bin/bash

# Change directory to the folder where the tar.gz file is located
cd Proj4Samples || exit

# Extract the contents of the tar.gz file
tar -xzf p4-all-samples-v7.tar.gz

echo "Extraction completed."
