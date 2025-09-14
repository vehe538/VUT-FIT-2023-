
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h> 
#include <netinet/ip.h>      
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>    

void tcpScan(char *interface, long tcp_ports[65535], char *dst_address, char errbuf[PCAP_ERRBUF_SIZE], pcap_t *response, int timeout);
void udpScan(char *interface, long udp_ports[65535], char *dst_address, char errbuf[PCAP_ERRBUF_SIZE], pcap_t *response, int timeout);