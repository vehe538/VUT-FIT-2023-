/**
 * @file semantics.h
 * @brief Functions for semantic analysis used in parsing.
 *
 * This header provides functions used for semantic analysis during parsing,
 * including symbol table checks, function/variable validation, and data type
 * conversions.
 * 
 * @author Eliška Krejčíková (xkrejce00)
 * 
 */

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "symtable.h"
#include "scanner.h"
#include <stdbool.h>

/**
 * @brief Checks whether a variable is being redefined in the current scope.
 *
 * @param stack A stack containing all symtables.
 * @param var_name Name of the variable being defined.
 * @return Returns true if the variable is already defined in the current scope, false otherwise.
 */
bool check_var_redefined(symtable_stack_t *stack, char *var_name);

/**
 * @brief Ensures a function symbol exists, inserting it if necessary.
 *
 * This is used for function calls: if the function is not yet declared,
 * an undeclared symbol is inserted into the global scope.
 *
 * @param stack A stack containing all symtables.
 * @param func_name Name of the function.
 * @param param_cnt Number of parameters.
 * @param function_type Type of the function (normal, getter, setter).
 */
void check_function(symtable_stack_t *stack, char *func_name, int param_cnt, function_type_t function_type);

/**
 * @brief Looks through the symbol tables. 
 *          
 * If the variable does not exist, the symbol could possibly be a getter,
 *  so inserts a new symbol of type getter to the first symtable.
 * 
 *
 * @param stack A stack containing all symtables.
 * @param var_name Name of the variable.
 */
void check_variable(symtable_stack_t *stack, char *var_name);

/**
 * @brief Looks through the symbol table for non-defined functions.
 *
 * @param symtable Pointer to the symbol table.
 * @return Returns true if all functions are defined, false if any remain undefined.
 */
bool check_nondefined(symbol_t **symtable);

/**
 * @brief Verifies argument counts for built-in functions.
 *
 * @param func_name Name of the built-in function.
 * @param param_cnt Number of arguments provided.
 * @return
 * - 0 if correct  
 * - 1 if incorrect parameter count  
 * - 2 if it's not a built-in function
 */
int check_built_in_function_params(char *func_name, int param_cnt);

/**
 * @brief Checks whether a main() function is present.
 *
 * @param symtable Pointer to the symbol table.
 * @return Returns true if main() exists, false otherwise.
 */
bool check_main(symbol_t **symtable);

/**
 * @brief Function looks for the same named functions but with different parameter counts.
 * 
 * @param root Pointer to the root of the symbol table.
 * @param non_declared Pointer to the non-defined function symbol.
 * @return Returns true if a conflict is found, false otherwise.
 */
bool find_conflict(symbol_t *root, symbol_t *non_declared);

/**
 * @brief Helper function to resursively check for wrong parameter counts in the whole symbol table.
 *
 * @param root_symbol Pointer to the root of the symbol table.
 * @param node Pointer to the current symbol node.
 * @return Returns true if a conflict is found, false otherwise.
 */
bool wrong_param_cnt_check_symtable(symbol_t *root_symbol, symbol_t *node);

/**
 * @brief Checks for wrong parameter counts in non-defined function calls.
 *
 * If a function is called with wrong parameter count, the symbol is inserted as non-defined.
 * In order to return correct error, 
 * @param symtable Pointer to the symbol table.
 * @return Returns true if a mismatch is found, false otherwise.
 */
bool check_wrong_param_cnt_fun_call(symbol_t **symtable);

/**
 * @brief Converts an internal expression data type into a variable type.
 *
 * @param dt The expression data type.
 * @return Corresponding variable type.
 */
var_type_t data_type_to_var_type(data_type dt);

/**
 * @brief Converts a variable type into an internal expression data type.
 *
 * @param vt The variable type.
 * @return Corresponding expression data type.
 */
data_type var_type_to_data_type(var_type_t vt);

#endif
