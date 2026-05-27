/**
 * @file semantics.c
 * @brief Implementation of functions used in semantic analysis
 * 
 * This file contains functions to perform semantic checks
 * on the symbol tables and function/variable declarations, parameter count, data types, etc.
 * 
 * @author
 *  - Eliška Krejčíková (xkrejce00)
 *  - Natália Holbíková (xholbin00)
 */

#include <stdlib.h>

#include "parser.h"
#include "semantics.h"
#include "symtable.h"
#include "error_codes.h"

void check_function(symtable_stack_t *stack, char *func_name, int param_cnt, function_type_t function_type) {
    if (stack == NULL) {
        return;
    }
    char* key_name = create_key(func_name, param_cnt, function_type);
    if (key_name == NULL) {
        return;
    }

    symbol_t *func_symbol = get_symbol_from_upper_scopes(stack, key_name);
    if(func_symbol == NULL) {
        symbol_t *new_function_symbol = create_new_function_symbol(func_name, param_cnt, function_type, false);
        if (new_function_symbol == NULL) {
            free(key_name);
            exit(INTERNAL_ERROR);
        }
        insert_into_certain_scope(stack, new_function_symbol, 1);
        return;
    }

    return;
}

bool check_var_redefined(symtable_stack_t *stack, char *var_name) {
    if (stack == NULL) {
        return false;
    }
    char *key = create_key(var_name, 0, T_UNKNOWN);
    symbol_t **symtable = stack->top->symbol_table;
    symbol_t *var_symbol = get_symbol_from_symtable(*symtable, key);
    
    if(var_symbol == NULL) {
        free(key);
        return false;
    }

    if(var_symbol->is_function && var_symbol->function_type == T_SETTER && !var_symbol->declared) {
        free(key);
        return false;
    }

    free(key);
    return true;
}

void check_variable(symtable_stack_t *stack, char *var_name) {
    if (stack == NULL) {
        return;
    }
    char *key = create_key(var_name, 0, T_UNKNOWN);
    symbol_t *var_symbol = get_symbol_from_upper_scopes(stack, key);
    
    if(var_symbol == NULL) {
        symbol_t *function_symbol = create_new_function_symbol(var_name, 0, T_GETTER, false);
        if(function_symbol == NULL) {
            free(key);
            return;
        }

        insert_into_certain_scope(stack, function_symbol,1);
    }
}

bool check_nondefined(symbol_t **symtable) {
    if (*symtable == NULL) {
        return true;
    }
    if(symtable == NULL) {
        return true;
    }

    symbol_t *node = *symtable;

    if (!node->declared) {
        return false;
    }

    if (!check_nondefined(&node->left)) {
        return false;
    }

    if (!check_nondefined(&node->right)) {
        return false;
    }

    return true;
}

int check_built_in_function_params(char *func_name, int param_cnt) {
    if(strcmp(func_name, "write") ==0) {
        if(param_cnt != 1) {
            return 1;
        }
    } else if (strcmp(func_name, "read_str") ==0) {
        if(param_cnt != 0) {
            return 1;
        }
    } else if (strcmp(func_name, "read_num") ==0) {
        if(param_cnt != 0) {
            return 1;
        }
    } else if (strcmp(func_name, "floor") ==0) {
        if(param_cnt != 1) {
            return 1;
        }
    } else if (strcmp(func_name, "str") ==0) {
        if(param_cnt != 1) {
            return 1;
        }
    } else if (strcmp(func_name, "length") ==0) {
        if(param_cnt != 1) {
            return 1;
        }
    }else if (strcmp(func_name, "substring") ==0) {
        if(param_cnt != 3) {
            return 1;
        }
    }else if (strcmp(func_name, "strcmp") ==0) {
        if(param_cnt != 2) {
            return 1;
        }
    } else if (strcmp(func_name, "ord") == 0) {
        if(param_cnt != 2) {
            return 1;
        }
    } else if (strcmp(func_name, "chr") ==0) {
        if(param_cnt != 1) {
            return 1;
        }
    } else {
        return 2;
    }
    return 0;

}


bool check_main(symbol_t **symtable) {
    char *key = create_key("main", 0, T_FUNC);
    symbol_t *main_symbol = get_symbol_from_symtable(*symtable, key);
    if(main_symbol == NULL) {
        free(key);
        return false;
    }

    if(!main_symbol->is_function || main_symbol->function_type != T_FUNC) {
        free(key);
        return false;
    }
    free(key);
    return true;
}

bool find_conflict(symbol_t *root, symbol_t *non_declared) {
    if (root == NULL || non_declared == NULL) {
        return false;
    }

    if (root != non_declared && strcmp(root->name, non_declared->name) == 0) {

        if (root->param_cnt != non_declared->param_cnt && root->function_type == T_FUNC && non_declared->function_type == T_FUNC) {
            return true; 
        }
    }

    if (find_conflict(root->left, non_declared)) {
        return true;
    }
    if (find_conflict(root->right, non_declared)) {
        return true;
    }
    return false;
}


bool wrong_param_cnt_check_symtable(symbol_t *root_symbol, symbol_t *node) {
    if (node == NULL) {
        return false;
    }

    if (!node->declared && node->is_function) {
        if (find_conflict(root_symbol, node)) {
            return true;
        }
    }

    if (wrong_param_cnt_check_symtable(root_symbol, node->left)) {
        return true;
    }

    if (wrong_param_cnt_check_symtable(root_symbol, node->right)) {
        return true;
    }

    return false;
}

bool check_wrong_param_cnt_fun_call(symbol_t **symtable) {
    if (symtable == NULL || *symtable == NULL) {
        return false;
    }

    return wrong_param_cnt_check_symtable(*symtable, *symtable);
}

// Type conversion functions
var_type_t data_type_to_var_type(data_type dt) {
    switch (dt) {
        case DATA_TYPE_NUM:
            return TYPE_NUM;
        case DATA_TYPE_STRING:
            return TYPE_STRING;
        case DATA_TYPE_NULL:
            return TYPE_NULL;
        case DATA_TYPE_BOOL:
        case DATA_TYPE_VOID:
        case DATA_TYPE_UNKNOWN:
        default:
            return TYPE_UNKNOWN;
    }
}

data_type var_type_to_data_type(var_type_t vt) {
    switch (vt) {
        case TYPE_NUM:
            return DATA_TYPE_NUM;
        case TYPE_STRING:
            return DATA_TYPE_STRING;
        case TYPE_NULL:
            return DATA_TYPE_NULL;
        case TYPE_PARAM:
        case TYPE_UNKNOWN:
        default:
            return DATA_TYPE_UNKNOWN;
    }
}
