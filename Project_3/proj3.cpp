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

#define REQUIRED_ARGC 6
#define PORT_POS 1
#define MSG_POS 2
#define ERROR 1
#define QLEN 1
#define PROTOCOL "tcp"
#define BUFLEN 1024
#define PORT_NUMBER_POS -1
#define ROOT_DIRECTORY_POS -1 
#define TERMINATION_TOKEN_POS -1


int usage (char *progname)
{
    fprintf (stderr,"usage: %s port msg\n", progname);
    exit (ERROR);
}

int errexit (std::string format, std::string arg)
{
    std::cerr << format << arg << std::endl;
    fprintf (stderr,"\n");
    exit (ERROR);
}



void parseArgs(int argc, char *argv []){
    try{
        for(int i = 1; i < argc; i++){
            switch(argv [i]){
                case "-p":
                    i++;
                    PORT_NUMBER_POS = i;
                    break;
                case "-r":
                    i++;
                    ROOT_DIRECTORY_POS = i;
                    break;
                case "-t":
                    i++;
                    TERMINATION_TOKEN_POS = i;
                    break;
            }
        }
    }
    catch(const char* msg){
        std::cout << "bruh" << msg;
    }
}

int main (int argc, char *argv [])
{
    struct sockaddr_in sin;
    struct sockaddr addr;
    struct protoent *protoinfo;
    unsigned int addrlen;
    int sd, sd2;
    
    if (argc != REQUIRED_ARGC)
        usage (argv [0]);
    
    parseArgs(argc, argv);
    std::cout << PORT_NUMBER_POS << std::endl;
    std::cout << ROOT_DIRECTORY_POS << std::endl;
    std::cout << TERMINATION_TOKEN_POS << std::endl;

    // for(int i = 0; i < argc; i++){
    //     std::cout << argv[i] << "\n";
    // }

    /* determine protocol */
    // if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
    //     errexit ("cannot find protocol information for ", PROTOCOL);

    // /* setup endpoint info */
    // memset ((char *)&sin,0x0,sizeof (sin));
    // sin.sin_family = AF_INET;
    // sin.sin_addr.s_addr = INADDR_ANY;
    // sin.sin_port = htons ((u_short) atoi (argv [PORT_POS]));

    // /* allocate a socket */
    // /*   would be SOCK_DGRAM for UDP */
    // sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    // if (sd < 0)
    //     errexit("cannot create socket", NULL);

    // /* bind the socket */
    // if (bind (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    //     errexit ("cannot bind to port %s", argv [PORT_POS]);

    // /* listen for incoming connections */
    // if (listen (sd, QLEN) < 0)
    //     errexit ("cannot listen on port %s\n", argv [PORT_POS]);

    // /* accept a connection */
    // addrlen = sizeof (addr);
    // sd2 = accept (sd,&addr,&addrlen);
    // if (sd2 < 0)
    //     errexit ("error accepting connection", NULL);
    
    // /* write message to the connection */
    // if (write (sd2,argv [MSG_POS],strlen (argv [MSG_POS])) < 0)
    //     errexit ("error writing message: %s", argv [MSG_POS]);

    // /* close connections and exit */
    // close (sd);
    // close (sd2);
    exit (0);
}