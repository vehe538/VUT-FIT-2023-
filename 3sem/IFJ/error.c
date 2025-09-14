/**
* @file error.c
 * @brief Implementation of pushError.
 *
 *
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */

#include <stdio.h>
#include <stdlib.h>

void pushError(char *error, int lvl) {
    fprintf(stderr, "Error: %s\n", error);
    exit(lvl);
}
