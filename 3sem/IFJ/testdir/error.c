/**
* @file error.c
 * @brief Simple error handling function.
 *
 *
 * Authors:
 * - Juraj Mesík (xmesikj00), 2025
 */


#include <stdio.h>
#include <stdlib.h>

void printError(int num, char *message) {

    fprintf(stderr, "Error %d: %s\n", num, message);
    exit(num);

}