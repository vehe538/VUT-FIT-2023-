#include "messages.h"


void buildAuthTcp(char *username, char *secret, char *displayname, char *result) {

    snprintf(result, 60036, "AUTH %s AS %s USING %s\r\n", username, displayname, secret);

}

void buildJoinTcp(char *channelid, char *displayname, char *result) {

    snprintf(result, 60036, "JOIN %s AS %s\r\n", channelid, displayname);

}


void buildMsgTcp(char *displayname, char *chat_message, char *result) {

    snprintf(result, 60035, "MSG FROM %s IS %s\r\n", displayname, chat_message);

}

void buildActionStatusTcp(char *result) {


    char *reply = strtok(result, " "); // REPLY
    char *res = strtok(NULL, " ");     // OK or NOK
    char *is = strtok(NULL, " ");      // IS


    char *tmp = strtok(NULL, "\n");
    char content[strlen(tmp)+1];
    strncpy(content, tmp,  sizeof(content));

    if (strcmp(res, "OK") == 0 && strcmp(reply, "REPLY") == 0 && strcmp(is, "IS") == 0) {

        snprintf(result, 60036, "Action Success: %s\r\n", content);

    } else if (strcmp(res, "NOK") == 0) {

        snprintf(result, 60036, "Action Failure: %s\r\n", content);

    } else {

        snprintf(result, 60036, "ERROR: Received malformed message.\n");
    }

}

void buildActionStatusUdp(char *result, char *action) {

    uint8_t reply = (uint8_t)result[0]; // REPLY
    uint8_t res = (uint8_t)result[3];   // OK or NOK

    if (reply == REPLY) {

        if (res == 1) { // OK
            printf("Action Success: %s success.\r\n", action);
        } else {        // NOK
            printf("Action Failure: %s failure.\r\n", action);
        }

    }

}

void buildIncomingtMsgTcp(char *response, char *result) {
    
    char *save = response;
    //skip MSG
    char *msg = strtok_r(response, " ", &save);
    //skip FROM
    char *from = strtok_r(NULL, " ", &save);
    //get name of sender or Server
    char *sender = strtok_r(NULL, " ", &save);
    if (!sender) {
        sender = "unknown";
    }
    //skip IS
    char *is = strtok_r(NULL, " ", &save);
    char *messagecontent = strtok_r(NULL, "\r\n", &save);

    if (strcmp(msg, "MSG") == 0 && strcmp(from, "FROM") == 0 && strcmp(is, "IS") == 0) {
        snprintf(result, 60000, "%s: %s\r\n", sender, messagecontent);
    } else {
        snprintf(result, 60000, "ERROR: Received malformed message.\n");
    }
}

void buildByeTcp(char *result, char *displayname) {

    snprintf(result, 99, "BYE FROM %s\r\n", displayname);

}

void buildByeUdp(char *result, char *displayname) {

    uint16_t messageid = htons(client_info.messageid);
    uint8_t type = 0xFF; // BYE representation
    int index = 0;

    memcpy(result+index, &type, 1);
    index++;
    
    memcpy(result+index, &messageid, 2);
    index += 2;

    memcpy(result+index, displayname, strlen(displayname)+1);
    index += strlen(displayname)+1;

}

void buildErrTcp(char *result) {

    //ERR
    char *err = strtok(result, " ");
    //FROM
    char *from = strtok(NULL, " ");
    // get sender name
    char *tmp = strtok(NULL, " ");
    char sender[21];
    strncpy(sender, tmp, 20);
    //IS
    char *is = strtok(NULL, " ");
    char *errcontent = strtok(NULL, "\n");

    if (strcmp(err, "ERR") == 0 && strcmp(from, "FROM") == 0 && strcmp(is, "IS") == 0) {
        snprintf(result, 60036, "ERROR FROM %s: %s\n", sender, errcontent);
    } else {
        snprintf(result, 60036, "ERROR: Received malformed message.\n");
    }


}

void buildErrUdp(char *result, char *sender) {

    uint8_t type = 0xFE; // ERR representation
    uint16_t messageid = htons(client_info.messageid);
    char *errcontent = result + 4 + strlen(sender);
    int index = 0;

    // first byte is type of message
    memcpy(result+index, &type, 1);
    index++;

    // second will be messageID
    memcpy(result+index, &messageid, 2);
    index += 2;

    memcpy(result+index, sender, strlen(sender)+1);
    index += strlen(sender)+1;
    
    memcpy(result + index, errcontent, strlen(errcontent)+1);

}


void buildConfirm(char *result, uint16_t messageid) {

    uint8_t type = 0x00; // CONFIRM represenation
    int index = 0;

    // first byte is type of message
    memcpy(result+index, &type, 1);
    index++;

    // next two bytes are messageid number
    memcpy(result+index, &messageid, 2);
    index += 2;
}


void buildAuthUdp(uint16_t messageid, char *username, char *secret, char *displayname, char *result) {

    int len = 3 + strlen(username) + 1 + strlen(displayname) + 1 + strlen(secret) + 1;

    uint8_t type = 0x02; // AUTH representation
    int index = 0;

    // first item is type 0x02
    memcpy(result+index, &type, 1);
    index++;

    // second is messageid
    memcpy(result+index, &messageid, 2);
    index += 2;

    // add username 
    memcpy(result+index, username, strlen(username)+1);
    index += strlen(username)+1;

    // add displayname
    memcpy(result+index, displayname, strlen(displayname)+1);
    index += strlen(displayname)+1;

    // add secret
    memcpy(result+index, secret, strlen(secret)+1);
    index += strlen(secret)+1;

    
}