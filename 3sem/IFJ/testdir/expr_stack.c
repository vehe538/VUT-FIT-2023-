/**
* @file expr_stack.c
 * @brief Expression stack for precedence analysis
 *
 *
 * Authors:
 * - Juraj Mesík (xmesikj00), 2025
 * - Natália Holbíková (xholbin00), 2025
 */


#include "expr_stack.h"

// function for initialization of expression stack
void stackInit(exprStack **stack) {
    *stack = malloc(sizeof(exprStack));
    if (*stack == NULL) {
        printError(INTERNAL_ERROR, "malloc fail for stack structure");
        return;
    }

    exprStackItem *bottom = createStackItem(T_STACK_BOTTOM, TERMINAL, NULL, NULL, NULL);
    (*stack)->top = bottom;
}

// function for getting item on top of stack
exprStackItem *stackTop(exprStack *stack) {

    return stack->top;

}

// function for getting terminal nearest to the top of stack
exprStackItem *stackTermTop(exprStack *stack) {

    exprStackItem *term_top = stackTop(stack);
    while(term_top->type != T_STACK_BOTTOM) {

        if (term_top->term_type == TERMINAL) return term_top;
        else term_top = term_top->next;
    }
    return term_top;
}

// function for getting nonterminal nearest to the top
exprStackItem *stackNontermTop(exprStack *stack) {

    exprStackItem *term_top = stackTop(stack);
    while(term_top->next != NULL) {
        if (term_top->term_type == NONTERMINAL) return term_top;
        term_top = term_top->next;
    }
    return NULL;
}

// function for pushing item to the top of stack
void stackPush(exprStack *stack, exprStackItem *push_item) {

    if (push_item->type == T_HANDLE_START && stack->top->term_type == NONTERMINAL) {
        exprStackItem *term_top = stackTermTop(stack);
        exprStackItem *nonterm_top = stackNontermTop(stack);

        nonterm_top->next = push_item;
        push_item->next = term_top;
        return;

    }

    push_item->next = stack->top;
    stack->top = push_item;

}

// function for popping item from stack top
void stackPop(exprStack *stack) {

    if (stack->top->type == T_STACK_BOTTOM) return;
    
    exprStackItem *first = stack->top;
    stack->top = first->next;

    free(first);
}

// function for removing all stack items and stack 
void stackDelete(exprStack **stack) {
    if (!stack || !*stack) return;

    while ((*stack)->top != NULL) {

        exprStackItem *first = (*stack)->top;
        (*stack)->top = first->next;

        if (first->token) {
            if (first->token->token_str) {
                free(first->token->token_str);
            }
            free(first->token);
        }
        free(first);
    }
    free(*stack);
    *stack = NULL;
}

// helper for safely making copies of scanner tokens
Token *copyToken(Token *src) {
    if (!src) return NULL;
    Token *copy = malloc(sizeof(Token));
    if (!copy) printError(INTERNAL_ERROR, "malloc fail");
    copy->type = src->type;
    copy->size = src->size;
    if (src->token_str) {
        copy->token_str = malloc(strlen(src->token_str) + 1);
        if (!copy->token_str) printError(INTERNAL_ERROR, "malloc fail");
        strcpy(copy->token_str, src->token_str);
    } else {
        copy->token_str = NULL;
    }
    return copy;
}

// function for creating new stack item
exprStackItem *createStackItem(prec_table_token type, item_type term_type, Token *scanner_token, AstNode *node, exprStackItem *next) {

    exprStackItem *new = malloc(sizeof(*new));
    if (!new) {
        printError(INTERNAL_ERROR, "malloc failed");   
        return NULL;
    }

    new->type = type;
    new->term_type = term_type;
    new->node = node;
    new->next = next;
    
    if (scanner_token) {
        new->token = copyToken(scanner_token);
        new->expr_data_type = scanner_token->data_type;    
    } else {
        new->token = NULL;
        new->expr_data_type = DATA_TYPE_UNKNOWN;
    }
    return new;
}