/*
Author: Ryan Lin
CaseID: rhl72
File name: proj2.cpp
Date: 10-16-2024
Description: Web Client
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <iostream>
#include <cstring>
#include <cstdio>

#define ERROR 1
#define REQUIRED_ARGC 3
#define HOST_POS 1
#define PORT_POS 2
#define PROTOCOL "tcp"
#define BUFLEN 4096
#define PORT 80
#define URL_PREFIX "http://"

// Function to print usage information with an optional error message
void usage(const char *progname, const char *error_message) {
    if (error_message) {
        fprintf(stderr, "Error: %s\n", error_message);
    }
    fprintf(stderr, "Usage: %s [-i] [-q] [-a] -u URL -w filename\n", progname);
    exit(ERROR);
}

int errexit (const char *format, const char *arg)
{
    fprintf (stderr,format,arg);
    fprintf (stderr,"\n");
    exit (ERROR);
}

// Function to parse the URL and extract the hostname and path
void parse_url(const char *url, char *hostname, char *path) {
    size_t prefix_length = strlen(URL_PREFIX);

    // Check if the URL starts with "http://", case-insensitively
    if (strncasecmp(url, URL_PREFIX, prefix_length) != 0) {
        fprintf(stderr, "Error: URL must start with '%s'\n", URL_PREFIX);
        exit(EXIT_FAILURE);
    }

    // Move the pointer past the "http://" prefix
    url += prefix_length;

    // Find the first '/' in the remaining URL to separate the hostname and path
    const char *slash_position = strchr(url, '/');
    if (slash_position) {
        // Copy the hostname part
        strncpy(hostname, url, slash_position - url);
        hostname[slash_position - url] = '\0'; // Null-terminate the hostname string

        // Copy the path part
        strcpy(path, slash_position);
    } else {
        // If no path is specified, set the path to "/"
        strcpy(hostname, url);
        strcpy(path, "/");
    }
}

int main (int argc, char *argv [])
{   
    // Structure used to specify the address for this socket
    struct sockaddr_in sin;
    struct hostent *hinfo;
    struct protoent *protoinfo;
    char buffer[BUFLEN];
    int sd, ret;

    // Variables to store command-line options
    char *url = NULL, *filename = NULL;
    int show_info = 0, show_request = 0, show_response = 0;

    char hostname[BUFLEN];
    char path[BUFLEN];

    // Command-line argument parsing
    int opt;
    while((opt = getopt(argc, argv, "iu:qaw:")) != -1){
        switch(opt){
            case 'u':
                url = optarg;
                break;
            case 'w':
                filename = optarg;
                break;
            case 'i':
                show_info = 1;
                break;
            case 'q':
                show_request = 1;
                break;
            case 'a':
                show_response = 1;
                break;
            default:
                usage(argv[0], "Incorrect format use: %s [-i] [-q] [-a] -u URL -w filename\n");
        }
    }

    // Check if at least one of the mandatory options is missing
    if (!url || !filename) {
        usage(argv[0], "Both the '-u' URL and '-w' filename options are required.");
    }

    // Check if more than one of the -i, -q, or -a options is set
    if (show_info + show_request + show_response != 1) {
        usage(argv[0], "One of the options: '-i', '-q', or '-a' must be specified.");
    }

    // Parse URL
    parse_url(url, hostname, path);

    // If the -i option is active, display debug information
    if (show_info) {
        printf("INFO: host: %s\nINFO: web_file: %s\nINFO: output_file: %s\n", hostname, path, filename);

        // Write the debug information to the specified file
        FILE *file = fopen(filename, "w");
        if (file) {
            fprintf(file, "INFO: host: %s\nINFO: web_file: %s\nINFO: output_file: %s\n", hostname, path, filename);
            fclose(file);
        } else {
            std::cerr << "Error: Unable to write debug information to file: " << filename << std::endl;
        }
        exit(0);
    }

    /* lookup the hostname using the parsed hostname */
    hinfo = gethostbyname(hostname);
    if (hinfo == NULL){
        errexit("cannot find name: %s", hostname);
    }
    
    /* set endpoint information */
    memset ((char *)&sin, 0x0, sizeof (sin));
    sin.sin_family = AF_INET;
    //host to netoworks hton
    sin.sin_port = htons(PORT);  

    memcpy ((char *)&sin.sin_addr,hinfo->h_addr,hinfo->h_length);

    if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
        errexit ("cannot find protocol information for %s", PROTOCOL);

    /* allocate a socket */
    /*   would be SOCK_DGRAM for UDP */
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
        errexit("cannot create socket",NULL);

    /* connect the socket */
    if (connect (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit ("cannot connect", NULL);

    // Construct the HTTP request
    int request_length = snprintf(buffer, BUFLEN, "GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Case CSDS 325/425 WebClient 0.1\r\n\r\n", path, hostname);
    if (request_length >= BUFLEN) {
        fprintf(stderr, "Error: HTTP request was truncated. Buffer size is too small.\n");
        exit(EXIT_FAILURE);
    }

    // If the -q option is active, display the HTTP request details
    if (show_request) {
        printf("REQ: GET %s HTTP/1.0\nREQ: Host: %s\nREQ: User-Agent: Case CSDS 325/425 WebClient 0.1\n", path, hostname);
    }

    // Send the HTTP request to the server
    write(sd, buffer, strlen(buffer));

    // Read the server's response into the buffer
    memset(buffer, 0x0, BUFLEN);
    ret = read(sd, buffer, BUFLEN - 1);
    if (ret < 0)
        errexit("reading error", NULL);

    // If the -a option is active, display only the response headers
    if (show_response) {
        char *header_end = strstr(buffer, "\r\n\r\n");  // Find the end of the headers
        if (header_end) {
            *header_end = '\0';  // Null-terminate the header section

            // Print each line with "RSP: " prefix
            char *line = strtok(buffer, "\r\n");
            while (line != nullptr) {
                std::cout << "RSP: " << line << std::endl;
                line = strtok(nullptr, "\r\n");
            }
        }
    } else if (strstr(buffer, "200 OK")) {  // Check if the response contains a "200 OK" status code
        char *header_end = strstr(buffer, "\r\n\r\n");  // Find the end of the headers
        if (header_end) {
            // Write the body content to the file specified by the -w option
            FILE *file = fopen(filename, "w");
            if (file) {
                // Add "RSP: " prefix to each line in the body content before writing to the file
                char *body_line = strtok(header_end + 4, "\r\n");
                while (body_line != nullptr) {
                    fprintf(file, "RSP: %s\n", body_line);
                    body_line = strtok(nullptr, "\r\n");
                }
                fclose(file);
            } else {
                std::cerr << "Error: Unable to write content to file: " << filename << std::endl;
            }
        }
    } else {
        std::cerr << "Error: Server returned a non-200 status code." << std::endl;
    }
            
    /* close & exit */
    close (sd);
    exit (0);
}
