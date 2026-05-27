/**
 * @file parser.c
 * @brief Implementation of the parser
 *
 * This file contains functions to parse input by getting tokens
 * from the scanner. While parsing, an ast is being built and basic semantics is checked.
 *
 * @author 
 *  - Eliška Krejčíková (xkrejce00)
 *  - Adam Kinzel (xkinzea00)
 */


#define _POSIX_C_SOURCE 200809L
#include "parser.h"
#include "scanner.h"
#include "error_codes.h"
#include "ast.h"
#include "psa.h"
#include "symtable.h"
#include "semantics.h"

#include <stdio.h>
#include <string.h>



// this function is used instead of getToken, because scanner gives me MULTILINE_COMMENT as a token
// that should be taken as a whitespace and be ignored
Token *getTokenSyntax(){
    Token *token = getToken();
    while(token->type == TOKEN_MULTILINE_COMMENT) {
        free(token);
        token = getToken();
    }
    return token;
}

// function skips all eols and both comment types
// so it returns the next non-eol/comment token
Token *skip_eol_comment() {
    // without multiline
    Token *token = getTokenSyntax();
    while(token->type == TOKEN_ONELINE_COMMENT || token->type == TOKEN_EOL) {
        if(token->type == TOKEN_ONELINE_COMMENT) {
            free(token);
            // second token is eol
            token = getToken();
        }
        free(token);
        token = getTokenSyntax();
    }

    return token;
}

void memory_dealloc(AstNode *ast, symtable_stack_t *symtable_stack) {
    deleteNodeList(ast);
    
    while(symtable_stack->num_of_scopes > 0) {
        symbol_t **popped_symtable = pop_from_stack(symtable_stack);
        delete_symtable(popped_symtable);
        free(popped_symtable);
    }
}

// Skips eol and returns the next token
// There has to minimal one EOL
// 1 - N EOL
Token *eol_rule() {
    if(check_token(TOKEN_EOL, true) == NULL) {
       exit(SYNTAX_ERROR);
    }

    return eol_rule_next();
}
// 0 - N EOL
Token* eol_rule_next() {
    Token *token = getTokenSyntax();
    if(token->type == TOKEN_EOL) {
        free(token);
        token = eol_rule_next();
    } else if(token->type == TOKEN_ONELINE_COMMENT) {
        free(token);
        // throwing away the eol after comment as well
        token = getTokenSyntax();
        free(token);
        token = eol_rule_next();
    }
    return token;
}

bool check_token_by_str_value( char *token_str, token_type t_type, bool comment_eol_skip) {
    Token *token;
    if(comment_eol_skip) {
        token = skip_eol_comment();
    } else {
        token = getTokenSyntax();
    }
    //also skip eols after comments

    if(token->type == t_type) {
        if (strcmp(token->token_str, token_str) == 0) {
            free(token);
            return true;
        } else {
            free(token);
            return false;
        }
    }
    return false;
}

Token* check_token(token_type t_type, bool comment_eol_skip) {
    // multiline = whitespace
    Token *token;
    if(comment_eol_skip) {
        // have to be careful here, eol could be skipped when oneline comment is skipped
        if(t_type == TOKEN_EOL) {
            token = getTokenSyntax();
            if(token->type == TOKEN_ONELINE_COMMENT) {
                free(token);
                token = getTokenSyntax();
            }
        } else {
            token = skip_eol_comment();
        }
    } else {
        token = getTokenSyntax();
    }
    if (token->type == t_type) {
        return token;
    } else {
        return NULL;
    }
}


void program(AstNode *ast, symtable_stack_t *symtable_stack) {
    
    prolog();

    symbol_t **symtable = create_new_symtable();
    if(symtable == NULL) {
        exit(INTERNAL_ERROR);
    }
    push_to_stack(symtable_stack, symtable);

    classDefinition(ast, symtable_stack);

    if (symtable_stack->top != NULL) {
        symtable = symtable_stack->top->symbol_table;
    } else {
        exit(INTERNAL_ERROR);
    }

    //semantic checks
    if(check_main(symtable) == false) {
        memory_dealloc(ast, symtable_stack);
        exit(SEMANTIC_NONDEF_ERROR);
    }
    if(check_wrong_param_cnt_fun_call(symtable)) {
        memory_dealloc(ast, symtable_stack);
        exit(SEMANTIC_PARAM_ERROR);
    }

    if(!check_nondefined(symtable)) {
        memory_dealloc(ast, symtable_stack);
        exit(SEMANTIC_NONDEF_ERROR);
    }

    if(check_token(TOKEN_EOF, true) == NULL) {
        memory_dealloc(ast, symtable_stack);
        exit(SYNTAX_ERROR);
    }
}

// RULE: 2
void prolog() {
    //skips comments/eols before prolog
    if (false == check_token_by_str_value("import", TOKEN_LOCAL_IDENTIFIER, true)) {
       exit(SYNTAX_ERROR);
    }
    Token *token = eol_rule_next();
    if(token->type != TOKEN_LITERAL_STRING || strcmp(token->token_str, "ifj25") != 0) {
       exit(SYNTAX_ERROR);
    }

    free(token);
    token = eol_rule_next();
    if(token->type != TOKEN_LOCAL_IDENTIFIER || strcmp(token->token_str, "for") != 0) {
       exit(SYNTAX_ERROR);
    }

    free(token);
    token = eol_rule_next();
    if(token->type != TOKEN_KEYWORD_IFJ) {
       exit(SYNTAX_ERROR);
    }

    free(token);
    //INFO: eol_rule called in class def
}

// RULE: 3
void classDefinition(AstNode *ast, symtable_stack_t *symtable_stack) {
    // has to be minimal one eol
    Token *token = eol_rule();
    if(token->type != TOKEN_KEYWORD_CLASS) {
        memory_dealloc(ast, symtable_stack);
        exit(SYNTAX_ERROR);
    }

    if(check_token_by_str_value("Program", TOKEN_LOCAL_IDENTIFIER, false) == false) {
        memory_dealloc(ast, symtable_stack);
        exit(SYNTAX_ERROR);
    }

    if(check_token(TOKEN_CBRACKET_OPEN, false) == NULL) {
        memory_dealloc(ast, symtable_stack);
        exit(SYNTAX_ERROR);
    }
    free(token);

    functionList(ast, symtable_stack);
    // INFO: the cbracket token is being checked in functionList
}

// RULE: 4, 5
void functionList(AstNode *ast, symtable_stack_t *symtable_stack) {
    // either epsilon or the def + list
    // comments and eols before function def or after
    Token *token = eol_rule();
    if(token->type == TOKEN_CBRACKET_CLOSE) {
        // RULE: 5
        // this the ending bracket from class def
        // successfully finished the whole program
        return;
    } else {
        // RULE: 4
        functionDefinition(token, ast, symtable_stack);
        functionList(ast, symtable_stack);
    }
}

// RULE: 6
void functionDefinition(Token *token, AstNode *ast, symtable_stack_t *symtable_stack) {
    // INFO: comment already skipped in functionList
    if(token->type != TOKEN_KEYWORD_STATIC) {
        memory_dealloc(ast, symtable_stack);
        exit(SYNTAX_ERROR);
    }

    AstNode* funcNode = createNode(ast, NT_FUNC, true);

    if(functionHeader(funcNode, symtable_stack)) {
        // true = rule 22: getter
        Token *token = malloc(sizeof(Token));
        if(token == NULL) {
            memory_dealloc(ast, symtable_stack);
            exit(INTERNAL_ERROR);
        }
        token->type = TOKEN_CBRACKET_OPEN;

        block(token, funcNode, symtable_stack);
    } else {
        // setter or normal function
        block(NULL, funcNode, symtable_stack);
    }

    symbol_t **popped_symtable = pop_from_stack(symtable_stack);

    delete_symtable(popped_symtable);
    free(popped_symtable);
}

// RULE: 7
bool functionHeader(AstNode *funcNode, symtable_stack_t *symtable_stack) {
    Token *token;
    if((token = check_token(TOKEN_LOCAL_IDENTIFIER, false)) == NULL) {
        memory_dealloc(funcNode, symtable_stack);
        exit(SYNTAX_ERROR);
    }        

    //AST: token->token_str is function name
    funcNode->func.name = strdup(token->token_str);
    
    int param_cnt = 0;
    if(functionHeaderTail(token->token_str, funcNode, symtable_stack, &param_cnt)) {
        
        return true;
    }

    return false;
}

// RULE: 8, 9, 10
bool functionHeaderTail(char *token_id, AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt) {
    Token *token = getTokenSyntax();

    if(token->type == TOKEN_BRACKET_OPEN) {
        // RULE: 8
        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        parameterList(funcNode, symtable_stack, param_cnt);

        // need to go one symtable before and change the param count
        new_symtable = pop_from_stack(symtable_stack);

        symbol_t *function_symbol = create_new_function_symbol(token_id, *param_cnt, T_FUNC, true);
        if(function_symbol == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }

        // int return_val = insert_symbol_to_symtable(symtable_stack->top->symbol_table, function_symbol);
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, function_symbol) == 1) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        push_to_stack(symtable_stack, new_symtable);


        return false;
        // INFO: closing bracket should be checked in parameterList

    } else if(token->type == TOKEN_EQUALS) {
        // RULE: 9
        // setter
        symbol_t *new_function_symbol = create_new_function_symbol(token_id, 1, T_SETTER, true);
        if(new_function_symbol == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }

        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, new_function_symbol) == 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }
        Token *token_after_possile_eol = eol_rule_next();
        if(token_after_possile_eol->type != TOKEN_BRACKET_OPEN) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SYNTAX_ERROR);
        }

        free(token_after_possile_eol);
        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);


        Token *second_token = getTokenSyntax();
        if(second_token->type != TOKEN_LOCAL_IDENTIFIER) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        symbol_t *param_symbol = create_new_variable_symbol(second_token->token_str, false, TYPE_PARAM);
        if(param_symbol == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }

        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, param_symbol) == 1) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        // AST: this is setter

        if(check_token(TOKEN_BRACKET_CLOSE, false) == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        return false;
    } else if (token->type == TOKEN_CBRACKET_OPEN) {
        // RULE: 10
        // this is getter

        symbol_t *new_function_symbol = create_new_function_symbol(token_id, 0, T_GETTER, true);
        if(new_function_symbol == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, new_function_symbol) == 1) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        return true;
    } else {
        memory_dealloc(funcNode, symtable_stack);
        exit(SYNTAX_ERROR);
    }
}

// RULE: 11, 12
void parameterList(AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt) {
    Token *token = getTokenSyntax();
    if(token->type == TOKEN_LOCAL_IDENTIFIER) {
        // RULE: 11
        symbol_t *param_symbol = create_new_variable_symbol(token->token_str, false, TYPE_PARAM);
        if(param_symbol == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }
        if(insert_symbol_to_symtable(symtable_stack->top->symbol_table, param_symbol)== 1) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        AstNode *param = createNode(funcNode, NT_PARAM, true);
        param->param.name = strdup(token->token_str);
        funcNode->func.params = param;
        funcNode->func.paramCnt = 1;
        free(token);
        (*param_cnt)++;

        parameterListTail(funcNode, symtable_stack, param_cnt);
    } else if(token->type == TOKEN_BRACKET_CLOSE) {
        // RULE: 12
        // this is eps
        // no params

        funcNode->func.paramCnt = 0;
        funcNode->func.params = NULL;
        return;
    } else {
        memory_dealloc(funcNode, symtable_stack);
        exit(SYNTAX_ERROR);
    }
}

// RULE: 13, 14
void parameterListTail(AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt) {
    Token *token = getTokenSyntax();

    if(token->type == TOKEN_COMMA) {
        // RULE: 13
        Token *paramToken = eol_rule_next();
        if(paramToken->type != TOKEN_LOCAL_IDENTIFIER) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SYNTAX_ERROR);
        }

        symbol_t *param_symbol = create_new_variable_symbol(paramToken->token_str, false, TYPE_PARAM);
        if(param_symbol == NULL) {
            memory_dealloc(funcNode, symtable_stack);
            exit(INTERNAL_ERROR);
        }

        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, param_symbol) == 1) {
            memory_dealloc(funcNode, symtable_stack);
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        AstNode *lastParam = funcNode->func.params;
        while(lastParam->next != NULL) {
            lastParam = lastParam->next;
        }
        AstNode *param = createNode(lastParam, NT_PARAM, false);
        param->param.name = strdup(paramToken->token_str);
        funcNode->func.paramCnt++;
        free(paramToken);

        (*param_cnt)++;

        parameterListTail(funcNode, symtable_stack, param_cnt);

    } else if(token->type == TOKEN_BRACKET_CLOSE) {
        // RULE: 14
        // epsilon
        //  ) form parameterList
        return;
    } else {
        memory_dealloc(funcNode, symtable_stack);
        exit(SYNTAX_ERROR);
    }
}

// RULE: 15
void block(Token *first_token, AstNode *parentNode, symtable_stack_t *symtable_stack) {
    
    if(first_token == NULL) {
        if(check_token(TOKEN_CBRACKET_OPEN, false) == NULL) {
            memory_dealloc(parentNode, symtable_stack);
            exit(SYNTAX_ERROR);
        }
    } else if (first_token->type != TOKEN_CBRACKET_OPEN) {
        memory_dealloc(parentNode, symtable_stack);
        exit(SYNTAX_ERROR);
    }
    AstNode *blockNode = createNode(parentNode, NT_BLOCK, true);

    Token *token = eol_rule();
    statementList(token, blockNode, symtable_stack);

    // INFO: the closing bracket is being checked in statementList
}

// RULE: 16, 17
void statementList(Token *token, AstNode *ast, symtable_stack_t *symtable_stack) {
    // comments on its own line inside block skipped
    
    if(token->type == TOKEN_CBRACKET_CLOSE) {
        // RULE: 17
        // epsilon
        // this is the ending bracket from block
        return;
    } else {
        // RULE: 16
        Token *token_from_statement = statement(token, ast, symtable_stack);

        statementList(token_from_statement, ast, symtable_stack);
    }
}

// RULE: 18, 19, 20 ,21 ,22, 23
Token* statement(Token *first_token, AstNode *ast, symtable_stack_t *symtable_stack) {
    
    Token *token = NULL;
    if(first_token->type == TOKEN_KEYWORD_VAR) {
        // 18
        token = getTokenSyntax();

        if(token->type != TOKEN_LOCAL_IDENTIFIER) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }

        if(check_var_redefined(symtable_stack, token->token_str)) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        AstNode *varDeclNode = createNode(ast, NT_VAR, true);
        varDeclNode->var.name = strdup(token->token_str);

        symbol_t *new_variable_symbol = create_new_variable_symbol(token->token_str, false, TYPE_UNKNOWN);
        if(new_variable_symbol == NULL) {
            exit(INTERNAL_ERROR);
        }
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, new_variable_symbol) == 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        free(token);
        token = NULL;

    } else if (first_token->type == TOKEN_KEYWORD_IF) {
        // 19
        token = getTokenSyntax();
        if(token->type != TOKEN_BRACKET_OPEN) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        free(token);
        token = NULL;

        AstNode *ifNode = createNode(ast, NT_IF_ELSE, true);
        ifNode->ifelse.condition = NULL;

        Token *_token = getTokenSyntax();
        Token *last_token = expression(_token, symtable_stack, ifNode);

        if(last_token->type != TOKEN_BRACKET_CLOSE) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }

        AstNode *trueBlock = createNode(ifNode, NT_BLOCK, true);

        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        block(NULL, trueBlock, symtable_stack);

        symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
        delete_symtable(symtable_from_stack);
        free(symtable_from_stack);

        ifNode->ifelse.true_body = trueBlock->block.firstStmt;

        //else not needed
        Token *forwardCheck = getTokenSyntax();
        if (forwardCheck->type == TOKEN_KEYWORD_ELSE) { 
            // RULE: 43
            AstNode *falseBlock = createNode(ifNode, NT_BLOCK, true);

            symbol_t **new_symtable = create_new_symtable();
            if(new_symtable == NULL) {
                exit(INTERNAL_ERROR);
            }
            push_to_stack(symtable_stack, new_symtable);


            block(NULL, falseBlock, symtable_stack);

            symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
            delete_symtable(symtable_from_stack);
            free(symtable_from_stack);

            ifNode->ifelse.false_body = falseBlock->block.firstStmt;
            ifNode->ifelse.else_present = true;
        } else if(forwardCheck->type == TOKEN_EOL) {
            // RULE: 44
            ifNode->ifelse.else_present = false;
            token = malloc(sizeof(Token));
            if(token == NULL) {
                exit(INTERNAL_ERROR);
            }
            token->type = TOKEN_EOL;
            
        } else {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        free(forwardCheck);


    } else if (first_token->type == TOKEN_KEYWORD_WHILE) {
        // RULE: 20
        token = getTokenSyntax();
        if(token->type != TOKEN_BRACKET_OPEN) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        free(token);
        token = NULL;

        AstNode *whileNode = createNode(ast, NT_WHILE, true);
        whileNode->while_stmt.condition = NULL;

        Token *_token = getTokenSyntax();
        Token *last_token = expression(_token, symtable_stack, whileNode); 
        if(last_token->type != TOKEN_BRACKET_CLOSE) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        AstNode *whileBlock = createNode(whileNode, NT_BLOCK, true);
        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);
        block(NULL, whileBlock, symtable_stack);

        symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
        delete_symtable(symtable_from_stack);
        free(symtable_from_stack);

        whileNode->while_stmt.body = whileBlock->block.firstStmt;

    } else if (first_token->type == TOKEN_KEYWORD_RETURN) {
        // RULE: 21
        Token *_token = getTokenSyntax();
        AstNode *returnNode = createNode(ast, NT_RETURN, true);
        token = expression(_token, symtable_stack, returnNode);

    } else if (first_token->type == TOKEN_LOCAL_IDENTIFIER) {
        // RULE: 23
        token = statementTail(symtable_stack, ast, first_token);

    } else if(first_token->type == TOKEN_GLOBAL_IDENTIFIER){
        // RULE: 24

        AstNode *assignNode = createNode(ast, NT_ASSIGN, true);
        assignNode->assign.left = createNode(assignNode, NT_VAR, true);
        assignNode->assign.left->var.name = strdup(first_token->token_str);
        assignNode->assign.right = NULL;

        if(check_token(TOKEN_EQUALS, false) == NULL) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }

        Token *token_after_eol = eol_rule_next();

        token = expression(token_after_eol, symtable_stack, assignNode);

        var_type_t var_type = TYPE_UNKNOWN;
        if (assignNode->expr_type != DATA_TYPE_UNKNOWN) {
            var_type = data_type_to_var_type(assignNode->expr_type);
        }


        // Check if the global variable already exists in the symbol table
        char *key = create_key(first_token->token_str, 0, T_UNKNOWN);
        symbol_t *existing_symbol = get_symbol_from_upper_scopes(symtable_stack, key);
        free(key);

        if (!existing_symbol) {
            //Global variable doesn't exist yet, create it
            symbol_t *new_global = create_new_variable_symbol(first_token->token_str, true, var_type);
            if (new_global) {
                insert_into_certain_scope(symtable_stack, new_global, 1);
            }

        } else {
            // Variable exists, just update its type
            if (var_type != TYPE_UNKNOWN) {
                change_variable_type(symtable_stack, first_token->token_str, var_type);
            }
        }

    
    } else if (first_token->type == TOKEN_KEYWORD_IFJ) {
        // RULE: 25
        //built in functions
        if(check_token(TOKEN_DOT, false) == NULL) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }

        token = eol_rule_next();
        if(token->type != TOKEN_LOCAL_IDENTIFIER) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        if(check_token(TOKEN_BRACKET_OPEN, false) == NULL) {
            memory_dealloc(ast, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        int function_call_args = 0;
        argumentList(symtable_stack, &function_call_args, NULL);

        int builtin_check =check_built_in_function_params(token->token_str, function_call_args);

        if (builtin_check == 1) {
            memory_dealloc(ast, symtable_stack);
            exit(SEMANTIC_PARAM_ERROR);
        } else if (builtin_check == 2) {
            memory_dealloc(ast, symtable_stack);
            exit(SEMANTIC_NONDEF_ERROR);
        }
        free(token);
        token = NULL;

        // INFO: the closing bracket is being checking in argumentListTail
    } else {
        // RULE: 22
        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            memory_dealloc(ast, symtable_stack);
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        block(first_token, ast, symtable_stack);

        symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
        delete_symtable(symtable_from_stack);
        free(symtable_from_stack);

    }
    
    if(token != NULL) {
        //checking eol in token from expr or return
        if(token->type == TOKEN_EOL) {
            free(token);
            token = eol_rule_next();
        } else if(token->type == TOKEN_ONELINE_COMMENT) {
            free(token);
            token = getTokenSyntax();
            free(token);
            token = eol_rule_next();
        }
        return token;

    } else {
        Token *token_after = eol_rule();
        return token_after;
    }
}

// RULE: 26, 27
Token *statementTail(symtable_stack_t *symtable_stack, AstNode *ast, Token *first_token) {
    Token *token = getTokenSyntax();
    //the eols are checked in the end of statement
    if(token->type == TOKEN_EQUALS) {
        // RULE: 26
        check_variable(symtable_stack, first_token->token_str);

        AstNode *assignNode = createNode(ast, NT_ASSIGN, true);
        assignNode->assign.left = createNode(assignNode, NT_VAR, true);
        assignNode->assign.left->var.name = strdup(first_token->token_str);

        assignNode->assign.right = NULL;

        Token *token_after_eol = eol_rule_next();

        Token *token = expression(token_after_eol, symtable_stack, assignNode);

        //Update variable type in symbol table based on epxression result
        if (assignNode->expr_type != DATA_TYPE_UNKNOWN) {

            var_type_t var_type = data_type_to_var_type(assignNode->expr_type);
            change_variable_type(symtable_stack, first_token->token_str, var_type);

        }
        return token;

    } else if(token->type == TOKEN_BRACKET_OPEN) {
        // RULE: 27

        AstNode *callNode = createNode(ast, NT_FUNC_CALL, true);
        callNode->call.name = token->token_str;

        int function_call_args = 0; 

        argumentList(symtable_stack, &function_call_args, callNode);

        symbol_t *function_symbol = create_new_function_symbol(first_token->token_str, function_call_args, T_FUNC, false);
        insert_into_certain_scope(symtable_stack, function_symbol, 1);

        return NULL;
        //the closing bracket check in argumentListTail/argumentList

    } else {
        memory_dealloc(ast, symtable_stack);
        exit(SYNTAX_ERROR);
    }
}

// RULE: 28,29
void argumentList(symtable_stack_t *symtable_stack, int *function_call_args, AstNode *callNode) {
    // term will deal with epsilon - rule 33

    Token *token = getTokenSyntax();
    if(!term(token, symtable_stack, callNode)) {
        // no argument
        if(token->type == TOKEN_BRACKET_CLOSE) {
            // RULE: 28
            return;
        }
    }

    // RULE: 29
    (*function_call_args)++;
    argumentListTail(symtable_stack, function_call_args, callNode);

}

// RULE: 32, 33, 34
bool term(Token *token, symtable_stack_t *symtable_stack, AstNode *callNode) {
    if(token->type == TOKEN_LOCAL_IDENTIFIER) {
        // RULE: 32
        check_variable(symtable_stack, token->token_str);
        AstNode *astLocIdTerm = createNode(callNode, NT_VAR, true);
        astLocIdTerm->var.name = token->token_str;

        return true;
    } else if(token->type == TOKEN_GLOBAL_IDENTIFIER) {
        // RULE: 33
        AstNode *astGlobIdTerm = createNode(callNode, NT_VAR, true);
        astGlobIdTerm->var.name = token->token_str;

        return true;
    } else {
        // RULE: 34
        if(literal(token)) {
            createNode(callNode, NT_LITERAL, true);
            return true;
        } else {
            return false;
        }
    }
}

// RULE: 30, 31
void argumentListTail(symtable_stack_t *symtable_stack, int *function_call_args, AstNode *callNode) {
    Token *token = getTokenSyntax();
    if(token->type == TOKEN_COMMA) {
        // RULE: 30

        Token *next = eol_rule_next();
        if(!term(next, symtable_stack, callNode)) {
            memory_dealloc(callNode, symtable_stack);
            exit(SYNTAX_ERROR);
        }
        (*function_call_args)++;

        argumentListTail(symtable_stack, function_call_args, callNode);

        free(token); free(next);

    } else if(token->type == TOKEN_BRACKET_CLOSE) {
        // RULE: 31
        return;
    } else {
        memory_dealloc(callNode, symtable_stack);
        exit(SYNTAX_ERROR);
    }
}

// RULE: 35, 36, 37, 38, 39
bool literal(Token *token) {
    if (token->type == TOKEN_LITERAL_INT) {
        return true;
    } else if(token->type == TOKEN_LITERAL_FLOAT) {
        return true;
    } else if(token->type == TOKEN_LITERAL_STRING) {
        return true;
    } else if(token->type == TOKEN_LITERAL_HEX) {
        return true;
    } else if(token->type == TOKEN_KEYWORD_NULL_SMALL) {
        return true;
    }
    return false;
}


void parse(AstNode *ast, symtable_stack_t *symtable_stack) {
    program(ast, symtable_stack);
}
