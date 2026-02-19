/**
 * @file parser.h
 * @brief Header file for the parser module.
 *
 * This file contains declarations for all parsing functions used to build
 * the abstract syntax tree (AST) and perform basic syntactic and semantic checks.
 *
 * @author Eliška Krejčíková (xkrejce00)
 */

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "ast.h"
#include "symtable.h"

/**
 * @brief Function to start parsing.
 * 
 * @param ast Pointer to the abstract syntax tree.
 * @param symtable_stack A stack containing all symtables.
 */
void parse(AstNode *ast, symtable_stack_t *symtable_stack);

/**
 * @brief Parses the entire program (RULE 1).
 * 
 * @param ast Pointer to the abstract syntax tree.
 * @param symtable_stack A stack containing all symtables.
 */
void program(AstNode *ast, symtable_stack_t *symtable_stack);

/**
 * @brief Parses the program prolog (RULE 2).
 */
void prolog();

/**
 * @brief Parses the class definition (RULE 3).
 * 
 * @param ast Pointer to the abstract syntax tree.
 * @param symtable_stack A stack containing all symtables.
 */
void classDefinition(AstNode *ast, symtable_stack_t *symtable_stack);

/**
 * @brief Parses a list of function definitions (RULES 4–5).
 * 
 * @param ast Pointer to the abstract syntax tree.
 * @param symtable_stack A stack containing all symtables.
 */
void functionList(AstNode *ast, symtable_stack_t *symtable_stack);

/**
 * @brief Parses a single function definition (RULE 6).
 * 
 * @param token Token that has to contain the keyword 'static'.
 * @param ast Pointer to the abstract syntax tree.
 * @param symtable_stack A stack containing all symtables.
 */
void functionDefinition(Token *token, AstNode *ast, symtable_stack_t *symtable_stack);

/**
 * @brief Parses the function header (RULE 7).
 * 
 * @param funcNode Pointer to the AST node for the function.
 * @param symtable_stack A stack containing all symtables.
 * @return Returns true if the header is a getter, false otherwise.
 */
bool functionHeader(AstNode *funcNode, symtable_stack_t *symtable_stack);

/**
 * @brief Parses the tail of a function header (RULES 8–10).
 * 
 * @param token_id Function name.
 * @param funcNode Pointer to the AST node for the function.
 * @param symtable_stack A stack containing all symtables.
 * @param param_cnt Number of parameters.
 * @return Returns true if the function is a getter, false otherwise.
 */
bool functionHeaderTail(char *token_id, AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt);

/**
 * @brief Parses the the first parameter of the function (RULES 11–12).
 * 
 * @param funcNode Pointer to the AST node for the function.
 * @param symtable_stack A stack containing all symtables.
 * @param param_cnt Number of parameters.
 */
void parameterList(AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt);

/**
 * @brief Parses the rest of the function parameters (RULES 13–14).
 * 
 * @param funcNode Pointer to the AST node for the function.
 * @param symtable_stack A stack containing all symtables.
 * @param param_cnt Number of parameters.
 */
void parameterListTail(AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt);

/**
 * @brief Parses a block (RULE 15).
 * 
 * @param first_token 
 * @param parentNode Pointer to the parent AST node.
 * @param symtable_stack A stack containing all symtables.
 */
void block(Token *first_token, AstNode *parentNode, symtable_stack_t *symtable_stack);

/**
 * @brief Parses a list of statements in the bloc (RULES 16–17).
 * 
 * @param token The last taken token.
 * @param ast Pointer to the block node.
 * @param symtable_stack A stack containing all symtables.
 */
void statementList(Token *token, AstNode *ast, symtable_stack_t *symtable_stack);

/**
 * @brief Parses the invidual statement (RULES 18–25, 43, 44).
 * 
 * @param first_token Pointer to the first token of the statement.
 * @param ast Pointer to the block node.
 * @param symtable_stack A stack containing all symtables.
 * @return Returns a pointer to the next token.
 */
Token *statement(Token *first_token, AstNode *ast, symtable_stack_t *symtable_stack);

/**
 * @brief Parses the tail of a statement (RULES 26–27).
 * @param symtable_stack A stack containing all symtables.
 * @param ast Pointer to the block node.
 * @param first_token Pointer to the first token of the statement.
 * @return Returns the next token.
 */
Token *statementTail(symtable_stack_t *symtable_stack, AstNode *ast, Token *first_token);

/**
 * @brief Parses argument list of a function call (RULES 28–29).
 * 
 * @param symtable_stack A stack containing all symtables.
 * @param function_call_args Output argument count.
 * @param callNode Pointer to the AST function call node.
 */
void argumentList(symtable_stack_t *symtable_stack, int *function_call_args, AstNode *callNode);

/**
 * @brief Parses the tail of an argument list (RULES 30–31).
 * 
 * @param symtable_stack A stack containing all symtables.
 * @param function_call_args Output argument count.
 * @param callNode Pointer to the AST call node.
 */
void argumentListTail(symtable_stack_t *symtable_stack, int *function_call_args, AstNode *callNode);

/**
 * @brief Parses a term (identifier or literal) (RULES 32–34).
 * 
 * @param token Pointer to the current token.
 * @param symtable_stack A stack containing all symtables.
 * @param callNode Pointer to the AST call node.
 * @return Returns true on success, false on failure.
 */
bool term(Token *token, symtable_stack_t *symtable_stack, AstNode *callNode);

/**
 * @brief Parses a literal (RULES 35–39).
 * 
 * @param token Pointer to the current token.
 * @return Returns true if the literal is valid.
 */
bool literal(Token *token);

/**
 * @brief Parses one or more EOL tokens.
 * @return Returns the next non-EOL token.
 */
Token *eol_rule();

/**
 * @brief Parses zero or more EOL tokens.
 * @return Returns the next non-EOL token.
 */
Token *eol_rule_next();

/**
 * @brief Skips EOL and comment tokens.
 * @return Returns the next valid token.
 */
Token *skip_eol_comment();

/**
 * @brief Checks whether next token matches the expected type.
 * 
 * @param t_type Expected token type.
 * @param comment_skip Whether to skip comments.
 * @return Pointer to matched token, or NULL.
 */
Token *check_token(token_type t_type, bool comment_skip);

// void dea

#endif // PARSER_H
