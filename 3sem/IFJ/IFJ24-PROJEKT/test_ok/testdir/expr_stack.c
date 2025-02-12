/**
 * @file expr_stack.c
 * @brief stack source file.
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tokeniser.h"
#include "expr_stack.h"


void exInit ( exStack *stack ) { 

	stack->top = NULL;
	stack->lenght = 0;

} 


void exPop ( exStack *stack ) {

	if (stack->top != NULL) {

		exElem *firstElement = stack->top;
		
		stack->top = firstElement->next;
		free(firstElement);

		stack->lenght--;

	}

}


void exPush ( exStack *stack, Token *token ) {

	exElem *newElement = (exElem *)malloc(sizeof(*newElement));

	if (newElement == NULL) {
		return;
	}

	newElement->token = token;
	newElement->next = stack->top;
	stack->top = newElement;

	stack->lenght++;

}

exElem *exTop ( exStack *stack) {

	return stack->top;

}
 

bool exEmpty ( exStack *stack ) {		

	return stack->top->token->type == TOKEN_STACK_BOTTOM;

}
 

void exDispose ( exStack *stack ) {

	while (exTop(stack) != NULL) {

		exPop(stack);

	}

	stack->top = NULL;
	stack->lenght = 0;


}