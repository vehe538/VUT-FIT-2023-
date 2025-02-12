/**
 * @file prec_table.h
 * @brief Header for prec_table.c 
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */


#include "tokeniser.h"


#ifndef PREC_TABLE_H
#define PREC_TABLE_H


typedef enum table_output {

	shift,
	reduction,
	equals,
	error

} tOut;

typedef struct tableItem {

	Token *input_token; 	  	    
	tOut *output_order[14]; 	

} tItem;

typedef tItem *tTable[14];
typedef enum operators {plus, minus, multiply, divide, openbr, closebr, less, more, lessequal, moreequal, equal, notequal, dollar} tOp; 


void set_table_value(tItem *token, int operator);
void table_insert(tTable **table, tItem* token, int operator);
void prec_table_init(tTable **table);


#endif