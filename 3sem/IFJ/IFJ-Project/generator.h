/**
* @file generator.h
 * @brief Header file for IFJcode25 code generator
 *
 * Authors:
 * - Natália Holbíková (xholbin00), 2025
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include "ast.h"
#include "symtable.h"

/**
 * @brief Main entry point for code generation.
 * 
 * @param ast Pointer to the root of abstract syntax tree.
 * @param global_symtable Pointer to the global symbol table.
 */
void generateCode(AstNode* ast, symbol_t* global_symtable);

/**
 * @brief Generates code for the entire program node.
 * 
 * @param node Pointer to the program AST node.
 */
void generateProgram(AstNode* node);

/**
 * @brief Generates code for a function definition.
 * 
 * @param node Pointer to the function AST node.
 */
void generateFunction(AstNode* node);

/**
 * @brief Generates code for a statement.
 * 
 * @param node Pointer to the statement AST node.
 */
void generateStatement(AstNode* node);

/**
 * @brief Generates code for an expression (result pushed on stack).
 * 
 * @param node Pointer to the expression AST node.
 */
void generateExpression(AstNode* node);

/**
 * @brief Generates code for an if-else statement.
 * 
 * @param node Pointer to the if-else AST node.
 */
void generateIfElse(AstNode* node);

/**
 * @brief Generates code for a while loop.
 * 
 * @param node Pointer to the while AST node.
 */
void generateWhile(AstNode* node);

/**
 * @brief Generates code for an assignment statement.
 * 
 * @param node Pointer to the assignment AST node.
 */
void generateAssignment(AstNode* node);

/**
 * @brief Generates code for a function call.
 * 
 * @param node Pointer to the function call AST node.
 */
void generateFunctionCall(AstNode* node);

/**
 * @brief Generates global variable definitions from symbol table.
 * 
 * @param node Pointer to the symbol table root node.
 */
void generateGlobalsFromSymtable(symbol_t *node);

/**
 * @brief Returns a unique label ID for jump instructions.
 * 
 * @return Unique integer label ID.
 */
int getUniqueLabel();

/**
 * @brief Generates code for built-in IFJ functions.
 * 
 * @param funcName Name of the built-in function (without "Ifj." prefix).
 * @param args Pointer to the first argument AST node.
 */
void generateBuiltinFunction(const char* funcName, AstNode* args);

#endif