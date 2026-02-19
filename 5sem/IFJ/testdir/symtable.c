/**
 * @file symtable.c
 * @brief This file contains functions for handling symbol table.
 *
 * Functions to create, delete, insert, and retrieve symbols from the symbol table.
 *
 * @author 
 *  - Eliška Krejčíková (xkrejce00)
 */

#include "symtable.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int get_height(symbol_t *symbol) {
    if (symbol == NULL) {
        return 0;
    }
    return symbol->height;
}


char *create_key(char *name, int param_cnt, function_type_t function) {
    if(function != T_UNKNOWN) {
        char function_type;

        switch(function) {
            case T_FUNC:
                function_type = 'f';

                break;
            case T_GETTER:
                function_type = 'g';
                break;
            case T_SETTER:
                function_type = 's';
                break;
            default:
                // should not happen
                return NULL;
        }
        // convert int to string
        int param_str_len = snprintf(NULL, 0, "%d", param_cnt);
        char *key = malloc(strlen(name) + strlen("_")*2 + param_str_len + 1 + 1);
        if (key == NULL) {
            return NULL;
        }
        sprintf(key, "%s_%d_%c", name, param_cnt, function_type);
        return key;
    } else {
        //var, just the name
        char *key = malloc(strlen(name) + 1);
        if (key == NULL) {
            return NULL;
        }
        strcpy(key, name);
        return key;
    }
}

symbol_t *rotate_to_the_right(symbol_t *symbol) {
    symbol_t *symbol_t_l = symbol->left;
    symbol_t *symbol_t_lr = symbol_t_l->right;
    symbol->left = symbol_t_lr;
    symbol_t_l->right = symbol;

    // change height
    if (get_height(symbol->left) > get_height(symbol->right)) {
        symbol->height = 1 + get_height(symbol->left);
    } else {
        symbol->height =1 + get_height(symbol->right);
    }
    if (get_height(symbol_t_l->left) > get_height(symbol_t_l->right)) {
        symbol_t_l->height = 1 + get_height(symbol_t_l->left);
    } else {
        symbol_t_l->height = 1 + get_height(symbol_t_l->right);
    }

    return symbol_t_l;
}
symbol_t *rotate_to_the_left(symbol_t *symbol) {
    symbol_t *symbol_t_r = symbol->right;
    symbol_t *symbol_t_rl = symbol_t_r->left;
 
    symbol->right = symbol_t_rl;
    symbol_t_r->left = symbol;

    //fix height 
    if (get_height(symbol->left) > get_height(symbol->right)) {
        symbol->height = 1 + get_height(symbol->left);
    } else {
        symbol->height = 1 + get_height(symbol->right);
    }
   
    if (get_height(symbol_t_r->left) > get_height(symbol_t_r->right)) {
        symbol_t_r->height = 1 + get_height(symbol_t_r->left);
    } else {
        symbol_t_r->height = 1 + get_height(symbol_t_r->right);
    }

    return symbol_t_r;
}

void balance_symtable(symbol_t** symtable, int balance, char *key_name) {     
    // balance
    if (balance > 1) { //left side is longer    
        if ((*symtable)->left != NULL) {
            int cmp_val = strcmp(key_name, (*symtable)->left->symbol_key);
            if (cmp_val < 0) { //which child of left
                *symtable = rotate_to_the_right(*symtable);
            } else {
                (*symtable)->left = rotate_to_the_left((*symtable)->left);
                *symtable = rotate_to_the_right(*symtable);
            }
        } else {
            return;
        }
    }
    else if (balance < -1) { //right side
        if ((*symtable)->right != NULL) {
            int cmp_val = strcmp(key_name, (*symtable)->right->symbol_key);
            if (cmp_val > 0) { //which child of right
                *symtable = rotate_to_the_left(*symtable);
            } else {
                (*symtable)->right = rotate_to_the_right((*symtable)->right);
                *symtable = rotate_to_the_left(*symtable);
            }
        } else {
            return;
        }
    }
}

// firstly call create_new_function_symbol/create_new_variable_symbol and then add
int insert_symbol_to_symtable(symbol_t **symtable, symbol_t *symbol) {
    if (*symtable == NULL) {
        *symtable = symbol;
        symbol->left = symbol->right = NULL;
        symbol->height = 1;
        return 0;
    }

    int cmp_val = strcmp(symbol->symbol_key, (*symtable)->symbol_key);
    if (cmp_val < 0) {
        return insert_symbol_to_symtable(&(*symtable)->left, symbol);
    } else if (cmp_val >0) {
        return insert_symbol_to_symtable(&(*symtable)->right,symbol);
    } else if (cmp_val == 0) { //already in, check the symbol_t later
        // changing bool to declared
        if((*symtable)->declared == true) {
            // exit semantic error redefinition            
                return 1;
            
        }
        (*symtable)->declared = symbol->declared;
        (*symtable)->param_cnt = symbol->param_cnt;
        (*symtable)->function_type = symbol->function_type;
        (*symtable)->is_function = symbol->is_function;
        (*symtable)->is_global_var = symbol->is_global_var;
        (*symtable)->var_type = symbol->var_type;

        
        return 0;
    }
    
    //change height of symtable
    int height_of_left = get_height((*symtable)->left);
    int height_of_right = get_height((*symtable)->right);

    if (height_of_left > height_of_right) {
        (*symtable)->height = height_of_left + 1;
    } else {
        (*symtable)->height = height_of_right + 1;
    }

    int balance = height_of_left - height_of_right; 
    if (balance == 1 || balance == -1 || balance == 0) { //doesnt need to be balanced
        return 0;
    }
    balance_symtable(symtable, balance, symbol->symbol_key);

    return 0;
}

void delete_symtable(symbol_t **symtable) {
    if (*symtable == NULL) {
        return;
    }
    delete_symtable(&(*symtable)->left);
    delete_symtable(&(*symtable)->right);

    free((*symtable)->name);
    free((*symtable)->symbol_key);
    free(*symtable);

    *symtable = NULL;
}

symbol_t *create_new_function_symbol(char *name, int param_cnt, function_type_t function_type, bool declared) {
    symbol_t *symbol = malloc(sizeof(symbol_t));
    if (symbol == NULL) {
        return NULL;
    }
    symbol->is_function = true;

    symbol->name = malloc(strlen(name) + 1);
    if (symbol->name == NULL) { 
        free(symbol); 
        return NULL; 
    }
    strcpy(symbol->name, name);
    symbol->symbol_key = create_key(name, param_cnt, function_type);
    if (symbol->symbol_key == NULL) {
        free(symbol->name);
        free(symbol);
        return NULL;
    }

    symbol->param_cnt = param_cnt;
    symbol->function_type = function_type;
    symbol->declared = declared;

    symbol->left = NULL;
    symbol->right = NULL;
    symbol->height = 1;

    return symbol;
}

bool change_variable_type(symtable_stack_t *stack, char *name, var_type_t new_type) {
    if (stack == NULL) {
        return false;
    }
    char* key_name = create_key(name, 0, T_UNKNOWN);
    if (key_name == NULL) {
        return false;
    }

    symbol_t *var_symbol = get_symbol_from_upper_scopes(stack, key_name);
    if(var_symbol == NULL) {
        free(key_name);
        return false;
    }

    var_symbol->var_type = new_type;
    free(key_name);
    return true;
}

symbol_t *create_new_variable_symbol(char *name, bool global, var_type_t var_type) {
    symbol_t *symbol = malloc(sizeof(symbol_t));
    if (symbol == NULL) {
        return NULL;    
    }
    symbol->is_function = false;
    symbol->declared = true;
    symbol->is_global_var = global;
    symbol->param_cnt = 0;
    symbol->var_type = var_type;

    symbol->name = malloc(strlen(name) + 1);
    if (symbol->name == NULL) {
        free(symbol);
        return NULL;
    }
    strcpy(symbol->name, name);

    symbol->symbol_key = malloc(strlen(name) + 1);
    if (symbol->symbol_key == NULL) { 
        free(symbol->name);
        free(symbol);
        return NULL; 
    }
    strcpy(symbol->symbol_key, name);

    symbol->left = NULL;
    symbol->right = NULL;
    symbol->height = 1;

    return symbol;
}

symbol_t **create_new_symtable() {
    symbol_t **symbol_table = malloc(sizeof(symbol_t*));
    if (symbol_table == NULL) {
        return NULL;
    }
    *symbol_table = NULL;

    return symbol_table;
}

int insert_to_symtable(symbol_t **symtable, char *name, bool function, int param_cnt, function_type_t function_type, bool declared, bool global_var, var_type_t var_type) {
    if (symtable == NULL) {
        return 1;
    }

    symbol_t *symbol = NULL;
    if (function) {
        symbol = create_new_function_symbol(name, param_cnt, function_type, declared);
    } else {
        symbol = create_new_variable_symbol(name, global_var, var_type);
    }
    if (symbol == NULL) {
        return 1;
    }

    insert_symbol_to_symtable(symtable, symbol);
    return 0;
}

// before being called, make sure you are using key_name not name(use function create_key)
symbol_t *get_symbol_from_symtable(symbol_t *symbol, char *key_name) {
    if (symbol == NULL) {
        return NULL;
    }
    int cmp_val = strcmp(key_name, symbol->symbol_key);
    if (cmp_val < 0) {
        return get_symbol_from_symtable(symbol->left, key_name);
    } else if (cmp_val > 0) {
        return get_symbol_from_symtable(symbol->right, key_name);
    } else {
        return symbol;
    }
    
    return NULL;
}


// firstly create key_name 
int get_scope_number(stack_element_t* stack,char *key_name) {
    int scope_number = 0;
    stack_element_t *current = stack;
    while (current != NULL) {
        symbol_t *found = get_symbol_from_symtable(*(current->symbol_table), key_name);
        if(found != NULL) {
            return scope_number;
        }
        current = current->next;
        scope_number++;
    }
    return -1;
}


symtable_stack_t * init_stack() {
    symtable_stack_t *stack = malloc(sizeof(symtable_stack_t));
    if (stack == NULL) {
        return NULL;
    }

    stack->num_of_scopes = 0;
    stack->top = NULL;

    return stack;
}

int push_to_stack(symtable_stack_t *stack, symbol_t **symtable) {
    if (stack == NULL || symtable == NULL) {
        return 1;
    }
    stack_element_t *new_element = malloc(sizeof(stack_element_t));
    if (new_element == NULL) {
        return 1;
    }

    new_element->symbol_table = symtable;
    new_element->next = stack->top;
    stack->top = new_element;
    stack->num_of_scopes++;

    return 0;

}

symbol_t **pop_from_stack(symtable_stack_t *stack) {
    if(stack == NULL) {
        return NULL;
    }

    stack_element_t *top = stack->top;
    symbol_t **symtable = top->symbol_table;

    stack->top = top->next;
    stack->num_of_scopes--;

    return symtable;
}

void delete_stack(symtable_stack_t *stack) {
    if (stack == NULL) {
        return;
    }

    while (stack->num_of_scopes != 0) {
        symbol_t **symtable = pop_from_stack(stack);
        delete_symtable(symtable);
        free(symtable);
    }
    free(stack);
}

symbol_t *get_symbol_from_upper_scopes(symtable_stack_t *stack, char *key_name) {

    stack_element_t *top = stack->top;
    while (top != NULL) {
        symbol_t *found = get_symbol_from_symtable(*(top->symbol_table), key_name);
        if(found == NULL) {
            top = top->next;
        } else {
            return found;
        }

    }
    return NULL;
}

bool insert_into_certain_scope(symtable_stack_t *stack, symbol_t*symbol, int scope_number) {
    if (stack == NULL || symbol == NULL) {
        return false;
    }
    stack_element_t *current = stack->top;
    int current_scope = stack->num_of_scopes;
    while (current != NULL) {
        if (current_scope == scope_number) {

            insert_symbol_to_symtable(current->symbol_table, symbol);

            return true;
        }
        current = current->next;
        current_scope--;
    }
    return false;
}

void print_symbol_table(symbol_t **symtable_from_stack) {
    if (symtable_from_stack == NULL || *symtable_from_stack == NULL) {
        printf("--------------------------------------------\n");
        return;
    }

    symbol_t *root = *symtable_from_stack;
    print_symbol_table(&root->left);

    printf("Symbol name: %s\n", root->name);
    if (root->is_function) {
        printf("is a function with %d params\n", root->param_cnt);
        printf("function type: %d\n", root->function_type);
        printf("declared: %d\n", root->declared);
    } else {
        printf("is a variable\n");
        printf("type: %d\n", root->var_type);
    }

    print_symbol_table(&root->right);
}