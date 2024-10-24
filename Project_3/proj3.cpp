/*
Author: Ryan Lin
CaseID: rhl72
File name: proj1.cpp
Date: 10-21-2024
Description: IPv4 format checker
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <cstdlib>   // for exit()
#include <string>    // for std::string
#include <cctype>    // for isdigit
#include <stdexcept> // for std::stoi

#define REQUIRED_ARGC 6
#define PORT_POS 1
#define MSG_POS 2
#define ERROR 1
#define QLEN 1
#define PROTOCOL "tcp"
#define BUFLEN 1024

int usage (std::string progname)
{
    std::cout << progname << " is an unknown option, please use the format: \n" <<  "./proj3 -p <Port Number> -r <Root directory> -t <Terminate token>" << std::endl;
    exit (ERROR);
}

int badArgumentValues(std::string message="Bad Argument Value Error"){
    std::cout << message << std::endl;
    std::cout << "Argument values were invalid: \n" <<  "Please use the format: ./proj3 -p <Port Number> -r <Root directory> -t <Terminate token>" << std::endl;
    exit (ERROR);
}

int ArgumentValidation(int port, std::string root, std::string token){
    if(port <= 1024){
    badArgumentValues("Port Numbers less than 1024 are reserved.");
    }
    return 0;
}

int errexit (std::string format, std::string arg)
{
    std::cerr << format << arg << std::endl;
    fprintf (stderr,"\n");
    exit (ERROR);
}


int main (int argc, char *argv [])
{
    //for ipv4
    struct sockaddr_in sin;
    
    //address for ipv4 or ipv6
    struct sockaddr addr;

    //protocal information 
    struct protoent *protoinfo;

    //length of address
    unsigned int addrlen;

    //socket descriptors 
    int sd, sd2;
    
    //check to see if the right options were provided.
    int portNumber = -1;
    std::string rootDirectory = "";
    std::string terminationToken = "";

    //get the options.
    int opt;
    while((opt = getopt(argc, argv,"p:r:t:")) != -1){
        switch(opt){
            case 'p':
                portNumber = atoi(optarg);
                break;
            case 'r':   
                rootDirectory = optarg;
                break;
            case 't':
                terminationToken = optarg;
                break;
            case '?':
                usage(std::to_string(opt));
                break;
        }
    }
    //check to see if all the port, root directory, and terminate token were provided
    std::cout << portNumber << std::endl;
    std::cout << rootDirectory << std::endl;
    std::cout << terminationToken << std::endl;
    
    //handle missing things
    if(portNumber == -1 || rootDirectory.empty() || terminationToken.empty() ){
        badArgumentValues();
    }

    //validate the port
    ArgumentValidation(portNumber, rootDirectory, terminationToken);

    /* determine protocol */
    if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
        errexit ("cannot find protocol information for ", PROTOCOL);
    
    std::cout << "Protocol: " << protoinfo->p_name << "\n" << "Number: " << protoinfo->p_proto << std::endl;

    /* setup endpoint info */

    // (char *)&sin means converting memory address of sin into a byte ptr
    // 0x0 is value to set this memory to
    // sizeof (sin) is sockaddr_in size 
    /*
    so this is saying set memory to this address of sin (in the form of a pointer to a 
    char or byte- typically used in memory level access) to the value of 0x0 
    and allocate a size of however big sin is */
    memset ((char *)&sin,0x0,sizeof (sin));

    // AF_INET is for ipv4
    sin.sin_family = AF_INET;

    // INADDR_ANY tells the socket to listen to all available interfaces
    sin.sin_addr.s_addr = INADDR_ANY;

    // host to networks byte order (Big Endian) 
    //Not sure if this should be my port number or what. Consult the socketsd.c for original.
    sin.sin_port = htons ((u_short) (portNumber));
 
    /* allocate a socket */
    /*   would be SOCK_DGRAM for UDP */
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
        errexit("cannot create socket", NULL);
    
    /* bind the socket */
    if (bind (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit ("cannot bind to port %s", argv [PORT_POS]);

    /* listen for incoming connections */
    if (listen (sd, QLEN) < 0)
        errexit ("cannot listen on port %s\n", argv [PORT_POS]);

    /* accept a connection */
    addrlen = sizeof (addr);
    sd2 = accept (sd,&addr,&addrlen);
    if (sd2 < 0)
        errexit ("error accepting connection", NULL);
    
    /* write message to the connection */
    if (write (sd2,argv [MSG_POS],strlen (argv [MSG_POS])) < 0)
        errexit ("error writing message: %s", argv [MSG_POS]);

    /* close connections and exit */
    close (sd);
    close (sd2);
    exit (0);
}