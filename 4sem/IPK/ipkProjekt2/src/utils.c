
#include "utils.h"

void printHelp() {

    printf("USAGE:\n");
        
    printf("./ipk25chat-client [ARGUMENTS]\n");
    printf("----------------------------------------\n");
    printf("POSSIBLE ARGUMENTS:\n");
    printf("-t <tcp/udp>                  -- Transport protocol used for connection\n");
    printf("-s <IP address or hostname>   -- Server IP or hostname\n");
    printf("-p (optional) <uint16 number> -- Server port\n");
    printf("-d (optional) <uint16 number> -- UDP confirmation timeout (in milliseconds)\n");
    printf("-r (optional) <uint8 number>  -- Maximum number of UDP retransmissions\n");
    printf("-h (solo argument)            -- Prints program help output and exits\n");
}


void printCommandsHelp() {

    printf("AVAILABLE COMMANDS:\n");
    printf("--------------------------------------------------------------------------------------------------- \n");
    printf("\\help                                   -- prints command manual\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("\\auth {Username} {Secret} {DisplayName} -- sends AUTH message with the data provided from the\n");
    printf("                                           command to the server and locally sets DisplayName value.\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("\\join {ChannelID}                       -- sends JOIN message with channel name from the\n");
    printf("                                           command to the server.\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("\\rename {DisplayName}                   -- locally changes the display name of the user to be\n");
    printf("                                           sent with new messages/selected commands\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("VALUES' LIMITS: (based on POSIX regex rules)\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("Username    -- [a-zA-Z0-9-]{1,20}\n");
    printf("ChannelID   -- [a-zA-Z0-9-]{1,20}\n");
    printf("Secret      -- [a-zA-Z0-9-]{1,128}\n");
    printf("DisplayName -- [\\x21-\\x7E]{1,20}\n");
    printf("-----------------------------------------------------------------------------------------------------\n");


}



int isValid(char *string, char *component) {

    regex_t regex;
    int result;
    const char *pattern;

    if (strcmp(component, "username") == 0 || strcmp(component, "channelid") == 0) {

        pattern = "^[\\.a-zA-Z0-9-]{1,20}$";
    
    } else if (strcmp(component, "secret") == 0) {

        pattern = "^[a-zA-Z0-9-]{1,128}$";

    } else if (strcmp(component, "displayname") == 0) {

        pattern = "^[\x21-\x7E]{1,20}";

    }


    regcomp(&regex, pattern, REG_EXTENDED);
    result = regexec(&regex, string, 0, NULL, 0);
    regfree(&regex);


    if (!result) {
        return 1;  // Match
    } else {
        return 0;  // No match
    }

}

