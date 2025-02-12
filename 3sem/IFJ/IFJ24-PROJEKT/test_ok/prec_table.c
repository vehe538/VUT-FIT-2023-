/**
 * @file prec_table.c
 * @brief Functions for precedence table. 
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */


#include <stdlib.h>
#include <stdio.h>

#include "error.h"
#include "tokeniser.h"
#include "prec_table.h"

void prec_table_init (tTable **table) {

	tOp operator;
	for (int i = 0; i < 14; i++) {

		operator = i;
		tItem *exprToken = malloc(sizeof(tItem));
		if (!exprToken) {
			pushError("malloc fail.\n", 99);
		}
		
		for (int j = 0; j < 14; j++) {
			exprToken->output_order[j] = malloc(sizeof(tOut));	/* this cycle allocates space for 14 tOut values so every operator on input has a rule according to top of the stack */
			if (!exprToken->output_order) {
				pushError("malloc fail.\n", 99);
			}
		}

		set_table_value(exprToken, operator);					/* according to input operator, this function sets its table values */
		table_insert(table, exprToken, operator);				/* inserts an operator to precedence table */
	
	}
}

void table_insert(tTable **table, tItem* token, int operator) {  /* this function inserts operator to the parsing table */

    (**table)[operator] = token;

}

void set_table_value(tItem *token, int operator) {              /* this function is an implementation of parsing table */

    switch (operator)
    {
    
    case 0: // input is '+'
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = shift;
        *token->output_order[7] = shift;
        *token->output_order[8] = shift;
        *token->output_order[9] = shift;
        *token->output_order[10] = shift;
        *token->output_order[11] = shift;
        *token->output_order[12] = shift;        
        *token->output_order[13] = reduction;
        break;

    case 1: // input is '-'
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = shift;
        *token->output_order[7] = shift;
        *token->output_order[8] = shift;
        *token->output_order[9] = shift;
        *token->output_order[10] = shift;
        *token->output_order[11] = shift;
        *token->output_order[12] = shift;
        *token->output_order[13] = reduction;
        break;

    case 2: // input is '*'
        *token->output_order[0] = shift;
        *token->output_order[1] = shift;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = shift;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = reduction;
        *token->output_order[13] = reduction;
        break;

    case 3: // input is '/'
        *token->output_order[0] = shift;
        *token->output_order[1] = shift;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = shift;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = reduction;
        *token->output_order[13] = reduction;
        break;

    case 4: // input is '('
        *token->output_order[0] = shift;
        *token->output_order[1] = shift;
        *token->output_order[2] = shift;
        *token->output_order[3] = shift;
        *token->output_order[4] = shift;
        *token->output_order[5] = error;
        *token->output_order[6] = shift;
        *token->output_order[7] = shift;
        *token->output_order[8] = shift;
        *token->output_order[9] = shift;
        *token->output_order[10] = shift;
        *token->output_order[11] = shift;
        *token->output_order[12] = shift;
        *token->output_order[13] = error;
        break;

    case 5: // input is ')'
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = equals;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = error;
        *token->output_order[13] = reduction;
        break;

    case 6: // input is '<'
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = shift;
        *token->output_order[13] = reduction;
        break;

    case 7: // input is '>'
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = shift;
        *token->output_order[13] = reduction;
        break;

    case 8: // input is '<='
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = shift;
        *token->output_order[13] = reduction;
        break;

    case 9: // input is '>='
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = shift;
        *token->output_order[13] = reduction;
        break;

    case 10: // input is '=='
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = shift;
        *token->output_order[13] = reduction;
        break;

    case 11: // input is '!='
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = shift;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = shift;
        *token->output_order[13] = reduction;
        break;

    case 12: // input is '$'
        *token->output_order[0] = reduction;
        *token->output_order[1] = reduction;
        *token->output_order[2] = reduction;
        *token->output_order[3] = reduction;
        *token->output_order[4] = error;
        *token->output_order[5] = reduction;
        *token->output_order[6] = reduction;
        *token->output_order[7] = reduction;
        *token->output_order[8] = reduction;
        *token->output_order[9] = reduction;
        *token->output_order[10] = reduction;
        *token->output_order[11] = reduction;
        *token->output_order[12] = error;
        *token->output_order[13] = reduction;
        break;

    case 13: // input is variable 'i'
        *token->output_order[0] = shift;
        *token->output_order[1] = shift;
        *token->output_order[2] = shift;
        *token->output_order[3] = shift;
        *token->output_order[4] = shift;
        *token->output_order[5] = error;
        *token->output_order[6] = shift;
        *token->output_order[7] = shift;
        *token->output_order[8] = shift;
        *token->output_order[9] = shift;
        *token->output_order[10] = shift;
        *token->output_order[11] = shift;
        *token->output_order[12] = shift;
        *token->output_order[13] = error;
        break;

    default:
        break;
    }
}