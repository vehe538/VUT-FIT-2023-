#include "utils.h"

// lists available interfaces 
void listInterfaces() {

    pcap_if_t *all;
    pcap_if_t *interface;
    char error_buffer[PCAP_ERRBUF_SIZE];

    pcap_findalldevs(&all, error_buffer);

    printf("Available interfaces:\n\n");

    for (interface = all; interface != NULL; interface = interface->next) {

        printf("%s\n", interface->name);
    
    }
    pcap_freealldevs(all);

}

//checks if interface from user input is valid
int checkInterfaces(char *input) {

    int available = 0;

    pcap_if_t *all;
    pcap_if_t *interface;
    char error_buffer[PCAP_ERRBUF_SIZE];

    pcap_findalldevs(&all, error_buffer);
    for (interface = all; interface != NULL; interface = interface->next) {

        if (strcmp(input, interface->name) == 0) {
            available = 1;
        }
    
    }

    pcap_freealldevs(all);
    return available;

}

// detect if user input range of ports or not
int isRange(char *arg) {  

    int isrange = 0;
    for (int i = 0; i < strlen(arg); i++) {

        if (((arg[i] == ',') && isrange == 1) || ((arg[i] == '-') && isrange == 1)) {
            return -1;
        }

        if (arg[i] == '-') {
            isrange = 1;
        }

    }

    return isrange;

}
 
int isDefined(char *arg, int order, int number_of_args) {                   

    // this function is mostly used for checking if value of CLI argument is correctly defined
    // that means if first character is '-' (only flags should start with '-') or value is last of all arguments (only host address can be last), function returns 0
    if (arg[0] == '-') {
        return 0; //not properly defined
    }

    return 1;  //defined

}

// function assings all specified ports to 'ports' array  
void setPorts(long ports[], char *arg) {                            

    int p = 0;
    switch(isRange(arg)) {

        case 0: // there are specific ports in arguments

            char *port = strtok(arg, ",");
            while (port != NULL) {                                          // loop gets ports from argument and puts them into an array as long int
                
                if(port[0] > 57 || port[0] < 48) {                          // ports can be specified only as numbers
                    fprintf(stderr, "Error: invalid port input\n");
                    exit(EXIT_FAILURE);
                }
                ports[p] = strtol(port, NULL, 10);
                port = strtok(NULL, ",");
                p++;

            }
            break;
        
        case 1: // theres a range of ports in arguments

            char *start = strtok(arg, "-");
            char *end = strtok(NULL, "-");
            
            long s = strtol(start, NULL, 10);
            long e = strtol(end, NULL, 10);

            for (long port = s; port <= e; port++) {
                ports[port-s] = port;                                   // putting only ports from given range to an array
    
            }
            break;
        
        case -1: // ports are specified wrong
            fprintf(stderr, "Error: invalid input\n");
            exit(EXIT_FAILURE);
        
        default:
            fprintf(stderr, "error: unknown\n");
    }
    return;
}
 


void processPacket(const unsigned char *packet, u_int8_t port_type, char *ip_str, int port_number, pcap_t *response) {
    // creating ip header of response packet
    struct ip *ip_header = (struct ip *)(packet+14);
    int ip_header_len = ip_header->ip_hl * 4;
 
     
    if (port_type == IPPROTO_UDP) { 
    
        // in case that response packet would be ICMP, we need to create header for this packet and check its code and type 
        struct icmphdr *icmp_header = (struct icmphdr*)(packet+ip_header_len+14);
        // now checking the response code
        if (icmp_header->code == ICMP_PORT_UNREACH && icmp_header->type == ICMP_PORT_UNREACH && ip_header->ip_p == IPPROTO_ICMP) {
            // ICMP packet with code 3 type 3 means port is closed
            printf("%s udp %u closed\n", ip_str, port_number);

        } else {
            // otherwise, port is open
            printf("%s %u udp open\n", ip_str, port_number);
        }
    }

    if (port_type == IPPROTO_TCP) {
        
        // if we are capturing TCP packet, TCP header is needed for better analysis
        struct tcphdr *tcp_header = (struct tcphdr*)(packet+ip_header_len+14);    

        if (tcp_header->rst) {
            // RST response means port is closed
            printf("%s %u tcp closed\n", ip_str, port_number);
        } else if (tcp_header->ack && tcp_header->syn) {
            // SYN-ACK response means port is open
            printf("%s %u tcp open\n", ip_str, port_number);
        } else {
            // no response captured means port is filtered
            printf("%s %u tcp filtered\n", ip_str, port_number);

        }   
    }
}


 
void sendPacket(int socket, char *packet_message, struct sockaddr *dest, int ip_version) {

    int send;
    if (ip_version == 1) {       
        int send = sendto(socket, packet_message, strlen(packet_message),0, dest, sizeof(struct sockaddr_in6)); 
    } else {
        int send = sendto(socket, packet_message, strlen(packet_message),0, dest, sizeof(struct sockaddr_in));
    }
    if (send < 0) {

        fprintf(stderr, "Sending packet failed %s.\n", packet_message);
        exit(EXIT_FAILURE);

    }
}

void captureResponse(pcap_t *response, char *ip_str, int port_number, int protocol) {

    struct bpf_program filter;
    struct pcap_pkthdr *header;
    const u_char *packet;
    char filter_exp[128];

    // Create the filter expression based on the protocol
    if (protocol == IPPROTO_UDP) {
        snprintf(filter_exp, sizeof(filter_exp), "icmp or udp");
    } else if (protocol == IPPROTO_TCP) {
        snprintf(filter_exp, sizeof(filter_exp), "tcp and src host %s and src port %d", ip_str, port_number);
    }

    // Compile the filter
    if (pcap_compile(response, &filter, filter_exp, 0, PCAP_NETMASK_UNKNOWN) != 0) {
        fprintf(stderr, "Filter compilation failed: %s\n", pcap_geterr(response));
        exit(EXIT_FAILURE);
    }

    // Apply the filter
    if (pcap_setfilter(response, &filter) != 0) {
        fprintf(stderr, "Setting the filter failed: %s.\n", pcap_geterr(response));
        exit(EXIT_FAILURE);
    }

    
    if (protocol == IPPROTO_UDP) {
        pcap_next_ex(response, &header, &packet);
    }
    // Capture packets in a loop until a valid response is found
    while (1) {
        int response_packet = pcap_next_ex(response, &header, &packet);

        if (response_packet == 1) {  // Valid packet received
            if (packet != NULL) {
                processPacket(packet, protocol, ip_str, port_number, response);
                break;
                // Stop after receiving one valid response
            }
        } 
        else if (response_packet == 0 && protocol == IPPROTO_TCP) {  // Timeout
            printf("%s %u tcp filtered\n", ip_str, port_number);
            break;
        } 
        else if (response_packet == -1) {  // Error while capturing packets
            fprintf(stderr, "Capturing packet failed: %s\n", pcap_geterr(response));
            break;
        }
    }

    pcap_freecode(&filter);  // Cleanup filter
}

 
