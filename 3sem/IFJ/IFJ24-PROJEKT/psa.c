/**
 * @file psa.c
 * @brief Expression parser.
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
#include <stdbool.h>	
#include <string.h>


#include "tokeniser.h"
#include "error.h"
#include "parser.h"
#include "symtable.h"
#include "psa.h"
#include "codegen.h"


token_type allowed_tokens[18] = {
	
        TOKEN_ADD,
        TOKEN_SUB,
		TOKEN_MUL,
		TOKEN_DIV,
		TOKEN_LEFT_PAREN,
        TOKEN_RIGHT_PAREN,
		TOKEN_LESS,
		TOKEN_GREATER,
		TOKEN_LESS_EQUAL,
		TOKEN_GREATER_EQUAL,
		TOKEN_EQUAL,
        TOKEN_NOT_EQUAL,
        TOKEN_STACK_BOTTOM,
        TOKEN_IDENTIFIER,
        TOKEN_INT_LITERAL,
        TOKEN_STRING_LITERAL,
		TOKEN_FLOAT_LITERAL,
		TOKEN_KEYWORD_NULL

};

token_type logical_exp_tokens[6] = {

    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,

};

int get_top (exStack* stack) {

	for (int i = 0; i < 18; i++) {
		if (exTop(stack)->token->type == allowed_tokens[i]) {
			return i;
		}
	}
	return -1;
}


int get_input (exStack* stack, Token *current) {

	for (int i = 0; i < 18; i++) {
		if (current->type == allowed_tokens[i]) {
			return i;
		}
	}
	return -1;
}

/* this function processes tokens and set variables' values accordingly */
void loadToken (tParser **current, exStack *stack, int **brackets, int **num_of_logical, int *top, int *input, int *end_if, int *has_null, int *has_float, int *has_string, int *has_int, Token **logical, ht_item_t **variable) {
	
	for (int i = 0; i < 17; i++) {
		
		if ((*current)->current_token->type == allowed_tokens[i]) {

			*input = get_input(stack, (*current)->current_token);
			*top = get_top(stack);
			break;
		} else {
			*input = -1;
			*top = get_top(stack);
		}
	}

	
	/* this loop will track, number of logical operators in expression */
	/* if expression is logical, the operator will be stored in 'loigcal' helper token variable */
	for (int l = 0; l < 6; l++) {

		if((*current)->current_token->type == logical_exp_tokens[l]) {
			(*logical) = (*current)->current_token;
			(**num_of_logical)++;
		}
	}
	
	/* brackets logic */
	if ((*current)->current_token->type == TOKEN_LEFT_PAREN) {
		(**brackets)++;
	}
	if ((*current)->current_token->type == TOKEN_RIGHT_PAREN && ((*current)->next_token->type != TOKEN_LEFT_BRACE && (*current)->next_token->type != TOKEN_PIPE)) {
		(**brackets)--;
	}
	if ((*current)->current_token->type == TOKEN_RIGHT_PAREN && ((*current)->next_token->type == TOKEN_LEFT_BRACE || (*current)->next_token->type == TOKEN_PIPE)) {
			*end_if = 1;
	}


	/* setting helper variables based on input symbol */
	switch (*input) {
		case 17:	/* null token on input */
			*has_null = 1;
			break;
		case 16:	// float literal on input
			*has_float = strtod((*current)->current_token->param, NULL);
			break;
		case 15: // string literal on input
			pushError("Cannot have string in expression.\n", 7);
		case 14: // integer literal on input
			*has_int = strtol((*current)->current_token->param, NULL, 10);
			break;
		case 13: //token identifier - variable on input - need to search for it in symtable

			ht_item_t *tmp = searchHash((*current)->table, (*current)->current_token->param);
			if (tmp == NULL) {
				pushError("Using undefined variable.\n", 3);	
			}
			if (tmp->isnullable == 1) {
				*has_null = 1;
				break;
			}


			switch(tmp->type) {
				case TOKEN_KEYWORD_U8:
					*has_string = 1;
					break;
				case TOKEN_KEYWORD_F64:
					*has_float = 1;
					break;
				case TOKEN_KEYWORD_I32:
					*has_int = 1;
					break;
				default:
					break;
			}
			

	}
	

	if (*input >= 13) {
		*input = 13;
	}
	if (*top >= 13) {
		*top = 13;
	}	

}

/* main function for parsing expressions */

// 0 - int result
// 1 - float result
// 2 - string result
// 3 - true/false
// 4 - null result
int parse_expr(tParser **current, ht_item_t *variable) {

	/* creates precedence table */
	tTable *table = malloc(sizeof(tTable));
	if (!table) {
		pushError("malloc fail\n", 99);
	}
	prec_table_init(&table);
	
	/* creation and init of stack for tokens */
	exStack *stack = malloc(sizeof(exStack));
	if (!stack) {
		pushError("stack allocation fail.\n", 99);
	}

	Token *bottom_stack = createToken(TOKEN_STACK_BOTTOM, "$", 1);
	exInit(stack);
	exPush(stack, bottom_stack);	


	//FILE *file = fopen(argv[1], "r");
	Token *shift = createToken(TOKEN_IDENTIFIER, "shift", 6);
	Token *logical = createToken(TOKEN_IDENTIFIER, "logical", 5);
	

	
	/* this variable ensures that expression is valid */					
	int *num_of_reductions = malloc(sizeof(int));
	/* this variable tracks number of logical operators  */
	int *num_of_logical = malloc(sizeof(int));
		
	int *is_logical = malloc(sizeof(int));

	/*symbol of input and on top of stack */
	int *input = malloc(sizeof(int));
	int *top = malloc(sizeof(int));
	
	int *end_if = malloc(sizeof(int));

	/* tracking brackets in expression */
	int *brackets = malloc(sizeof(int));

	int *has_null = malloc(sizeof(int));
	int *has_float = malloc(sizeof(int));
	int *has_string = malloc(sizeof(int));
	int *has_int = malloc(sizeof(int));


	if (!brackets || !top || !input || !end_if || !is_logical || !num_of_logical || !num_of_reductions ) {
		pushError("Helper variable malloc fail.\n", 99);
	}

	if (!has_float || !has_int || !has_null || !has_string) {
		pushError("Helper vairalbe malloc fail.\n", 99);
	}

	//variables tracking if certain variable types are in current expression
	*has_null = 0;
	*has_float = 0;
	*has_string = 0;
	*has_int = 0;

	//variable tracking other useful data related to expression
	*num_of_reductions = 0;
	*num_of_logical = 0;
	*is_logical = -1;
	*input = -1;
	*top = -1;
	*end_if = 0;
	*brackets = 0;

	int r;
	int rule;

	// processing fisrt token
	loadToken(current, stack, &brackets, &num_of_logical, top, input, end_if, has_null , has_float, has_string, has_int, &logical, &variable);

	
	while ((exEmpty(stack) != 0 || *num_of_reductions == 0 || *input != -1)) {	/* main loop for parsing tokens */


		if (*input != -1 && *top != -1) {
			rule = *(*table)[*input]->output_order[*top];
		} else {
			break;
		}

		if (*end_if == 1) {
			break;
		}


		switch(rule) 
		{
			case 0:	//shift
				exPush(stack, shift);
				exPush(stack, (*current)->current_token);

				update(current);
				loadToken(current, stack, &brackets, &num_of_logical, top, input, end_if, has_null, has_float, has_string, has_int, &logical, &variable);
				break;
			case 1:	//reduction

				while (strcmp(exTop(stack)->token->param, "shift") != 0) {
					exPop(stack);					
				}
				exPop(stack);
				*top = get_top(stack);
				(*num_of_reductions)++;
				break;
			
			case 2: //equal

				exPush(stack, (*current)->current_token);
				update(current);
				loadToken(current, stack, &brackets, &num_of_logical, top, input, end_if, has_null, has_float, has_string, has_int, &logical, &variable);
				break;

			case 3: //error
				if (*input != -1 && *end_if != 0) {
					pushError("Invalid expression\n", 2);
				} else {

					while (strcmp(exTop(stack)->token->param, "shift") != 0) {
						exPop(stack);					
					}
					exPop(stack);
					*top = get_top(stack);
					(*num_of_reductions)++;
					goto end;
				}
		}

	}

   end:


	while (exEmpty(stack) != 1) {
		while (strcmp(exTop(stack)->token->param, "shift") != 0) {
			exPop(stack);					
		}
		exPop(stack);
		*top = get_top(stack);
		(*num_of_reductions)++;	
	}
	


	if (*num_of_logical == 1) {
		*is_logical = 1;
	}
	else if (*num_of_logical == 0) {
		*is_logical = 0;
	}
	else {
		pushError("too many logical operators.\n", 2 );
	}


   	if (*end_if == 1 && (*is_logical != 1 || *num_of_reductions > 2)) {
		pushError("After 'if' or 'while' must be logical expression.\n", 7);
   	} 

   	if (*end_if == 0 && *is_logical != 0) {
		pushError("Cannot use logical expression after assign token.\n", 7);
   	}

	if (*has_null == 1 && (*is_logical != 1 || *num_of_reductions > 2)) { 
		pushError("Aritmethic expression has null token.\n", 7);
	}


	if (*has_null == 1 && (logical->type != TOKEN_EQUAL || logical->type != TOKEN_NOT_EQUAL)) {
		pushError("Expression with null can contain only '==' or '!=' as logical operators.\n", 7);
	}

	if (*is_logical == 1) {
		r = 3; //result type true/false
	} else {

		if (*has_int && *has_float == 0) {
			r = 0; //reuslt type integer
			if (*num_of_reductions < 2) {
				AssignInt( LOCAL_FRAME ,variable->name, *has_int);
			}
		}
		else if (*has_float) {
			r = 1; //result type float 
			if (*num_of_reductions < 2) {
				AssignFloat(LOCAL_FRAME, variable->name, *has_float);
			}
		}
		else if (*has_null == 1 && *num_of_reductions < 3) {
			r = 4; //result type null
		}
	}


	if (*is_logical == 1 && (*num_of_logical > 1 || *brackets != 0) ) {
		printf("is logical: %d, num_of_logical: %d, brackets: %d\n", *is_logical, *num_of_logical, *brackets);
		pushError("Invalid logical expression.\n", 2);
	} 

	if (*is_logical == 0 && (*num_of_logical != 0 || *brackets != 0)) {
		pushError("Invalid arithmetic expression.\n", 2);
	}


	if (*num_of_reductions > 0) {
		if (exEmpty(stack) != 1) {
			exPop(stack);
		}
		printf("\nSUCCESS!\nNumber of reductions: %d\nNumber of logical operators: %d => is logical: %d\n", *num_of_reductions, *num_of_logical, *is_logical);
		printf("brackets: %d\ntop stack: %s\n", *brackets, exTop(stack)->token->param);

	} else {
		pushError("Invalid expression.\n", 2);
	}
	

	/* freeing allocated helper variables and precedence table */
	free(has_float);
	free(has_int);
	free(has_string);
	free(has_null);
	free(logical);

	free(bottom_stack);
	free(shift);
	free(brackets);
	free(is_logical);
	free(num_of_logical);
	free(num_of_reductions);
	free(input);
	free(top);

	exDispose(stack);
	free(stack);
	
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {			
			free((*table)[i]->output_order[j]);
		}
		free((*table)[i]);
	}

	free(table);

	return r;

}


