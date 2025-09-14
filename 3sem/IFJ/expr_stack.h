/**
 * @file expr_stack.h
 * @brief Header for stack.
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */



#include <stdbool.h>
#include "tokeniser.h"


#ifndef STACK_H
#define STACK_H


typedef struct exElem {

	Token *token;
	struct exElem *next;

} exElem;


typedef struct exStack {

	exElem *top;
	int lenght;

} exStack;


void exInit(exStack *stack);
void exPop( exStack *stack);
void exPush( exStack *stack, Token *token);
exElem *exTop( exStack *stack);
bool exEmpty( exStack *stack);
void exDispose ( exStack *stack );


#endif