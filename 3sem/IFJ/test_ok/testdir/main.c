/**
 * @file main.c
 * @brief Main function of compiler. 
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
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "tokeniser.h"
#include "expr_stack.h"
#include "symtable.h"
#include "error.h"



int main(){
    
    FILE *file = stdin;    
    start(file);
    return 0;
}