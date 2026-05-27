/**
 * @file symtable.h
 * @brief Implementation of symbol table and a stack of symbol tables.
 * 
 * Includes all functions to manipulate symbol tables and the stack.
 * 
 * @author Eliška Krejčíková (xkrejce00)
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>

/**
 * @brief Types of function-like entities stored in the symbol table.
 * 
 * T_UNKNOWN can be used for variables.
 */
typedef enum {
    T_FUNC,
    T_GETTER,
    T_SETTER,
    T_BUILTIN,
    T_UNKNOWN
} function_type_t;

/**
 * @brief Types of variables stored in the symbol table.
 */
typedef enum {
    TYPE_NUM,
    TYPE_STRING,
    TYPE_NULL,
    TYPE_PARAM,
    TYPE_UNKNOWN
} var_type_t;

/**
 * @brief Symbol table node representing either a variable or a function.
 *
 * Setters and getters are treated as a special type of function.
 * The symbols have to a have unique in the symbol table. So the symbol will have
 * to be identified by a key consisting of name + param_cnt + function_type for functions.
 * 
 * Declared is used as a flag to keep track of whether the function has been declared yet. 
 * It's used do semantics checks after the program is fully parsed.
 */
typedef struct symbol {
    bool is_function;
    char *name;
    char *symbol_key;

    bool is_global_var;
    int param_cnt;
    bool declared;

    function_type_t function_type;
    var_type_t var_type;

    struct symbol *left;
    struct symbol *right;
    int height;
} symbol_t;

/**
 * @brief A node in the stack of symbol tables.
 */
typedef struct stack_element {
    symbol_t **symbol_table;
    struct stack_element *next;
} stack_element_t;

/**
 * @brief Stack of symtables used to track scopes.
 */
typedef struct symtable_stack {
    int num_of_scopes;
    stack_element_t *top;
} symtable_stack_t;

/**
 * @brief Initializes a new symbol table stack.
 * 
 * @return Pointer to a newly allocated stack, or NULL on failure.
 */
symtable_stack_t *init_stack();

/**
 * @brief Pops the top symbol table from the stack.
 * 
 * @param stack A stack containing all symtables.
 * @return Pointer to the popped symbol table, or NULL if empty.
 */
symbol_t **pop_from_stack(symtable_stack_t *stack);

/**
 * @brief Pushes a symbol table onto the stack.
 * 
 * @param stack A stack containing all symtables.
 * @param symtable A symbol table to push onto the stack.
 * @return 0 on success, 1 on failure.
 */
int push_to_stack(symtable_stack_t *stack, symbol_t **symtable);

/**
 * @brief Deletes the entire symbol table stack including all symtables.
 * 
 * @param stack A stack containing all symtables.
 */
void delete_stack(symtable_stack_t *stack);


/**
 * @brief Computes the height of the symbol in the tree.
 * 
 * @param symbol Symbol table node.
 * @return Height of the node.
 */
int get_height(symbol_t *symbol);

/**
 * @brief Creates a unique key for a symbol.
 *
 * For functions: `name_paramcnt_type` 
 * For type is 'f' for function, 'g' for getter, 's' for setter 
 * For variables: `name`
 *
 * @param name Base identifier name.
 * @param param_cnt Number of parameters. For variables, 0 is used.
 * @param function Function type.
 * @return Newly allocated key string, or NULL on failure.
 */
char *create_key(char *name, int param_cnt, function_type_t function);

/**
 * @brief Performs a right rotation on an a subtree.
 * 
 * @param symbol Subtree root.
 * @return New root after rotation.
 */
symbol_t *rotate_to_the_right(symbol_t *symbol);

/**
 * @brief Performs a left rotation on an a subtree.
 * 
 * @param symbol Subtree root.
 * @return New root after rotation.
 */
symbol_t *rotate_to_the_left(symbol_t *symbol);

/**
 * @brief Balances a subtree after insertion.
 * 
 * @param symtable Pointer to subtree root.
 * @param balance Balance factor of the subtree.
 * @param key_name Key of the inserted symbol.
 */
void balance_symtable(symbol_t **symtable, int balance, char *key_name);

/**
 * @brief Inserts a symbol into the symbol table.
 *
 *
 * @param symtable Pointer to the symbol table root.
 * @param symbol Symbol to insert.
 * @return 0 on success, 1 on semantic redefinition error.
 */
int insert_symbol_to_symtable(symbol_t **symtable, symbol_t *symbol);

/**
 * @brief Recursively deletes all nodes in a symbol table.
 * @param symtable Pointer to the symbol table root.
 */
void delete_symtable(symbol_t **symtable);

/**
 * @brief Creates a new function symbol.
 * @param name Function name.
 * @param param_cnt Number of parameters.
 * @param function_type Function type.
 * @param declared True if declared.
 * @return Newly allocated symbol, or NULL on failure.
 */
symbol_t *create_new_function_symbol(char *name, int param_cnt,function_type_t function_type, bool declared);

/**
 * @brief Creates a new variable symbol.
 * @param name Variable name.
 * @param global Whether the variable is global.
 * @param var_type Variable type.
 * @return Newly allocated symbol, or NULL on failure.
 */
symbol_t *create_new_variable_symbol(char *name, bool global, var_type_t var_type);

/**
 * @brief Allocates a new (empty) symbol table.
 * @return Pointer to newly allocated symbol table.
 */
symbol_t **create_new_symtable();

/**
 * @brief Inserts a symbol into the table.
 * 
 * @param symtable Pointer to the symbol table root.
 * @param name Symbol name.
 * @param function True if symbol is a function.
 * @param param_cnt Number of parameters (0 for variables).
 * @param function_type Function type.
 * @param declared True if declared.
 * @param global_var True if variable is global.
 * @param var_type Variable type.
 * @return 0 on success, 1 on failure.
 */
int insert_to_symtable(symbol_t **symtable, char *name, bool function, int param_cnt, function_type_t function_type, bool declared, bool global_var, var_type_t var_type);

/**
 * @brief Looks up a symbol by key in a symbol table.
 * 
 * @param symbol Root of the symbol table.
 * @param key_name Key of the symbol.
 * @return Pointer to the symbol, or NULL if not found.
 */
symbol_t *get_symbol_from_symtable(symbol_t *symbol, char *key_name);

/**
 * @brief Updates a variable’s type in the nearest scope.
 * 
 * @param stack A stack containing all symtables.
 * @param name Variable name.
 * @param new_type New variable type.
 * @return True on success.
 */
bool change_variable_type(symtable_stack_t *stack, char *name, var_type_t new_type);

/**
 * @brief Searches for a symbol through all the symbol tables in the stack.
 *
 *
 * @param stack A stack containing all symtables.
 * @param key_name Symbol key to search for.
 * @return Pointer to symbol or NULL if not found.
 */
symbol_t *get_symbol_from_upper_scopes(symtable_stack_t *stack, char *key_name);

/**
 * @brief Function used to insert symbol into a certain scope in the stack.
 * 
 * @param stack A stack containing all symtables.
 * @param symbol Symbol to insert.
 * @param scope_number Target scope number.
 * @return True if inserted.
 */
bool insert_into_certain_scope(symtable_stack_t *stack, symbol_t *symbol, int scope_number);

/**
 * @brief Helper function to get the scope number of a symbol in the stack.
 * 
 * @param stack Stack element.
 * @param key_name Key of the symbol.
 * @return Scope number, or -1 if not found.
 */
int get_scope_number(stack_element_t *stack, char *key_name);

/**
 * @brief Helper function used for debugging: prints the symbol table.
 * 
 * @param symtable_from_stack Pointer to the root of the symbol table.
 */
void print_symbol_table(symbol_t **symtable_from_stack);

#endif // SYMTABLE_H
