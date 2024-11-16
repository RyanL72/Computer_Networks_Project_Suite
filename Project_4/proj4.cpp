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
#include <fstream>
#include <arpa/inet.h>
#include <iomanip> 

#define INFORMATION_MODE 1
#define SIZE_ANALYSIS_MODE 2
#define TCP_PACKET_PRINTING_MODE 3
#define TRAFFIC_MATRIX_MODE 4
#define ERROR 1
#define MISSING_FILE_NAME_MSG "Error: You must specify a trace file using the -r option."
#define ONE_MODE_MSG "Must specify one mode."
#define TCP 6
#define UDP 17
#define UDP_HEADER_LEN 8

void errexit(std::string msg) {
    std::cerr << msg << std::endl;
    exit(1);
}

int usage (std::string progname) {
    std::cout << progname << " is an unknown option, please use the format: \n" 
              << "./proj4 -r <Trace File_name> -i|-t|-s|-m" << std::endl;
    exit(ERROR);
}

unsigned short next_packet(int fd, struct pkt_info *pinfo) {
    struct meta_info meta;
    int bytes_read;

    memset(pinfo, 0x0, sizeof(struct pkt_info));
    memset(&meta, 0x0, sizeof(struct meta_info));

    // Read the meta information (12 bytes)
    bytes_read = read(fd, &meta, int(sizeof(meta)));
    if (bytes_read == 0)
        return (0); // End of file
    if (bytes_read < int(sizeof(meta)))
        errexit("Cannot read meta information");

    // Convert metadata fields to host byte order
    meta.secs = ntohl(meta.secs); // Convert 4-byte seconds
    meta.usecs = ntohl(meta.usecs); // Convert 4-byte microseconds
    meta.caplen = ntohs(meta.caplen); // Convert 2-byte caplen

    // Calculate the timestamp
    pinfo->now = meta.secs + (meta.usecs / 1e6); // Combine secs and usecs

    pinfo->caplen = meta.caplen;

    if (pinfo->caplen == 0)
        return (1); // Skip if no packet captured
    if (pinfo->caplen > MAX_PKT_SIZE)
        errexit("Packet too big");

    // Read the packet data
    bytes_read = read(fd, pinfo->pkt, pinfo->caplen);
    if (bytes_read < 0)
        errexit("Error reading packet");
    if (bytes_read < pinfo->caplen)
        errexit("Unexpected end of file encountered");
    if (bytes_read < int(sizeof(struct ether_header)))
        return (1); // Skip if no Ethernet header

    // Parse Ethernet header
    pinfo->ethh = (struct ether_header *)pinfo->pkt;
    pinfo->ethh->ether_type = ntohs(pinfo->ethh->ether_type); // Convert type to host byte order

    // Parse IP header (if Ethernet type is IPv4 and caplen allows)
    if (pinfo->ethh->ether_type == ETHERTYPE_IP &&
        pinfo->caplen >= (sizeof(struct ether_header) + sizeof(struct iphdr))) {
        pinfo->iph = (struct iphdr *)(pinfo->pkt + sizeof(struct ether_header));
    }

    // Parse TCP/UDP headers based on IP protocol (if IP header exists)
    if (pinfo->iph != NULL) {
        if (pinfo->iph->protocol == IPPROTO_TCP &&
            pinfo->caplen >= (sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct tcphdr))) {
            pinfo->tcph = (struct tcphdr *)((unsigned char *)pinfo->iph + (pinfo->iph->ihl * 4));
        } else if (pinfo->iph->protocol == IPPROTO_UDP &&
                   pinfo->caplen >= (sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr))) {
            pinfo->udph = (struct udphdr *)((unsigned char *)pinfo->iph + (pinfo->iph->ihl * 4));
        }
    }

    return (1);
}



int main(int argc, char *argv[]) {

    int mode;
    std::string fileName;

    if(argc < 3 || argc > 4){
        // for (int i = 0; i < argc; ++i) {
        //     std::cout << "Argument " << i << ": " << argv[i] << std::endl;
        // }
        // return -1;
        usage(ONE_MODE_MSG);
    }

    int opt;
    while((opt = getopt(argc, argv, "r:istm")) != -1) {
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

    if (fileName.empty()) {
        usage(MISSING_FILE_NAME_MSG);
    }

    // Open the file using a low-level file descriptor for packet processing
    int fd = open(fileName.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        exit(1);
    }

    if (mode == INFORMATION_MODE) {
        double first_time = 0.0, last_time = 0.0;
        int total_pkts = 0, IP_pkts = 0;

        struct pkt_info pinfo;
        while (next_packet(fd, &pinfo)) {
            total_pkts++;

            // Use the `now` field from `pkt_info`
            double packet_time = pinfo.now;
            if (total_pkts == 1) {
                first_time = packet_time;
            }
            last_time = packet_time;

            // Check if the packet is IPv4
            if (pinfo.ethh != NULL && pinfo.ethh->ether_type == ETHERTYPE_IP) {
                IP_pkts++;
            }
        }

        // Output the summary in the required format
        printf("%s %.6f %.6f %d %d\n", fileName.c_str(), first_time, last_time - first_time, total_pkts, IP_pkts);
    }
    else if (mode == SIZE_ANALYSIS_MODE) {
        struct pkt_info pinfo;
        while (next_packet(fd, &pinfo)) {
            // Ethernet and if minimum length is satisfied
            if (pinfo.ethh == NULL ) {
                continue; // Skip packets without Ethernet headers
            }

            // Time Stamp
            double packet_time = pinfo.now;

            // Captured Length
            unsigned int caplen = pinfo.caplen;

            if (pinfo.iph == NULL) {
                std::cout << std::fixed << std::setprecision(6) << packet_time << " " << caplen << " - - - - -" << std::endl;
                continue;
            }

            // IPv4 Packet Length
            unsigned int ip_len = ntohs(pinfo.iph->tot_len);

            // IPv4 Header Length (in bytes)
            unsigned int ihl = pinfo.iph->ihl * 4;

            // Protocol, Transport Header Length, and Payload
            int protocol = pinfo.iph->protocol;
            char protocolChar;
            std::string trans_hl = "-";
            std::string payload = "-";

            if (protocol == TCP) {
                protocolChar = 'T';
                if (pinfo.tcph != NULL) {
                    trans_hl = std::to_string(pinfo.tcph->doff * 4);
                    payload = std::to_string(ip_len - ihl - (pinfo.tcph->doff * 4));
                } else {
                    trans_hl = "-";
                    payload = "-";
                }
            } 
            else if (protocol == UDP) {
                protocolChar = 'U';
                trans_hl = std::to_string(UDP_HEADER_LEN);
                payload = std::to_string(ip_len - ihl - UDP_HEADER_LEN);
            } 
            else {
                protocolChar = '?';
                trans_hl = "?";
                payload = "?";
            }


            // Print Row
            std::cout << std::fixed << std::setprecision(6) << packet_time << " " << caplen << " "
                    << ip_len << " " << ihl << " " << protocolChar << " " << trans_hl << " " << payload << std::endl;
        }
    }


    close(fd);

    return 0;    
}
