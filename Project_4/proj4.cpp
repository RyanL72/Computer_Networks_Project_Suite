/*
Author: Ryan Lin
CaseID: rhl72
File name: proj4.cpp
Date: 11-14-2024
Description: Packet Trace Analyzer 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include "next.h"
#include <string>

using std::string;
using namespace std;

#define INFORMATION_MODE 1
#define SIZE_ANALYSIS_MODE 2
#define TCP_PACKET_PRINTING_MODE 3
#define TRAFFIC_MATRIX_MODE 4
#define ERROR 1

void errexit (char *msg) {
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

int usage (string progname) {
    std::cout << progname << " is an unknown option, please use the format: \n" 
              << "./proj4 -r <Trace File_name> -r <Root directory> -t <Terminate token>" 
              << std::endl;
    exit(ERROR);
}

unsigned short next_packet (int fd, struct pkt_info *pinfo) {
    struct meta_info meta;
    int bytes_read;

    memset(pinfo, 0x0, sizeof(struct pkt_info));
    memset(&meta, 0x0, sizeof(struct meta_info));

    bytes_read = read(fd, &meta, sizeof(meta));
    if (bytes_read == 0)
        return (0);
    if (bytes_read < sizeof(meta))
        errexit("cannot read meta information");
    pinfo->caplen = ntohs(meta.caplen);

    if (pinfo->caplen == 0)
        return (1);
    if (pinfo->caplen > MAX_PKT_SIZE)
        errexit("packet too big");

    bytes_read = read(fd, pinfo->pkt, pinfo->caplen);
    if (bytes_read < 0)
        errexit("error reading packet");
    if (bytes_read < pinfo->caplen)
        errexit("unexpected end of file encountered");
    if (bytes_read < sizeof(struct ether_header))
        return (1);

    pinfo->ethh = (struct ether_header *)pinfo->pkt;
    pinfo->ethh->ether_type = ntohs(pinfo->ethh->ether_type);
    if (pinfo->ethh->ether_type != ETHERTYPE_IP)
        return (1);
    if (pinfo->caplen == sizeof(struct ether_header))
        return (1);

    return (1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage(argv[0]);
    }

    char mode = 0;
    string fileName;

    int opt;
    while((opt = getopt(argc, argv, "ristm")) != -1) {
        switch(opt) {
            case 'r':
                fileName = optarg;
                break;
            case 'i':
                mode = INFORMATION_MODE;
                break;
            case 's':
                mode = SIZE_ANALYSIS_MODE;
                break;
            case 't':
                mode = TCP_PACKET_PRINTING_MODE;
                break;
            case 'm':
                mode = TRAFFIC_MATRIX_MODE;
                break;
            case '?':
                usage(std::to_string(opt));
                break;
        }
    }

    cout << mode << endl;

    // int fd = open(fileName.c_str(), O_RDONLY);
    // if (fd < 0) {
    //     perror("Error opening file");
    //     return 1;
    // }

    // struct pkt_info pinfo;
    // while (next_packet(fd, &pinfo)) {
    //     printf("Packet length: %u\n", pinfo.caplen);
    //     if (pinfo.ethh != NULL && pinfo.ethh->ether_type == ETHERTYPE_IP) {
    //         printf("This packet contains an IP header.\n");
    //     }
    // }

    // close(fd);
    return 0;
}
