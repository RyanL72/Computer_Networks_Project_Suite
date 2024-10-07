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

#define ERROR 1
#define REQUIRED_ARGC 3
#define HOST_POS 1
#define PORT_POS 2
#define PROTOCOL "tcp"
#define BUFLEN 1024
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

// Function to parse the URL and extract the hostname and port
void parse_url(const char *url, char *hostname, int *port) {
    const char *url_prefix = "http://";  // Define the URL prefix
    int default_port = 80;               // Default port for HTTP

    // Check if the URL starts with "http://"
    if (strncasecmp(url, url_prefix, strlen(url_prefix)) != 0) {
        errexit("Invalid URL format. URL must start with 'http://'");
    }

    // Move past "http://"
    const char *host_start = url + strlen(url_prefix);
    const char *port_start = strchr(host_start, ':');
    const char *path_start = strchr(host_start, '/');

    if (port_start && (!path_start || port_start < path_start)) {
        // There is a port specified in the URL
        strncpy(hostname, host_start, port_start - host_start);
        hostname[port_start - host_start] = '\0'; // Null-terminate the hostname string
        *port = atoi(port_start + 1);  // Convert the port string to an integer
    } else if (path_start) {
        // No port specified, use the default port 80
        strncpy(hostname, host_start, path_start - host_start);
        hostname[path_start - host_start] = '\0'; // Null-terminate the hostname string
        *port = default_port;  // Use the default HTTP port 80
    } else {
        // Only hostname, no port or path specified
        strcpy(hostname, host_start);
        *port = default_port;  // Use the default HTTP port 80
    }
}


int main (int argc, char *argv [])
{   
    //structure used to specify the address for this socket.
    struct sockaddr_in sin;

    //structure that holds information about a host 
    struct hostent *hinfo;

    //structure containing information about a protocal
    struct protoent *protoinfo;

    //store data being sent or received over the network
    //BUFLEN is typically defined elsewhere and represents the maximum size of the data 
    char buffer [BUFLEN];

    //sd socket descriptor, an integer value returned by the socket() representing the unique identifier for a socket connection
    //ret variable used to store return values from various socket functions. It is typically used to check for success or error codes
    int sd, ret;

    /*Command Line*/

    //variables to store command-line options
    char *url= NULL, *filename=NULL;
    int show_info = 0, show_request = 0, show_response = 0;

    //command-line argument parsing
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
                show_info=1;
                break;
            case 'q':
                show_request=1;
                break;
            case 'a':
                show_response=1;
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
    
    /* lookup the hostname */
    hinfo = gethostbyname (argv [HOST_POS]);
    if (hinfo == NULL)
        errexit ("cannot find name: %s", argv [HOST_POS]);

    /* set endpoint information */
    memset ((char *)&sin, 0x0, sizeof (sin));
    sin.sin_family = AF_INET;
    //host to netoworks hton
    sin.sin_port = htons (atoi (argv [PORT_POS]));
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

    /* snarf whatever server provides and print it */
    memset (buffer,0x0,BUFLEN);
    ret = read (sd,buffer,BUFLEN - 1);
    if (ret < 0)
        errexit ("reading error",NULL);
    fprintf (stdout,"%s\n",buffer);
            
    /* close & exit */
    close (sd);
    exit (0);
}
