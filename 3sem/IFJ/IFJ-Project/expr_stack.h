/**
* @file expr_stack.h
 * @brief Header file for precedence analysis stack; declaration of used data structures and functions 
 *
 *
 * Authors:
 * - Juraj Mesík (xmesikj00), 2025
 * - Natália Holbíková (xholbin00), 2025
 */

#include "scanner.h"
#include "precedence_table.h"
#include "error_codes.h"
#include "ast.h"

#ifndef EXPR_STACK_H
#define EXPR_STACK_H

typedef enum {

    TERMINAL,
    NONTERMINAL

} item_type;

typedef struct exprStackItem {

    prec_table_token type;
    item_type term_type;
    Token *token;
    AstNode *node;
    data_type expr_data_type;
    struct exprStackItem *next;

} exprStackItem;

typedef struct {

    exprStackItem *top;
    int lenght;

} exprStack;

exprStackItem *createStackItem(prec_table_token type, item_type term_type, Token *scanner_token, AstNode *node, exprStackItem *next);
void stackInit(exprStack **stack);
void stackPop(exprStack *stack);
exprStackItem *stackTop(exprStack *stack);
exprStackItem *stackNontermTop(exprStack *stack);
exprStackItem *stackTermTop(exprStack *stack);
void stackPush(exprStack *stack, exprStackItem *push_item);
void stackDelete(exprStack **stack);

#endif