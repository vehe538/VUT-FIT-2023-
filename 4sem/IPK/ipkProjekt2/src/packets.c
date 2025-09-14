#include "packets.h"
#include "chat_clients.h"
#include "messages.h"



void sendPacket(int type, int server_port, char *ip_str, char *message, int socket) {

    if (type == IPPROTO_TCP) {
        //send packet
        int send_packet = send(socket, message, strlen(message), 0);
        if (send_packet < 0) {

            printf("ERROR: TCP packet sending failed.\n");
            exit(EXIT_FAILURE);
        }

    }


    if (type == IPPROTO_UDP) {

        //send packet
        int send_packet = sendto(socket, message, 60036, 0, (struct sockaddr *)&client_info.destination, sizeof(client_info.destination));
        if (send_packet < 0) {

            printf("ERROR: UDP packet sending failed.\n");
            exit(EXIT_FAILURE);
        }
        // after every sent packet, messageid will be incremented
        client_info.messageid++;
    }

}

void receivePacket(int type, messageType messagetype, int socket, char *response) {

    if (type == IPPROTO_TCP) {

        //receive packet
        int received = recv(socket, response, 60036, 0);
        if (received < 0) {

            printf("ERROR: TCP packet processing failed.\n");
            exit(EXIT_FAILURE);
        }
        response[received] = '\0';
    }

    if (type == IPPROTO_UDP) {

        char new[60036];
        int received;
        // set timeout
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = client_info.udp_timeout * 1000;  
        
        setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        // receive packet
        for (int retries = 0; retries < client_info.max_retransmissions + 1; retries++) {

            received = recv(socket, response, 60036, 0);
            // if received packet, function exits succesfully
            if (received >= 0) {
                return;
            }

            // if timeout occurs, another packet is sent
            if (errno == EWOULDBLOCK || errno == EAGAIN) {

                // this function should work for general purpose so based on message that didn't get response,
                // new identical message will be constructed and sent in the packet
                buildAuthUdp(client_info.messageid, client_info.username, client_info.secret, client_info.displayname, new);
                sendPacket(IPPROTO_UDP, client_info.server_port, client_info.ip_str, new, socket);                                

            } else {

                printf("ERROR: receiving UDP packet failed.\n");
                exit(EXIT_FAILURE);

            }

        }
        // if no reponse was received after mulitple tries
        printf("ERROR: no response received after retransmissions.\n");
        exit(EXIT_FAILURE);
        
    }


}