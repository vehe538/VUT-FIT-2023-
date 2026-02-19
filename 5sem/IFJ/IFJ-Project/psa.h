/**
* @file psa.c
 * @brief Main header file for precedence analysis.
 *
 *
 * Authors:
 * - Juraj Mesík (xmesikj00), 2025
 * - Natália Holbíková (xholbin00), 2025
 */


#include "scanner.h"
#include "expr_stack.h"
#include "symtable.h"
#include "semantics.h"
#include "ast.h"
#include "error_codes.h"
#include "semantics.h"
#include "parser.h"
#include <math.h>

#ifndef PSA_H
#define PSA_H

void freeMemory(Token **input_token, exprStack *stack);
char *tokenType(prec_table_token t);
void parseExpr(Token **input_token, exprStack *stack, prec_table_op operation, bool advance_token, bool expect_operator, AstNode *exprNode, symtable_stack_t *symtable_stack);
bool isExprToken(token_type type);
bool isOperator(prec_table_token type);
prec_table_token convertToken(token_type scanner_token);
prec_table_token handleExpression(exprStackItem handle_items[], int len, AstNode *exprNode, AstNode **outputNode, symtable_stack_t *symtable_stack);
Token *expression(Token *input_token, symtable_stack_t *symtable_stack, AstNode *exprNode);
AstNodeType convertToAst (prec_table_token ptoken);


Token *copyToken(Token *src);
double convertNum(char *token_str);
void assignNodeValue(exprStackItem item, AstNodeType node_type, AstNode *ast_node);
data_type get_expression_result_type(exprStack *stack);
bool allows_eol_after(token_type type);

#endif