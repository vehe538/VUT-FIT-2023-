 #include "utils.h"
 #include "scanners.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 

int main(int argc, char *argv[]) {

    long tcp_ports[65535];
    long udp_ports[65535];
    char interface[50];
    char dst_address[50];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *response;
    int timeout = 5000; // in milliseconds
    

    if (argc == 1) {
        fprintf(stderr, "Error: missing arguments.\n");
        exit(EXIT_FAILURE);
    }
 
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {

        printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("USAGE: ./ipk-l4-scan [-i interface | --interface interface] [--pu port-ranges | --pt port-ranges | -u port-ranges | -t port-ranges] {-w timeout} [hostname | ip-address].\n");
        printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        exit(EXIT_SUCCESS);
    }
 
 
    int interface_set = 0, tcp_set = 0, udp_set = 0, address_set = 0;

    for (int i = 1; i < argc; i++) {
        
        // Interface handling
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interface") == 0) {
            if (i == argc-1 || isDefined(argv[i+1], i+1, argc) == 0) {

                listInterfaces();
                exit(EXIT_FAILURE);
            } else if (checkInterfaces(argv[i+1]) == 0) {
                listInterfaces();
                exit(EXIT_SUCCESS);
            }
            strcpy(interface, argv[i+1]);
            interface_set++;
        }
        
        // TCP ports handling
        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--pt") == 0) {
            if (i == argc-1 || isDefined(argv[i+1], i+1, argc) == 0) {
    
                listInterfaces();
                exit(EXIT_FAILURE);
            }
            setPorts(tcp_ports, argv[i+1]);
            tcp_set++;
        }

        // UDP ports handling
        else if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--pu") == 0) {
            if (i == argc-1 || isDefined(argv[i+1], i+1, argc) == 0) {
                listInterfaces();
                exit(EXIT_FAILURE);
            }
            setPorts(udp_ports, argv[i+1]);
            udp_set++;
        }

        // Timeout handling
        else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--wait") == 0) {
            if (i == argc-1 || isDefined(argv[i+1], i+1, argc) == 0) {
                listInterfaces();
                exit(EXIT_FAILURE);
            }
            timeout = atol(argv[i+1]);
        }

        // Destination address handling (IP Address)
        else if (strlen(argv[i]) > 8 && argv[i-1][0] != '-') {
            strcpy(dst_address, argv[i]);
            address_set++;
        }
    }

    // Check if any required flag is missing
    if (interface_set != 1 || (udp_set != 1 && tcp_set != 1) || address_set != 1) {

        listInterfaces();
        exit(EXIT_FAILURE);
    }
    
    if (argc < 5) {

        listInterfaces();
        exit(EXIT_FAILURE);
    }
     
    // scanning UDP ports if specified
    udpScan(interface, udp_ports, dst_address, errbuf, response, timeout);
    // scanning TCP ports if specified
    tcpScan(interface, tcp_ports, dst_address, errbuf, response, timeout);
     
    exit(EXIT_SUCCESS);
}
 