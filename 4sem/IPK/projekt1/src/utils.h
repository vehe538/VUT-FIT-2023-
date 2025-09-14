
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
#include <netinet/ip.h>      // For IP header
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>    

void listInterfaces();
int checkInterfaces(char *input);
int isRange(char *arg);
int isDefined(char *arg, int order, int number_of_args);
void setPorts(long ports[], char *arg);
void processPacket(const unsigned char *packet, u_int8_t port_type, char *ip_str, int port_number, pcap_t *response);
void sendPacket(int socket, char *packet_message, struct sockaddr *dest, int ip_version);
void captureResponse(pcap_t *response, char *ip_str, int port_number, int protocol);