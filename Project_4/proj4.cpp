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

#define INFORMATION_MODE 1
#define SIZE_ANALYSIS_MODE 2
#define TCP_PACKET_PRINTING_MODE 3
#define TRAFFIC_MATRIX_MODE 4
#define ERROR 1
#define MISSING_FILE_NAME_MSG "Error: You must specify a trace file using the -r option."
#define ONE_MODE_MSG "Must specify one mode."

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

    // std::cout << "Mode " << mode << " with argc: " << argc << std::endl;

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

                // // Print all attributes of the first packet
                // printf("First Packet Attributes:\n");
                // printf("  Timestamp (now): %.6f seconds\n", pinfo.now);
                // printf("  Captured Length (caplen): %u bytes\n", pinfo.caplen);

                // // Print Ethernet header details if present
                // if (pinfo.ethh != NULL) {
                //     printf("  Ethernet Header:\n");
                //     printf("    Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                //         pinfo.ethh->ether_dhost[0], pinfo.ethh->ether_dhost[1],
                //         pinfo.ethh->ether_dhost[2], pinfo.ethh->ether_dhost[3],
                //         pinfo.ethh->ether_dhost[4], pinfo.ethh->ether_dhost[5]);
                //     printf("    Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                //         pinfo.ethh->ether_shost[0], pinfo.ethh->ether_shost[1],
                //         pinfo.ethh->ether_shost[2], pinfo.ethh->ether_shost[3],
                //         pinfo.ethh->ether_shost[4], pinfo.ethh->ether_shost[5]);
                //     printf("    Ether Type: 0x%04x\n", pinfo.ethh->ether_type);
                // } else {
                //     printf("  Ethernet Header: Not Present\n");
                // }

                // // Print IP header details if present
                // if (pinfo.iph != NULL) {
                //     printf("  IP Header:\n");
                //     printf("    Source IP: %s\n", inet_ntoa(*(struct in_addr *)&pinfo.iph->saddr));
                //     printf("    Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&pinfo.iph->daddr));
                //     printf("    Protocol: %u\n", pinfo.iph->protocol);
                //     printf("    Total Length: %u bytes\n", ntohs(pinfo.iph->tot_len));
                //     printf("    Time to Live (TTL): %u\n", pinfo.iph->ttl);
                // } else {
                //     printf("  IP Header: Not Present\n");
                // }

                // // Print TCP header details if present
                // if (pinfo.tcph != NULL) {
                //     printf("  TCP Header:\n");
                //     printf("    Source Port: %u\n", ntohs(pinfo.tcph->source));
                //     printf("    Destination Port: %u\n", ntohs(pinfo.tcph->dest));
                //     printf("    Sequence Number: %u\n", ntohl(pinfo.tcph->seq));
                //     printf("    Acknowledgment Number: %u\n", ntohl(pinfo.tcph->ack_seq));
                //     printf("    Flags: 0x%02x\n", ((unsigned char *)pinfo.tcph)[13]);
                //     printf("    Window Size: %u\n", ntohs(pinfo.tcph->window));
                // } else {
                //     printf("  TCP Header: Not Present\n");
                // }

                // // Print UDP header details if present
                // if (pinfo.udph != NULL) {
                //     printf("  UDP Header:\n");
                //     printf("    Source Port: %u\n", ntohs(pinfo.udph->source));
                //     printf("    Destination Port: %u\n", ntohs(pinfo.udph->dest));
                //     printf("    Length: %u bytes\n", ntohs(pinfo.udph->len));
                // } else {
                //     printf("  UDP Header: Not Present\n");
                // }
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

    // if (mode == INFORMATION_MODE) {
    //     double first_time = 0.0, last_time = 0.0;
    //     int total_pkts = 0, IP_pkts = 0;

    //     struct pkt_info pinfo;
    //     while (next_packet(fd, &pinfo)) {
    //         total_pkts++;

    //         // Use the `now` field from `pkt_info`
    //         double packet_time = pinfo.now;
    //         if (total_pkts == 1) {
    //             first_time = packet_time;
    //         }
    //         last_time = packet_time;

    //         // Check if the packet is IPv4
    //         if (pinfo.ethh != NULL && pinfo.ethh->ether_type == ETHERTYPE_IP) {
    //             IP_pkts++;
    //         }
    //     }

    //     // Output the summary in the required format
    //     printf("%s %.6f %.6f %d %d\n", fileName.c_str(), first_time, last_time - first_time, total_pkts, IP_pkts);
    // }

    // Close the file descriptor
    close(fd);

    return 0;    
}
