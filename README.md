# Computer Networks Project Suite

Welcome to the **Computer Networks Project Suite**—a collection of projects designed to reinforce computer networking concepts through hands-on implementation ( CSDS_325 Computer Networks Fall 2024 ). Each project focuses on a specific topic and builds practical skills in networking, programming, and system design.

## Overview

This repository includes four projects:

1. **IPv4 Format Checker**  
2. **Web Client**  
3. **Webserver**  
4. **Trace File Analyzer**

Each project is implemented in C++ and demonstrates key networking concepts with real-world applications. Whether you're validating IP addresses, building HTTP clients/servers, or analyzing network trace files, you'll find a wealth of learning opportunities in these projects.

---

## Projects

### 1. IPv4 Format Checker
- **Description**:  
  A command-line utility that checks whether each input string is a valid IPv4 address. It validates the address format, ensuring four segments separated by dots, each within the range 0–255, and rejects segments with leading zeroes.
  
- **Features**:
  - **List Mode**: Processes an input file line-by-line and prints each IP with a validity flag (`+` for valid, `-` for invalid).
  - **Summary Mode**: Reads an input file and outputs a summary of total lines, valid, and invalid IPs.
  
- **Usage**:
  - **Compile**:  
    ```bash
    g++ proj1.cpp -o ipv4check
    ```
  - **Run in List Mode**:  
    ```bash
    ./ipv4check -l -f input.txt
    ```
  - **Run in Summary Mode**:  
    ```bash
    ./ipv4check -s -f input.txt
    ```
  
- **Metadata**:
  - **Author**: Ryan Lin (CaseID: rhl72)
  - **Date**: September 17, 2024

---

### 2. Web Client
- **Description**:  
  A simple HTTP client that fetches a web resource and saves the response to a file. It demonstrates URL parsing, socket communication, and basic HTTP protocol handling.

- **Features**:
  - Parses a URL to extract the hostname and path.
  - Sends an HTTP GET request to the server.
  - Provides optional debug output: displaying request details, response headers, or additional info.
  
- **Usage**:
  - **Compile**:  
    ```bash
    g++ proj2.cpp -o webclient
    ```
  - **Run**:  
    ```bash
    ./webclient -u http://example.com/path -w output.txt
    ```
  - **Optional Flags**:
    - `-i`: Display connection and debug info.
    - `-q`: Print the HTTP request.
    - `-a`: Display response headers.
  
- **Metadata**:
  - **Author**: Ryan Lin (CaseID: rhl72)
  - **Date**: October 16, 2024

---

### 3. Webserver
- **Description**:  
  A lightweight webserver capable of handling HTTP GET requests to serve static files from a specified root directory. It also supports a custom `SHUTDOWN` method that, with a correct termination token, gracefully stops the server.

- **Features**:
  - Serves files based on the HTTP GET method.
  - Validates requests and returns appropriate HTTP error codes (e.g., 400, 404, 405, etc.).
  - Supports server shutdown via a termination token.
  
- **Usage**:
  - **Compile**:  
    ```bash
    g++ proj3.cpp -o webserver
    ```
  - **Run**:  
    ```bash
    ./webserver -p <Port Number> -r <Root Directory> -t <Terminate Token>
    ```
  
- **Metadata**:
  - **Author**: Ryan Lin (CaseID: rhl72)
  - **Date**: October 25, 2024

---

### 4. Trace File Analyzer
- **Description**:  
  A tool for analyzing network trace files. It offers multiple modes to extract and present packet data, including summary statistics, detailed size analysis, TCP packet field printing, and a traffic matrix to show communication patterns.

- **Features**:
  - **Information Mode**: Outputs basic stats like first/last packet times, total packet count, and the count of IP packets.
  - **Size Analysis Mode**: Displays per-packet information such as capture length, IP header size, and payload.
  - **TCP Packet Printing Mode**: Extracts and prints key fields from TCP packets.
  - **Traffic Matrix Mode**: Generates a matrix of communication pairs with counts and total payload sizes.
  
- **Usage**:
  - **Compile**:  
    ```bash
    g++ proj4.cpp -o trace_analyzer
    ```
  - **Run** (choose one mode):
    - Information Mode:  
      ```bash
      ./trace_analyzer -r <TraceFile> -i
      ```
    - Size Analysis Mode:  
      ```bash
      ./trace_analyzer -r <TraceFile> -s
      ```
    - TCP Packet Printing Mode:  
      ```bash
      ./trace_analyzer -r <TraceFile> -t
      ```
    - Traffic Matrix Mode:  
      ```bash
      ./trace_analyzer -r <TraceFile> -m
      ```
  
- **Metadata**:
  - **Author**: Ryan Lin (CaseID: rhl72)
  - **Date**: November 15, 2024

---



