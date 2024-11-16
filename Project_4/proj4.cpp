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
#include <map>
#include <utility>

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
#define MISSING_INFO "-"
#define META_MISSING_INFO "Cannot read meta information"
#define UNKNOWN '?'

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

    bytes_read = read(fd, &meta, int(sizeof(meta)));
    if (bytes_read == 0)
        return (0);
    if (bytes_read < int(sizeof(meta)))
        errexit(META_MISSING_INFO);

    meta.secs = ntohl(meta.secs);
    meta.usecs = ntohl(meta.usecs);
    meta.caplen = ntohs(meta.caplen);

    pinfo->now = meta.secs + (meta.usecs / 1e6);

    pinfo->caplen = meta.caplen;

    if (pinfo->caplen == 0)
        return (1);
    if (pinfo->caplen > MAX_PKT_SIZE)
        errexit("Packet too big");

    bytes_read = read(fd, pinfo->pkt, pinfo->caplen);

    if (bytes_read < 0)
        errexit("Error reading packet");
    if (bytes_read < pinfo->caplen)
        errexit("Unexpected end of file encountered");
    if (bytes_read < int(sizeof(struct ether_header)))
        return (1);

    pinfo->ethh = (struct ether_header *)pinfo->pkt;
    pinfo->ethh->ether_type = ntohs(pinfo->ethh->ether_type);

    if (pinfo->ethh->ether_type == ETHERTYPE_IP && pinfo->caplen >= (sizeof(struct ether_header) + sizeof(struct iphdr))) { 
        pinfo->iph = (struct iphdr *)(pinfo->pkt + sizeof(struct ether_header));
    }

    if (pinfo->iph != NULL) {
        if (pinfo->iph->protocol == TCP && pinfo->caplen >= (sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct tcphdr))) {
            pinfo->tcph = (struct tcphdr *)((unsigned char *)pinfo->iph + (pinfo->iph->ihl * 4));
        } 
        else if (pinfo->iph->protocol == UDP &&
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
            double packet_time = pinfo.now;
            if (total_pkts == 1) {
                first_time = packet_time;
            }
            last_time = packet_time;

            if (pinfo.ethh != NULL && pinfo.ethh->ether_type == ETHERTYPE_IP) {
                IP_pkts++;
            }
        }

        printf("%s %.6f %.6f %d %d\n", fileName.c_str(), first_time, last_time - first_time, total_pkts, IP_pkts);
    }
    else if (mode == SIZE_ANALYSIS_MODE) {
        struct pkt_info pinfo;
        while (next_packet(fd, &pinfo)) {
            if (pinfo.ethh == NULL) {
                continue;
            }

            double packet_time = pinfo.now;
            unsigned int caplen = pinfo.caplen;

            if (pinfo.iph == NULL) {
                std::cout << std::fixed << std::setprecision(6) << packet_time << " " << caplen << " - - - - -" << std::endl;
                continue;
            }

            unsigned int ip_len = ntohs(pinfo.iph->tot_len);
            unsigned int ihl = pinfo.iph->ihl * 4;

            int protocol = pinfo.iph->protocol;
            char protocolChar;
            std::string trans_hl = MISSING_INFO;
            std::string payload = MISSING_INFO;

            if (protocol == TCP) {
                protocolChar = 'T';
                if (pinfo.tcph != NULL) {
                    trans_hl = std::to_string(pinfo.tcph->doff * 4);
                    payload = std::to_string(ip_len - ihl - (pinfo.tcph->doff * 4));
                } else {
                    trans_hl = MISSING_INFO;
                    payload = MISSING_INFO;
                }
            } 
            else if (protocol == UDP) {
                protocolChar = 'U';
                trans_hl = std::to_string(UDP_HEADER_LEN);
                payload = std::to_string(ip_len - ihl - UDP_HEADER_LEN);
            } 
            else {
                protocolChar = UNKNOWN;
                trans_hl = UNKNOWN;
                payload = UNKNOWN;
            }

            std::cout << std::fixed << std::setprecision(6) << packet_time << " " << caplen << " " << ip_len << " " << ihl << " " << protocolChar << " " << trans_hl << " " << payload << std::endl;
        }
    }
    else if (mode == TCP_PACKET_PRINTING_MODE) {
        struct pkt_info pinfo;

        while (next_packet(fd, &pinfo)) {
            if (pinfo.ethh == NULL) {
                continue;
            }

            if (pinfo.iph == NULL) {
                continue;
            }

            if (pinfo.iph->protocol != TCP) {
                continue;
            }

            if (pinfo.tcph == NULL) {
                continue;
            }

            double timestamp = pinfo.now;
            std::string src_ip = inet_ntoa(*(struct in_addr *)&pinfo.iph->saddr);
            std::string dst_ip = inet_ntoa(*(struct in_addr *)&pinfo.iph->daddr);
            unsigned int ip_ttl = pinfo.iph->ttl;
            unsigned int ip_id = ntohs(pinfo.iph->id);
            unsigned int src_port = ntohs(pinfo.tcph->source);
            unsigned int dst_port = ntohs(pinfo.tcph->dest);
            bool syn_flag = pinfo.tcph->syn;
            std::string syn = syn_flag ? "Y" : "N";
            unsigned int window = ntohs(pinfo.tcph->window);
            unsigned int seqno = ntohl(pinfo.tcph->seq);

            std::cout << std::fixed << std::setprecision(6) << timestamp << " " << src_ip << " " << src_port << " " << dst_ip  << " " << dst_port << " " << ip_ttl << " " << ip_id << " " << syn << " " << window << " " << seqno << std::endl;
        }
    }
    else if (mode == TRAFFIC_MATRIX_MODE) {
        struct pkt_info pinfo;

        std::map<std::pair<std::string, std::string>, std::pair<int, int>> traffic_matrix;

        while (next_packet(fd, &pinfo)) {
            if (pinfo.ethh == NULL) {
                continue;
            }

            if (pinfo.iph == NULL) {
                continue;
            }

            if (pinfo.iph->protocol != TCP) {
                continue;
            }

            if (pinfo.tcph == NULL) {
                continue;
            }

            std::string src_ip = inet_ntoa(*(struct in_addr *)&pinfo.iph->saddr);
            std::string dst_ip = inet_ntoa(*(struct in_addr *)&pinfo.iph->daddr);
            unsigned int ip_len = ntohs(pinfo.iph->tot_len);
            unsigned int ihl = pinfo.iph->ihl * 4;
            unsigned int trans_hl = pinfo.tcph->doff * 4;
            int payload_len = ip_len - ihl - trans_hl;
            auto key = std::make_pair(src_ip, dst_ip);
            if (traffic_matrix.find(key) == traffic_matrix.end()) {
                traffic_matrix[key] = std::make_pair(0, 0);
            }
            traffic_matrix[key].first += 1;
            traffic_matrix[key].second += payload_len;
        }

        for (const auto &entry : traffic_matrix) {
            const auto &key = entry.first;
            const auto &value = entry.second;
            std::cout << key.first << " " << key.second << " " << value.first << " " << value.second << std::endl;
        }
    }

    close(fd);

    return 0;    
}
