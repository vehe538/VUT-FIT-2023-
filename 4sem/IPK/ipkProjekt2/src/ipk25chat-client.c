#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "utils.h"
#include "messages.h"
#include "chat_clients.h"

appStatus client_info;


int main(int argc, char *argv[]) {

    if (argc == 1) {
        printHelp();
        exit(0);
    } 
    if (strcmp(argv[1], "-h") == 0) {
        printHelp();
        exit(0);
    }

    char *hostaddress;
    char *protocol;

    //optionals
    int server_port = 4567;
    int udp_timeout = 250;
    int max_retransmissions = 3;

    int has_proto = 0;
    int has_ip = 0;
    int has_port = 0;
    int has_timeout = 0;
    int has_transmissionlimit = 0;

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-s") == 0) {
            
            hostaddress = malloc(strlen(argv[i+1])+1);
            strcpy(hostaddress, argv[i+1]);
            has_ip = 1;

        }

        if (strcmp(argv[i], "-t") == 0) {

            protocol = malloc(strlen(argv[i+1])+1);
            strcpy(protocol, argv[i+1]);
            has_proto = 1;

        }

        if (strcmp(argv[i], "-p") == 0) {

            server_port = atoi(argv[i+1]);
            has_port = 1;

        }

        if (strcmp(argv[i], "-d") == 0) {

            udp_timeout = atoi(argv[i+1]);
            has_timeout = 1;

        }

        if (strcmp(argv[i], "-r") == 0) {

            max_retransmissions = atoi(argv[i+1]);
            has_transmissionlimit = 1;

        }

    }

    if (!has_ip || !has_proto) {
        printf("ERROR: missing one of mandatory arguments.\n");
        exit(EXIT_FAILURE);
    }

    // termination system signal handle
    signal(SIGINT, connectionTerm);

    //convert string to valid ip address
    struct addrinfo hints, *res;
    char ip_str[INET_ADDRSTRLEN];
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    if (getaddrinfo(hostaddress, NULL, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, sizeof(ip_str));

    //printf("IPv4 address: %s\n", ip_str);

    //set known global variables
    strncpy(client_info.ip_str, ip_str, sizeof(client_info.ip_str));
    client_info.server_port = server_port;
    client_info.max_retransmissions = max_retransmissions;
    client_info.udp_timeout = udp_timeout;
    client_info.protocol = IPPROTO_UDP;

    if (strcmp(protocol, "tcp") == 0) {

        clientTcp(server_port, ip_str);

    }
    else if (strcmp(protocol, "udp") == 0) {

        clientUdp(server_port, ip_str);

    }

    
    free(hostaddress);
    free(protocol);


}