#include "scanners.h"
#include "utils.h"

// -------------------------------------------------UDP SCANNER---------------------------------------------------//
void udpScan(char *interface, long udp_ports[65535], char *dst_address, char errbuf[PCAP_ERRBUF_SIZE], pcap_t *response, int timeout){
   
    if (udp_ports[0] != -1) {
        
        for (int port = 0; udp_ports[port] != 0; port++) {
            // open interface for capture 
            response = pcap_open_live(interface, BUFSIZ, 1, timeout, errbuf);
            if (response == NULL) {
                fprintf(stderr, "Opening interface \"%s\" for capture failed: %s\n", interface, errbuf);
                exit(EXIT_FAILURE);
            }

            // Resolve hostname if needed
            struct addrinfo hints, *res, *r;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;   // Supports both IPv4 & IPv6
            hints.ai_socktype = SOCK_STREAM;

            int status = getaddrinfo(dst_address, NULL, &hints, &res);
            if (status != 0) {    
                fprintf(stderr, "Getaddrinfo failed: %s\n", gai_strerror(status));
                exit(EXIT_FAILURE);
            }

            char ip_str[INET6_ADDRSTRLEN];

            int is_ipv6 = 0;
            int socket_type;
            struct sockaddr_in *dest = (struct sockaddr_in *)res->ai_addr;
            // looping through addrinfo to be able to send also to ipv6
            for (r = res; r != NULL; r = r->ai_next) {
                if (r->ai_family == AF_INET6) {
                    is_ipv6 = 1;
                }
            }

            // we need to specify what type of socket will be created based on destination address type
            if (is_ipv6) {

                socket_type = AF_INET6;
                struct sockaddr_in6 *dest = (struct sockaddr_in6 *)res->ai_addr;
                dest->sin6_port = htons(udp_ports[port]);
                inet_ntop(AF_INET6, &(dest->sin6_addr), ip_str, INET6_ADDRSTRLEN);

            } else {

                socket_type = AF_INET;
                dest->sin_port = htons(udp_ports[port]);            
                inet_ntop(AF_INET, &(dest->sin_addr), ip_str, INET_ADDRSTRLEN);                

            }

            //  create socket 
            int udp_socket_new = socket(socket_type, SOCK_DGRAM, 0);
            if (udp_socket_new < 0) {
                fprintf(stderr, "Socket creation failed.\n");
                exit(EXIT_FAILURE);
            }

            // bind socket to user-specified interface
            int bind_interface = setsockopt(udp_socket_new, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface));
            if (bind_interface < 0) {

                fprintf(stderr, "Binding interface failed.\n");
                exit(EXIT_FAILURE);

            }
            // send udp packet and capture response
            sendPacket(udp_socket_new, "udp message", (struct sockaddr *)dest, IPPROTO_UDP);
            captureResponse(response, ip_str, udp_ports[port], IPPROTO_UDP);
            pcap_close(response);            
        }

    }
}

// -------------------------------------------------TCP SCANNER---------------------------------------------------//
void tcpScan(char *interface, long tcp_ports[65535], char *dst_address, char errbuf[PCAP_ERRBUF_SIZE], pcap_t *response, int timeout) {

    if (tcp_ports[0] != -1) {

        for (int port = 0; tcp_ports[port] != 0; port++) {
            struct timeval t;
            // open interface for capture 
            response = pcap_open_live(interface, BUFSIZ, 1, timeout, errbuf);
            if (response == NULL) {
                fprintf(stderr, "Opening interface \"%s\" for capture failed: %s\n", interface, errbuf);
                exit(EXIT_FAILURE);
            }

            // Resolve hostname if needed
            struct addrinfo hints, *res, *r;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;   // Supports both IPv4 & IPv6
            hints.ai_socktype = SOCK_STREAM;

            int status = getaddrinfo(dst_address, NULL, &hints, &res);
            if (status != 0) {    
                fprintf(stderr, "Getaddrinfo failed: %s\n", gai_strerror(status));
                exit(EXIT_FAILURE);
            }

            char ip_str[INET6_ADDRSTRLEN];

            int is_ipv6 = 0;
            int socket_type;
            
            struct sockaddr_in *dest = (struct sockaddr_in *)res->ai_addr;
            // looping through addrinfo to be able to send also to ipv6
            for (r = res; r != NULL; r = r->ai_next) {
                if (r->ai_family == AF_INET6) {
                    is_ipv6 = 1;
                }
            }
            // we need to specify what type of socket will be created based on destination address type
            socket_type = AF_INET;
            dest->sin_port = htons(tcp_ports[port]);
            inet_ntop(AF_INET, &(dest->sin_addr), ip_str, INET_ADDRSTRLEN);     

            // create socket for TCP packets
            int tcp_socket_new = socket(socket_type, SOCK_STREAM, 0);
            if (tcp_socket_new < 0) {
                fprintf(stderr, "Socket creation failed.\n");
                exit(EXIT_FAILURE);
            }

            // bind socket to user-specified interface
            int bind_interface = setsockopt(tcp_socket_new, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface));
            if (bind_interface < 0) {
                fprintf(stderr, "Binding interface failed.\n");
                exit(EXIT_FAILURE);
            }
            // Set socket to non-blocking so I can capture time taken to establish connection
            fcntl(tcp_socket_new, F_SETFL, O_NONBLOCK);
            // defined timeval structures for measuring time taken
            struct timeval start, end;
            gettimeofday(&start, NULL);  // start time
            
            // attempt to establish connection
            int connection = connect(tcp_socket_new, (struct sockaddr *)dest, sizeof(struct sockaddr_in));
            if (connection == 0) {

                // connection established immediately
                fcntl(tcp_socket_new, F_SETFL, fcntl(tcp_socket_new, F_GETFL) & ~O_NONBLOCK); // set socket back to blocking mode
                sendPacket(tcp_socket_new, "tcp message", (struct sockaddr *)dest, 0);
                captureResponse(response, ip_str, tcp_ports[port], IPPROTO_TCP);

            } else if (errno == EINPROGRESS) {

                // if connection wasn't made instantly I need to capture time taken
                fd_set write_fds;
                FD_ZERO(&write_fds);
                FD_SET(tcp_socket_new, &write_fds);

                struct timeval to;
                to.tv_sec = timeout / 1000; // converting timeout set by user to seconds
                to.tv_usec = 0;

                // select waits for connection to happen relevant to 'to' specified timeframe
                int result = select(tcp_socket_new + 1, NULL, &write_fds, NULL, &to);

                if (result > 0) {
                    // now I check if the socket is connected with getsockopt
                    int is_connected;
                    socklen_t len = sizeof(is_connected);
                    
                    // getsockopt gets final connection result to is_connected var
                    getsockopt(tcp_socket_new, SOL_SOCKET, SO_ERROR, &is_connected, &len);

                    if (is_connected == 0) {
                        // connection is succesful, timer can be stopped
                        gettimeofday(&end, NULL);
                        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
                        //printf("Connection established after %.3f seconds \n", elapsed);

                        // we can now send packet and verify response
                        sendPacket(tcp_socket_new, "tcp message", (struct sockaddr *)dest, 0);
                        captureResponse(response, ip_str, tcp_ports[port], IPPROTO_TCP);

                    } else {
                        // connection was unsuccesful - port is closed
                        captureResponse(response, ip_str, tcp_ports[port], IPPROTO_TCP);

                    }
                } else if (result == 0) {
                    // if select returns 0, this means that the time specified in 'to' and with 'timeout' was shorter than it took to establish connection
                    printf("%s %lu tcp filtered\n", ip_str, tcp_ports[port]);

                } else {
                    fprintf(stderr, "Select() function failed.\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("sending ipv6 TCP packet failed: %d\n", errno);

            }
            pcap_close(response);
        }   
    }


}

