#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "chat_clients.h"


void buildAuthTcp(char *username, char *secret, char *displayname, char *result);
void buildAuthUdp(uint16_t messageid,char *username, char *secret, char *displayname, char *result);

void buildJoinTcp(char *channelid, char *displayname, char *result);

void buildMsgTcp(char *displayname, char *chat_message, char *result);

void buildByeTcp(char *result, char *displayname);
void buildByeUdp(char *result, char *displayname);

void buildErrTcp(char *result);
void buildErrUdp(char *result, char *sender);

void buildActionStatusTcp(char *result);
void buildActionStatusUdp(char *result, char *action);

void buildIncomingtMsgTcp(char *response, char *result);

void buildConfirm(char *result, uint16_t messageid);