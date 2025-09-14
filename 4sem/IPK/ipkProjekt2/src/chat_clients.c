#include "chat_clients.h"
#include "utils.h"
#include "packets.h"
#include "messages.h"

// function handles graceful connection termination by SIGINT
void  connectionTerm(int sig)
{
    signal(sig, SIG_IGN);

    char bye_msg[100];
    if (client_info.protocol == IPPROTO_TCP) {
        buildByeTcp(bye_msg, client_info.displayname);
    } else {
        buildByeUdp(bye_msg, client_info.displayname);
    }

    sendPacket(client_info.protocol, client_info.server_port, client_info.ip_str, bye_msg, client_info.socket);
    endState();

}

// function simulates auth FSM state
void authState(int server_port, char *ip_str, int socket) {

    //here /auth command is expected
    char *username = malloc(21);
    char *secret = malloc(129);
    char *displayname = malloc(21);

    if (!username || !secret || !displayname) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // buffers for client input handling
    char input[6000];
    char tmp[60000];
    char message[60036];
    char response[60036];
    int protocol = client_info.protocol;
    while (1) {

        fd_set read;
        FD_ZERO(&read);
        // listen for both user input (commands or messages) but also for incoming messages from server
        FD_SET(STDIN_FILENO, &read);
        FD_SET(socket, &read);

        int max_fd = socket > STDIN_FILENO ? socket : STDIN_FILENO;

        // select allows me to monitor militple file descriptors - in my case stdin (for client input) and socket (for incoming messages)
        int activity = select(max_fd + 1, &read, NULL, NULL, NULL);
        if (activity < 0) {

            printf("ERROR: select() failed.\n");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO, &read)) {

            // first check for EOF (Ctrl+D)
            if (fgets(input, 6000, stdin) == NULL) {

                char bye_msg[100];
                if (client_info.protocol == IPPROTO_TCP) {
                    buildByeTcp(bye_msg, client_info.displayname);
                } else {
                    buildByeUdp(bye_msg, client_info.displayname);
                }
                
                sendPacket(client_info.protocol, client_info.server_port, client_info.ip_str, bye_msg, client_info.socket);
                endState();
            }
            strcpy(tmp, input);
        
            // separate client input command
            char *command = strtok(tmp, " ");
        
            if (strcmp(command, "/help\n") == 0) {
                    
                printCommandsHelp();
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(command, "/auth") == 0) {
                
                char *t_username = strtok(NULL, " ");
                char *t_secret = strtok(NULL, " ");
                char *t_displayname = strtok(NULL, "\n");
                
                // checking if client input consists of valid values and has allowed amount of components
                if (!isValid(t_username, "username") || !isValid(t_secret, "secret") || !isValid(t_displayname, "displayname")) {

                    printf("ERROR: /auth command values are invalid.\n");
                    printCommandsHelp();
                    authState(server_port, ip_str, socket);
                    break;
                }
        
                strncpy(username, t_username,21);
                strncpy(secret, t_secret,129);
                strncpy(displayname, t_displayname,21);

                // copy values also to global variables
                strncpy(client_info.displayname, t_displayname, sizeof(client_info.displayname));
                strncpy(client_info.username, t_username, sizeof(client_info.username));
                strncpy(client_info.secret, t_secret, sizeof(client_info.secret));
        
                // construct AUTH message components
                if (protocol == IPPROTO_TCP) {
                    buildAuthTcp(username, secret, displayname, message);
                }
                if (protocol == IPPROTO_UDP) {
                    buildAuthUdp(htons(client_info.messageid), username, secret, displayname, message);
                }
                // send packet
                sendPacket(protocol, server_port, ip_str, message, socket);
                // await reply
                receivePacket(protocol, CONFIRM, socket, response);

                // reference messageID
                uint16_t tmp;
                memcpy(&tmp, response + 1, 2);

                if ((uint8_t)response[0] == 0 || tmp == client_info.messageid - 1) { //if UDP packet is of type CONFIRM...
                    response[0] = '\0';
                    // ...another packet should be of type REPLY
                    receivePacket(protocol, REPLY, socket, response);

                    // check reference messageID
                    uint16_t tmp;
                    // 5th and 6th byte is reference messageID
                    memcpy(&tmp, response + 4, 2);
                    if (tmp != client_info.messageid - 1) {
                        printf("ERROR: Received wrong packet.\n");
                        exit(EXIT_FAILURE);
                    }

                    //send CONFIRM message to server
                    buildConfirm(message, tmp);
                    sendPacket(IPPROTO_UDP, server_port, ip_str, message, socket);
                    response[0] = '\0';

                    // construct reply message for stdout
                    buildActionStatusUdp(response, "Auth");
                } else {
                    // while waiting for reply, other messages can be received
                    while (response[0] != 'R') {

                        if (response[0] == 'E') {

                            buildErrTcp(response);
                            printf("%s", response);
                            response[0] = '\0';

                        } else if (response[0] == 'M') {

                            char result[1024];
                            buildIncomingtMsgTcp(response, result);
                            printf("%s", result);
                            response[0] = '\0';

                        }
                        receivePacket(protocol, REPLY, socket, response);

                    }
                    // construct reply message for stdout
                    buildActionStatusTcp(response);
                    // print formatted response to stdout and 'reset' the variable
                    printf("%s", response);

                    if (response[0] == 'E') {

                        // if received malformed message, ERR is sent to the server
                        sendPacket(IPPROTO_TCP, server_port, ip_str, "ERR FROM unknown IS Received malformed message\r\n", socket);
                        endState();

                    }
                    //if /auth was not successful, stay in this state
                    else if (response[7] == 'F') {
                        authState(server_port, ip_str, socket);
                        break;
                    }

                    response[0] = '\0';

                }
                openState(server_port, ip_str, socket, displayname);
                break;
                
            } else { // anything else sent by client in non-open state will print ERROR and help text
        
                free(username);
                free(displayname);
                free(secret);
        
                printf("ERROR: Invalid command, /auth command expected.\n");
                printCommandsHelp();

                authState(server_port, ip_str, socket);
                break;
            }
            

        }
        // handle possible ERR, BYE messages from the server
        if (FD_ISSET(socket, &read)) {

            receivePacket(protocol, MSG, socket, response);

            // now we need to separate scenarios when BYE and ERR are received (both udp and tcp)
            // I will do that by checking first letter (tcp) and first byte (udp) from received message 
            if (response[0] == 'B') {

                buildErrTcp(response);
                printf("%s", response);
                response[0] = '\0';

                endState();
                break;
            
            }
            else if (response[0] == 'E') {
                
                buildErrTcp(response);
                printf("%s", response);
                response[0] = '\0';

                endState();
                break;
            } 
            else if (response[0] == BYE) {
                
                // send CONFIRM message and go to end state
                char confirm_msg[3];
                buildConfirm(confirm_msg, client_info.messageid);
                sendPacket(IPPROTO_UDP, server_port, ip_str, confirm_msg, socket);
                endState();
                break;

            } else if (response[0] == ERR) {

                // send CONFIRM message and go to ens state
                char confirm_msg[3];
                buildConfirm(confirm_msg, client_info.messageid);
                sendPacket(IPPROTO_UDP, server_port, ip_str, confirm_msg, socket);
                endState();
                break;

            } else {

                // anything else will be malformed message
                printf("ERROR: Received malformed message.\n");
                char errbuff[60036];
                snprintf(errbuff, 60036, "ERR FROM %s IS Received malformed message.\r\n", client_info.displayname);
                sendPacket(IPPROTO_TCP, server_port, ip_str, errbuff, socket);
                endState();
            }

        }
    }
}

// function simulates open FSM state
void openState(int server_port, char *ip_str, int socket, char *displayname) {

    // buffers for client input handling
    char input[60000]; //size of max user input 
    char tmp[60000];
    char message[60036]; // maximum size of message constructed (MSG with longest displayname and longest message)
    char response[60036];// maxumum size of incoming message...
    char chat_message[60001];
    int protocol = client_info.protocol;

    while (1) {

        fd_set read;
        FD_ZERO(&read);
        // listen for both user input (commands or messages) but also for incoming messages from server
        FD_SET(STDIN_FILENO, &read);
        FD_SET(socket, &read);

        int max_fd = socket > STDIN_FILENO ? socket : STDIN_FILENO;

        int activity = select(max_fd + 1, &read, NULL, NULL, NULL);
        if (activity < 0) {
            printf("ERROR: Select failed.\n");
            exit(EXIT_FAILURE);
        }

        // handle user input
        if (FD_ISSET(STDIN_FILENO, &read)) {

            // first check if Ctrl+D was pressed
            if (fgets(input, 60000, stdin) == NULL) {
                
                char bye_msg[100];
                if (client_info.protocol == IPPROTO_TCP) {
                    buildByeTcp(bye_msg, client_info.displayname);
                } else {
                    buildByeUdp(bye_msg, client_info.displayname);
                }
            
                
                sendPacket(client_info.protocol, client_info.server_port, client_info.ip_str, bye_msg, client_info.socket);
                endState();
            }
            strcpy(tmp, input);
            char *command = strtok(tmp, " ");
            
            if (strcmp(command, "/join") == 0) {

                char *t_channelid = strtok(NULL, "\n");

                // check if input is valid
                if (!isValid(t_channelid, "channelid") || !t_channelid) {
                    printf("ERROR: client input \'%s\' is invalid or too long.\n", t_channelid);
                    printCommandsHelp();
                    continue;
                }

                // transition to join state
                joinState(server_port, ip_str, socket, displayname, t_channelid);
                break;

            } else if (strcmp(command, "/rename") == 0) {

                // get new displayname from input
                char *new_displayname = strtok(NULL, "\n");                

                if (!isValid(new_displayname, "displayname")) {
                    printf("ERROR: client \'%s\' input is invalid or too long.\n", new_displayname);
                    exit(EXIT_FAILURE);
                }
                
                strncpy(displayname, new_displayname, 20);
                strncpy(client_info.displayname, new_displayname, sizeof(client_info.displayname));

            } else if (strcmp(command, "/help\n") == 0) {

                printCommandsHelp();
                exit(EXIT_SUCCESS);

            } else if (strcmp(command, "/auth") == 0) {

                printf("ERROR: /auth command was already used.\n ");

            } 
            else {

                // copy input to variable
                strncpy(chat_message, input, 60000);

                chat_message[strcspn(chat_message, "\n")] = '\0';
                // use the variable to construct a MSG message
                buildMsgTcp(displayname, chat_message, message);
                // send packet with new message
                sendPacket(protocol, server_port, ip_str, message, socket);

            }
        }

        // handle incoming server message
        if (FD_ISSET(socket, &read)) {
            receivePacket(protocol, MSG, socket, response);
            
            // now we need to separate scenarios when MSG and ERR are received
            // I will do that by checking first letter from received message
            char tmp[60021];
            strncpy(tmp, response, sizeof(tmp));

            // 'res' is used to parse response messages containing more packets ('abc\r\nabc\r\n')
            char *res = strtok(tmp, "\r\n");
            while (res != NULL) {

                if (res[0] == 'M') {

                    char result[60021];
                    char response_copy[sizeof(response)];
                    strncpy(response_copy, res, sizeof(response_copy));
                    //printf("%s\n", response_copy);
                    
                    buildIncomingtMsgTcp(response_copy, result);
                    printf("%s", result);
                    result[0] = '\0';    
                
                } else if (res[0] == 'E') {
                        
                    // if received message is an error message, corresponding output will be printed
                    buildErrTcp(response);
                    printf("%s", response);
                    response[0] = '\0';
                    endState();
        
                } else if (res[0] == 'B') {
                        
                    // if received message is BYE, connection will terminate
                    buildErrTcp(response);
                    printf("%s", response);
                    response[0] = '\0';
                    endState();
        
                } else {

                    // anythin else is considered a malformed message
                    printf("ERROR: Received malformed message.\n");
                    char errbuff[60036];
                    snprintf(errbuff, 60036, "ERR FROM %s IS Received malformed message.\r\n", client_info.displayname);
                    sendPacket(IPPROTO_TCP, server_port, ip_str, errbuff, socket);
                    endState();

                }
                res = strtok(NULL, "\r\n");
            }
        }
    }
}


// function simulates join FSM state
void joinState(int server_port, char *ip_str, int socket, char *displayname, char *t_channelid) {

    char message[100];
    char response[60035];
    int protocol = client_info.protocol;


    char *channelid = malloc(21);
    if (!channelid) {

        printf("ERROR: memory allocation failed.\n");
        exit(EXIT_FAILURE);

    }



    strncpy(channelid, t_channelid,20);
    // construct JOIN message components
    buildJoinTcp(channelid, displayname, message);
    // send packet
    sendPacket(protocol, server_port, ip_str, message, socket);  
    //await msg
    receivePacket(protocol, MSG, socket, response);

    while (1) {
        
        if (response[0] == 'M') {

            //construct MSG for stdout
            char result[60036];
            buildIncomingtMsgTcp(response, result);
            printf("%s", result);
            response[0] = '\0';
            receivePacket(protocol, REPLY, socket, response);

        } else if (response[0] == 'R') {

            // construct action message for stdout
            buildActionStatusTcp(response);            
            printf("%s", response);

            // if reply message is invalid
            if (response[0] == 'E') {

                char errbuff[60036];
                snprintf(errbuff, 60036, "ERR FROM %s IS Received malformed message.\r\n", client_info.displayname);
                sendPacket(IPPROTO_TCP, server_port, ip_str, errbuff, socket);
                endState();

            }
            
            response[0] = '\0';

            openState(server_port, ip_str, socket, displayname);
            free(channelid);
            break;
        } else if (response[0] == 'E') {
            
            // if received message is an error message, corresponding output will be printed
            buildErrTcp(response);
            printf("%s", response);

            response[0] = '\0';
            
            endState();
            break;

        } else {
        
            //anything else will be malformed message
            printf("ERROR: Received malformed message");
            char errbuff[60036];
            snprintf(errbuff, 60036, "ERR FROM %s IS Received malformed message.\r\n", client_info.displayname);
            sendPacket(IPPROTO_TCP, server_port, ip_str, errbuff, socket);
            endState();

        }

    }

}

// function simulates end FSM state
void endState() {

    close(client_info.socket);
    exit(EXIT_SUCCESS);

}

// this function is called from main, also simulates start FSM state
void clientTcp(int server_port, char *ip_str) {

    //first we need to create TCP socket
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {

        printf("ERROR: TCP socket creation failed.\n");
        exit(EXIT_FAILURE);

    }

    // set up destination address
    struct sockaddr_in destination;
    
    memset(&destination, 0, sizeof(destination));
    destination.sin_family = AF_INET;
    destination.sin_port = htons(server_port);
    inet_pton(AF_INET, ip_str, &destination.sin_addr);


    // connect to the destination address
    int connection = connect(tcp_socket, (struct sockaddr *)&destination, sizeof(struct sockaddr_in));
    if (connection < 0) {

        printf("ERROR: connecting to socket failed. %d\n", errno);
        exit(EXIT_FAILURE);

    }
    // put new defined values into global structure 
    client_info.socket = tcp_socket;
    client_info.protocol = IPPROTO_TCP;

    // now we can move to auth state according to FSM
    authState(server_port, ip_str, tcp_socket);

}



void clientUdp(int server_port, char *ip_str) {

    //first we need to create UDP socket
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {

        printf("ERROR: UDP socket creation failed: %d\n", errno);
        exit(EXIT_FAILURE);

    }

    
    // set up destination address
    struct sockaddr_in destination;
    
    memset(&destination, 0, sizeof(destination));
    destination.sin_family = AF_INET;
    destination.sin_port = htons(server_port);
    destination.sin_addr.s_addr = INADDR_ANY;


    // put new defined values into global structure 
    strncpy(client_info.ip_str, ip_str, sizeof(client_info.ip_str));
    client_info.server_port = server_port;
    client_info.socket = udp_socket;
    client_info.protocol = IPPROTO_UDP;
    client_info.destination = destination;
    client_info.messageid = 0;

    // now we can move to auth state according to FSM
    authState(server_port, ip_str, udp_socket);

}