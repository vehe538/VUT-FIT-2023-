/**
 * @file psa.h
 * @brief Header for expression parser psa.c 
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */


#include <string.h>
#include <stdbool.h>
#include "tokeniser.h"
#include "prec_table.h"
#include "parser.h"
#include "expr_stack.h"
#include "symtable.h"

#ifndef PSA_H
#define PSA_H


/* this array specifies allowed token types in expression */
extern token_type allowed_tokens[18];
extern token_type logical_exp_tokens[6];



int parse_expr(tParser **current, ht_item_t *variable);
int get_top (exStack* stack);
int get_input (exStack* stack, Token *current);
void loadToken (tParser **current, exStack *stack, int **brackets, int **num_of_logical, int *top, int *input, int *end_if, int *has_null, int *has_float, int *has_string, int *has_int, Token **logical, ht_item_t **variable);


#endif