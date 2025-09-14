#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <regex.h>

void printHelp();
void printCommandsHelp();
int isValid(char *string, char *component);
