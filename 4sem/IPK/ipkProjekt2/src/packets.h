#include <errno.h>
#include "chat_clients.h"


void sendPacket(int type, int server_port, char *ip_str, char *message, int socket);
void receivePacket(int type, messageType messagetype, int socket, char *response);
