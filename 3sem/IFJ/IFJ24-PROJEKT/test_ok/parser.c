/**
* @file parser.c
 * @brief Parser.
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
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "psa.h"
#include "symtable.h"
#include "codegen.h"

// Function to map token_type to a human-readable string
char* tokenTypeToString(token_type type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_KEYWORD_CONST: return "KEYWORD_CONST";
        case TOKEN_KEYWORD_ELSE: return "KEYWORD_ELSE";
        case TOKEN_KEYWORD_FN: return "KEYWORD_FN";
        case TOKEN_KEYWORD_IF: return "KEYWORD_IF";
        case TOKEN_KEYWORD_I32: return "KEYWORD_I32";
        case TOKEN_KEYWORD_F64: return "KEYWORD_F64";
        case TOKEN_KEYWORD_NULL: return "KEYWORD_NULL";
        case TOKEN_KEYWORD_PUB: return "KEYWORD_PUB";
        case TOKEN_KEYWORD_RETURN: return "KEYWORD_RETURN";
        case TOKEN_KEYWORD_U8: return "KEYWORD_U8";
        case TOKEN_KEYWORD_VAR: return "KEYWORD_VAR";
        case TOKEN_KEYWORD_VOID: return "KEYWORD_VOID";
        case TOKEN_KEYWORD_WHILE: return "KEYWORD_WHILE";
        case TOKEN_IMPORT: return "IMPORT"; 
        case TOKEN_INT_LITERAL: return "INT LITERAL";
        case TOKEN_STRING_LITERAL: return "STRING LITERAL";
        case TOKEN_SEPARATOR: return "SEPARATOR";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_EQUAL: return "EQUAL (==)";
        case TOKEN_NOT_EQUAL: return "NOT EQUAL (!=)";
        case TOKEN_LESS: return "LESS (<)";
        case TOKEN_LESS_EQUAL: return "LESS EQUAL (<=)";
        case TOKEN_GREATER: return "GREATER (>)";
        case TOKEN_GREATER_EQUAL: return "GREATER EQUAL (>=)";
        case TOKEN_ASSIGN: return "ASSIGN (=)";
        case TOKEN_ADD: return "ADD (+)";
        case TOKEN_SUB: return "SUBTRACT (-)";
        case TOKEN_MUL: return "MULTIPLY (*)";
        case TOKEN_DIV: return "DIVIDE (/)";
        case TOKEN_LEFT_PAREN: return "LEFT PAREN (()";
        case TOKEN_RIGHT_PAREN: return "RIGHT PAREN ())";
        case TOKEN_LEFT_BRACE: return "LEFT BRACE ({)";
        case TOKEN_RIGHT_BRACE: return "RIGHT BRACE (})";
        case TOKEN_LEFT_BRACKET: return "LEFT BRACKET ([)";
        case TOKEN_RIGHT_BRACKET: return "RIGHT BRACKET (])";
        case TOKEN_COMMA: return "COMMA (,)";
        case TOKEN_DOT: return "DOT (.)";
        case TOKEN_COL: return "COLON (:)";
        case TOKEN_SEMICOL: return "SEMICOLON (;)";
        case TOKEN_STACK_BOTTOM: return "STACK BOTTOM";
        case TOKEN_QUOTEMARK: return "QUOTEMARK";
        case TOKEN_PIPE: return "PIPE";
        default: return "UNKNOWN";
    }
}

void pushTable(tParser **current) {
    CreateFrame(TEMPORARY_FRAME);
    CreateFrame(LOCAL_FRAME);
    ht_table_t *symtable = malloc(sizeof(ht_table_t));
    if (symtable == NULL) {
        pushError("Malloc failed.\n", 99);
    }
    initHash(symtable);
    sPush((*current)->hashStack, symtable);
    (*current)->table = symtable;
}

void popTable(tParser **current) {
    deleteTable((*current)->table);
    sPop((*current)->hashStack);
    if (sTop((*current)->hashStack) != NULL)
        (*current)->table = sTop((*current)->hashStack)->table;
    else
        (*current)->table = NULL;
}

void addBuiltin(tParser **current) {
    insertHash((*current)->hashStack, (*current)->table, "ifj.readstr", TOKEN_KEYWORD_U8, true, true, true, true, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.readi32", TOKEN_KEYWORD_I32, true, true, true, true, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.readf64", TOKEN_KEYWORD_F64, true, true, true, true, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.write", TOKEN_KEYWORD_VOID, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.i2f", TOKEN_KEYWORD_F64, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.f2i", TOKEN_KEYWORD_I32, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.string", TOKEN_KEYWORD_U8, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.length", TOKEN_KEYWORD_I32, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.concat", TOKEN_KEYWORD_U8, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.substring", TOKEN_KEYWORD_U8, true, true, true, true, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.strcmp", TOKEN_KEYWORD_U8, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.ord", TOKEN_KEYWORD_I32, true, true, true, false, false, false);
    insertHash((*current)->hashStack, (*current)->table, "ifj.chr", TOKEN_KEYWORD_U8, true, true, true, false, false, false);
}

bool checkForProlog(Token *token, FILE *file) {
    // Check if the initial token is NULL or not TOKEN_KEYWORD_CONST
    if (token == NULL || token->type != TOKEN_KEYWORD_CONST) {
        pushError("Prolog not found (check_first)", 2);
    }

    // Get the next token and validate it's an identifier with "ifj"
    token = getToken(file);
    if (token == NULL || token->type != TOKEN_IDENTIFIER || strncmp(token->param, "ifj", 3) != 0) {
        pushError("Prolog not found (check_second)", 2);
    }

    // Get the next token and validate it's an assignment
    token = getToken(file);
    if (token == NULL || token->type != TOKEN_ASSIGN) {
        pushError("Prolog not found (check_third)", 2);
    }

    // Get the next token and validate it's the keyword import
    token = getToken(file);
    if (token == NULL || token->type != TOKEN_IMPORT) {
        pushError("Prolog not found (check_fourth)", 2);
    }

    // Get the next token and validate it's a left parenthesis
    token = getToken(file);
    if (token == NULL || token->type != TOKEN_LEFT_PAREN) {
        pushError("Prolog not found (check_fifth)", 2);
    }

    // Get the next token and validate it's a string literal with "ifj24.zig"
    token = getToken(file);
    if (token == NULL || token->type != TOKEN_STRING_LITERAL || strncmp(token->param, "ifj24.zig", token->param_length) != 0) {
        pushError("Prolog not found (check_sixth)", 2);
    }

    // Get the next token and validate it's a right parenthesis
    token = getToken(file);
    if (token == NULL || token->type != TOKEN_RIGHT_PAREN) {
        pushError("Prolog not found (check_seventh)", 2);
    }

    // Get the next token and validate it's a semicolon
    token = getToken(file);
    if (token == NULL || token->type != TOKEN_SEMICOL) {
        pushError("Prolog not found (check_eighth)", 2);
    }

    // All checks passed; return true
    return true;
}

void update(tParser **current) {
    (*current)->table = sTop((*current)->hashStack)->table;
    (*current)->current_token = (*current)->next_token;
    (*current)->next_token = getToken((*current)->file);
    //if ((*current)->current_token != NULL)
    //    printf("%s, param: %s\n", tokenTypeToString((*current)->current_token->type), (*current)->current_token->param);

}

void funcDefs(tParser **current) {

    
    if ((*current)->current_token->type == TOKEN_KEYWORD_PUB) {

        update(current);
        funcDef(current);
        if ((*current)->current_token != NULL)
            funcDefs(current);

    }
}


void funcDef(tParser **current) {
    if ((*current)->current_token->type != TOKEN_KEYWORD_FN) {
        pushError("invalid function definition.\n", 2);
    }
    update(current);

    if ((*current)->current_token->type != TOKEN_IDENTIFIER) {
        pushError("invalid function definition.\n", 2);
    }
    ht_item_t *newfunc = insertHash((*current)->hashStack, (*current)->table, (*current)->current_token->param, TOKEN_KEYWORD_VOID, true, true, true, false, false, false);
    update(current);

    if ((*current)->current_token->type != TOKEN_LEFT_PAREN) {
        pushError("invalid function definition.\n", 2);
    }

    update(current);
    pushTable(current);
    param(current);

    if ((*current)->current_token->type != TOKEN_RIGHT_PAREN) {
        pushError("invalid function definition.\n", 2);
    }

    update(current);
    rtype(current, newfunc);

    if ((*current)->current_token->type != TOKEN_LEFT_BRACE) {
        pushError("invalid function definition.\n", 2);
    }

    update(current);
    stlist(current);
    popTable(current);

    if ((*current)->current_token->type != TOKEN_RIGHT_BRACE) {
        pushError("invalid function definition.\n", 2);
    }
    update(current);
}

void param(tParser **current) {

    //checks if there are any parameters in function def


    if ((*current)->current_token->type == TOKEN_IDENTIFIER) {
        ht_item_t *newvar = insertHash((*current)->hashStack,(*current)->table, (*current)->current_token->param, TOKEN_KEYWORD_VOID, false, true, true, false, false, false);
        update(current);
        
        if ((*current)->current_token->type == TOKEN_COL) {
            
            update(current);
            type(current, newvar);
            nextparam(current);

        } else {
            pushError("invalid function definition.\n", 2);
        }
    }
}

void type(tParser **current, ht_item_t *variable) {
    if ((*current)->current_token->type == TOKEN_QM) { // check if nullable
        variable->isnullable = true;
        update(current);
    }
    switch((*current)->current_token->type) {
        case TOKEN_KEYWORD_I32:
            variable->type = TOKEN_KEYWORD_I32;
            update(current);
            break;
        case TOKEN_KEYWORD_F64:
            variable->type = TOKEN_KEYWORD_F64;
            update(current);
            break;
        case TOKEN_LEFT_BRACKET:
            update(current);
            if ((*current)->current_token->type != TOKEN_RIGHT_BRACKET)
                pushError("invalid type.\n", 2);
            update(current);
            if ((*current)->current_token->type != TOKEN_KEYWORD_U8)
                pushError("invalid type.\n", 2);
            variable->type = TOKEN_KEYWORD_U8;
            update(current);
            break;
        default:
            pushError("invalid type.\n", 2);
    }
}

void nextparam(tParser **current) {
    if ((*current)->current_token->type == TOKEN_COMMA) {
        update(current);
        param(current);
    }
}

void rtype(tParser **current, ht_item_t *func) {
    if ((*current)->current_token->type == TOKEN_KEYWORD_VOID) {
        func->type = TOKEN_KEYWORD_VOID;
        update(current);
    } else {
        type(current, func);
    }
}

void stlist(tParser **current) {
    switch ((*current)->current_token->type) {
        case TOKEN_KEYWORD_CONST:
        case TOKEN_KEYWORD_VAR:
            vardecl(current);
        break;
        case TOKEN_IDENTIFIER:
            idst(current);
        break;
        case TOKEN_KEYWORD_IF:
            ifcond(current);
        break;
        case TOKEN_KEYWORD_WHILE:
            whileloop(current);
        break;
        case TOKEN_KEYWORD_RETURN:
            returnst(current);
        break;
        default:
            return;
    }
    if ((*current)->current_token->type != TOKEN_SEMICOL)
        pushError("expected semicolon.\n", 2);
    update(current);
    stlist(current);
}

void vardecl(tParser **current) {
    bool isconst = vartype(current);
    if ((*current)->current_token->type != TOKEN_IDENTIFIER)
        pushError("expected identifier.\n", 2);
    ht_item_t *newvar = insertHash((*current)->hashStack, (*current)->table, (*current)->current_token->param, TOKEN_KEYWORD_VOID, false, isconst, true, false, false, false);
    DefineVariable(LOCAL_FRAME, (*current)->current_token->param);
    update(current);
    typing(current, newvar);
    assign(current, newvar);
}

bool vartype(tParser **current) {
    if ((*current)->current_token->type == TOKEN_KEYWORD_CONST) {
        update(current);
        return true;
    }
    if ((*current)->current_token->type == TOKEN_KEYWORD_VAR) {
        update(current);
        return false;
    }
    pushError("invalid variable type.\n", 2);
    return false;
}

void typing(tParser **current, ht_item_t *variable) {
    if ((*current)->current_token->type == TOKEN_COL) {
        update(current);
        type(current, variable);
    }
}

void value_expr(tParser **current, ht_item_t *variable) {
    switch (parse_expr(current, variable)) {
        case 0:
            if (variable->type == TOKEN_KEYWORD_I32) {
                // is int
            } else if (variable->type == TOKEN_KEYWORD_VOID) {
                variable->type = TOKEN_KEYWORD_I32;
            } else {
                pushError("expected integer.\n", 7);
            }
        break;
        case 1:
            if (variable->type == TOKEN_KEYWORD_F64) {

            } else if (variable->type == TOKEN_KEYWORD_VOID) {
                variable->type = TOKEN_KEYWORD_F64;
            } else {
                pushError("expected float.\n", 7);
            }
        break;
        case 2:
            if (variable->type == TOKEN_KEYWORD_U8) {

            } else if (variable->type == TOKEN_KEYWORD_VOID) {
                variable->type = TOKEN_KEYWORD_U8;
            } else {
                pushError("expected string.\n", 7);
            }
        break;
        case 4:
            if (!variable->isnullable)
                pushError("cannot assign null to non-nullable variable.\n", 7);
        break;
        default:
            pushError("invalid type.\n", 7);
    }
}

bool builtin(tParser **current) {
    if ((*current)->current_token->type == TOKEN_IDENTIFIER && strcmp((*current)->current_token->param, "ifj") == 0) {
        update(current);
        if ((*current)->current_token->type != TOKEN_DOT)
            pushError("invalid function definition.\n", 2);
        update(current);
       return true;
    }
    return false;
}

void funccall(tParser **current, ht_item_t *func, bool isbuiltin) {
    if ((*current)->current_token->type != TOKEN_LEFT_PAREN)
        pushError("invalid function call.\n", 2);
    update(current);
    callparam(current, func, isbuiltin);
    if ((*current)->current_token->type != TOKEN_RIGHT_PAREN)
        pushError("invalid function call.\n", 2);
    update(current);
}

void callparam(tParser **current, ht_item_t *func, bool isbuiltin) {
    if (strcmp(func->name, "ifj.write") == 0)
    {
        if ((*current)->current_token->type == TOKEN_STRING_LITERAL)
        {
            PrintString((*current)->current_token->param);
        } else if ((*current)->current_token->type == TOKEN_IDENTIFIER) {
            ht_item_t *id = searchHash((*current)->table, (*current)->current_token->param);
            if (id == NULL || id->type == TOKEN_KEYWORD_VOID)
                return pushError("undefined variable.\n", 3);
            if (id->func)
                pushError("cannot use func as parameter\n", 4);
            WriteVariable(LOCAL_FRAME, (*current)->current_token->param);
        }
    }
    switch ((*current)->current_token->type) {
        case TOKEN_INT_LITERAL:
        case TOKEN_FLOAT_LITERAL:
        case TOKEN_STRING_LITERAL:
        case TOKEN_KEYWORD_NULL:
        case TOKEN_IDENTIFIER:
            update(current);
            break;
        default:
            return;
    }
    if ((*current)->current_token->type == TOKEN_COMMA) {
        update(current);
        callparam(current, func, isbuiltin);
    }
}

void idst(tParser **current) {
    bool isbuiltin = builtin(current);
    if ((*current)->current_token->type != TOKEN_IDENTIFIER)
        pushError("invalid identifier.\n", 2);
    ht_item_t *id;
    if (strcmp((*current)->current_token->param, "_") == 0) {
        id = NULL;
    } else {
        if (isbuiltin) {
            char builtinname[100] = "ifj.";
            strcat(builtinname, (*current)->current_token->param);
            id = searchHash(sBottom((*current)->hashStack)->table, builtinname);
        }
        else
            id = searchHash((*current)->table, (*current)->current_token->param);
        if (id == NULL)
            id = insertHash((*current)->hashStack, (*current)->table, (*current)->current_token->param, TOKEN_KEYWORD_VOID, true, true, false, false, false, false);
    }
    update(current);
    if ((*current)->current_token->type == TOKEN_ASSIGN) {
        if (id->func)
            if (id->defined)
                pushError("cannot assign value to function.\n", 10);
            else
                pushError("cannot assign value to undefined variable or function.\n", 3);
        assign(current, id);
    } else if ((*current)->current_token->type == TOKEN_LEFT_PAREN) {
        if (id == NULL) {
            pushError("invalid call.\n", 10);
        } else if (id->func) {
            funccall(current, id, isbuiltin);
        } else {
            pushError("cannot perform call of a variable.\n", 10);
        }
    } else
        pushError("invalid statement.\n", 2);
}


void assign(tParser **current, ht_item_t *variable) {

    if ((*current)->current_token->type != TOKEN_ASSIGN)
        pushError("expected assignment.\n", 2);
    update(current);
    // TODO func params
    bool isbuiltin = builtin(current);
    switch ((*current)->current_token->type) {
        case TOKEN_IDENTIFIER:
            ht_item_t *id;
            if (isbuiltin) {
                char builtinname[100] = "ifj.";
                strcat(builtinname, (*current)->current_token->param);
                id = searchHash(sBottom((*current)->hashStack)->table, builtinname);
            } else
                id = searchHash((*current)->table, (*current)->current_token->param);
        if (id == NULL) {
            id = insertHash((*current)->hashStack, (*current)->table, (*current)->current_token->param, TOKEN_KEYWORD_VOID, true, true, false, false, false, false);
            update(current);
            funccall(current, id, isbuiltin);
        } else if (id->func) {
            update(current);
            funccall(current, id, isbuiltin);
        } else {
            if (variable == NULL)
                pushError("cannot discard expression.\n", 4);
            value_expr(current, variable);
        }
        break;
        case TOKEN_LEFT_BRACE:
        case TOKEN_INT_LITERAL:
        case TOKEN_FLOAT_LITERAL:
        case TOKEN_STRING_LITERAL:
        case TOKEN_KEYWORD_NULL:
            if (variable == NULL)
                pushError("cannot discard expression.\n", 4);
            value_expr(current, variable);
        break;
        default:
            pushError("invalid value.\n", 2);
    }
}

void ifcond(tParser **current) {
    if ((*current)->current_token->type != TOKEN_KEYWORD_IF)
        pushError("invalid if.\n", 2);
    update(current);
    if ((*current)->current_token->type != TOKEN_LEFT_PAREN)
        pushError("invalid if.\n", 2);
    update(current);
    parse_expr(current, NULL);
    if ((*current)->current_token->type != TOKEN_RIGHT_PAREN)
        pushError("invalid if.\n", 2);
    update(current);
    nonnull_result(current);
    if ((*current)->current_token->type != TOKEN_LEFT_BRACE)
        pushError("invalid if.\n", 2);
    update(current);
    stlist(current);
    if ((*current)->current_token->type != TOKEN_RIGHT_BRACE)
        pushError("invalid if.\n", 2);
    update(current);
    if ((*current)->current_token->type != TOKEN_KEYWORD_ELSE)
        pushError("expected else.\n", 2);
    update(current); //
    if ((*current)->current_token->type != TOKEN_LEFT_BRACE)
        pushError("invalid else.\n", 2);
    update(current);
    
    stlist(current);
    if ((*current)->current_token->type != TOKEN_RIGHT_BRACE)
        pushError("invalid else.\n", 2);
    update(current);
}

void nonnull_result(tParser **current) {
    if ((*current)->current_token->type == TOKEN_PIPE) {
        update(current);
        if ((*current)->current_token->type != TOKEN_IDENTIFIER)
            pushError("expected identifier.\n", 2);
        update(current);
        if ((*current)->current_token->type != TOKEN_PIPE)
            pushError("expected pipe character.\n", 2);
        update(current);
    }
}

void whileloop(tParser **current) {

    if ((*current)->current_token->type != TOKEN_KEYWORD_WHILE)
        pushError("invalid while.\n", 2);
    update(current);
    if ((*current)->current_token->type != TOKEN_LEFT_PAREN)
        pushError("invalid while.\n", 2);
    update(current);
    parse_expr(current, NULL);
    if ((*current)->current_token->type != TOKEN_RIGHT_PAREN)
        pushError("invalid while.\n", 2);
    update(current);
    nonnull_result(current);
    
    if ((*current)->current_token->type != TOKEN_LEFT_BRACE)
        pushError("invalid while.\n", 2);
    update(current);
    stlist(current);
    if ((*current)->current_token->type != TOKEN_RIGHT_BRACE)
        pushError("invalid while.\n", 2);
    update(current);
}

void returnst(tParser **current) {
    if ((*current)->current_token->type != TOKEN_KEYWORD_RETURN)
        pushError("invalid return.\n", 2);
    update(current);
    // TODO returnval/expr
}

void start(FILE *file) {


    Token *tmp = getToken(file);

    checkForProlog(tmp, file);
    PrintHeader();

    tParser *current = malloc(sizeof(tParser));
    tStack *hashStack = malloc(sizeof(*hashStack));
    if (hashStack == NULL) {
        pushError("Malloc failed.\n", 99);
    }
    sInit(hashStack);
    current->hashStack = hashStack;
    pushTable(&current);
    current->table->global = true;
    addBuiltin(&current);
    current->current_token = getToken(file);
    current->next_token = getToken(file);
    current->file = file;

    funcDefs(&current);

}
