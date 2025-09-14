#include <arpa/inet.h>
#include <signal.h>
#include <stdint.h>


#ifndef SHARED_H
#define SHARED_H

typedef struct {

    int socket;
    char displayname[21];
    char username[21];
    char secret[128];
    char ip_str[INET_ADDRSTRLEN];
    int protocol;
    uint16_t messageid; // used only with UDP
    uint16_t server_port;
    uint8_t max_retransmissions;
    uint16_t udp_timeout;
    struct sockaddr_in destination;


} appStatus;

typedef enum uint8_t {

    CONFIRM = 0x00,
    REPLY = 0x01,
    NREPLY = 0xF1,
    AUTH = 0x02,
    JOIN = 0x03,
    MSG = 0x04,
    ERR = 0xFE,
    BYE = 0xFF

} messageType;

extern appStatus client_info;

#endif

void authState(int server_port, char *ip_str, int tcp_socket);
void openState(int server_port, char *ip_str, int tcp_socket, char *displayname);
void joinState(int server_port, char *ip_str, int tcp_socket, char *displayname, char* t_channelid);
void connectionTerm(int sig);
void clientTcp(int server_port, char *ip_str);
void clientUdp(int server_port, char *ip_str);
void endState();