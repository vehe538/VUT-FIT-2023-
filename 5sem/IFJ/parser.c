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
Token *getTokenSyntax(char * input) {
    Token *token = getToken(input);
    while(token->type == TOKEN_MULTILINE_COMMENT) {
        free(token);
        token = getToken(input);
    }
    return token;
}

// function skips all eols and both comment types
// so it returns the next non-eol/comment token
Token *skip_eol_comment(char *input) {
    // without multiline
    Token *token = getTokenSyntax(input);
    while(token->type == TOKEN_ONELINE_COMMENT || token->type == TOKEN_EOL) {
        if(token->type == TOKEN_ONELINE_COMMENT) {
            free(token);
            // second token is eol
            token = getToken(input);
        }
        free(token);
        token = getTokenSyntax(input);
    }

    return token;
}

void memory_dealloc() {
    // ast, symboltable, token, stack?
}

// Skips eol and returns the next token
// There has to minimal one EOL
// 1 - N EOL
Token *eol_rule(char *input) {
    if(check_token(input, TOKEN_EOL, true) == NULL) {
        exit(SYNTAX_ERROR);
    }

    return EolRuleNext(input);
}
// 0 - N EOL
Token* EolRuleNext(char *input) {
    Token *token = getTokenSyntax(input);
    if(token->type == TOKEN_EOL) {
        free(token);
        token = EolRuleNext(input);
    } else if(token->type == TOKEN_ONELINE_COMMENT) {
        free(token);
        // throwing away the eol after comment as well
        token = getTokenSyntax(input);
        free(token);
        token = EolRuleNext(input);
    }
    return token;
}

bool check_token_by_str_value(char *input,  char *token_str, token_type t_type, bool comment_eol_skip) {
    Token *token;
    if(comment_eol_skip) {
        token = skip_eol_comment(input);
    } else {
        token = getTokenSyntax(input);
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

Token* check_token(char *input, token_type t_type, bool comment_eol_skip) {
    // multiline = whitespace
    Token *token;
    if(comment_eol_skip) {
        // have to be careful here, eol could be skipped when oneline comment is skipped
        if(t_type == TOKEN_EOL) {
            token = getTokenSyntax(input);
            if(token->type == TOKEN_ONELINE_COMMENT) {
                free(token);
                token = getTokenSyntax(input);
            }
        } else {
            token = skip_eol_comment(input);
        }
    } else {
        token = getTokenSyntax(input);
    }
    if (token->type == t_type) {
        return token;
    } else {
        return NULL;
    }
}

/*
Functions for parsing 
*/


// RULE: 1
int program(char *input, AstNode *ast, symtable_stack_t *symtable_stack) {
    
    prolog(input, symtable_stack);
    symbol_t **symtable = create_new_symtable();
    if(symtable == NULL) {
        exit(INTERNAL_ERROR);
    }
    push_to_stack(symtable_stack, symtable);

    classDefinition(input, ast, symtable_stack);

    symtable = pop_from_stack(symtable_stack);

    if(check_main(symtable) == false) {
        exit(SEMANTIC_NONDEF_ERROR);
    }

    if(!check_nondefined(symtable)) {
        exit(SEMANTIC_NONDEF_ERROR);
    }


    Token *token = getTokenSyntax(input);
    // TODO: EOF is not being detected
    // if(check_token(input, TOKEN_EOF, true) == NULL) {
    //     exit(SYNTAX_ERROR);
    // }
    printf("YOU ARE A WIZARD, HARRY\n");
    return 0;
}

// RULE: 2
void prolog(char *input, symtable_stack_t *symtable_stack) {
    //skips comments/eols before prolog
    if (false == check_token_by_str_value(input, "import", TOKEN_LOCAL_IDENTIFIER, true)) {
        exit(SYNTAX_ERROR);
    }

    if(false == check_token_by_str_value(input, "ifj25", TOKEN_LITERAL_STRING, false)) {
        exit(SYNTAX_ERROR);
    }

    if(false == check_token_by_str_value(input, "for", TOKEN_LOCAL_IDENTIFIER, false)) {
        exit(SYNTAX_ERROR);
    }

    if(check_token(input, TOKEN_KEYWORD_IFJ, false) == NULL) {
        exit(SYNTAX_ERROR);
    }

    //INFO: eol_rule called in class def
}

// RULE: 3
void classDefinition(char *input, AstNode *ast, symtable_stack_t *symtable_stack) {
    // has to be minimal one eol

    Token *token = eol_rule(input);
    if(token->type != TOKEN_KEYWORD_CLASS) {
        exit(SYNTAX_ERROR);
    }

    if(check_token_by_str_value(input, "Program", TOKEN_LOCAL_IDENTIFIER, false) == false) {
        exit(SYNTAX_ERROR);
    }

    if(check_token(input, TOKEN_CBRACKET_OPEN, false) == NULL) {
        exit(SYNTAX_ERROR);
    }
    free(token);

    functionList(input, ast, symtable_stack);
    // INFO: the cbracket token is being checked in functionList
}

// RULE: 4, 5
void functionList(char *input, AstNode *ast, symtable_stack_t *symtable_stack) {
    // either epsilon or the def + list
    // comments and eols before function def or after
    Token *token = eol_rule(input);
    if(token->type == TOKEN_CBRACKET_CLOSE) {
        // RULE: 5
        // this the ending bracket from class def
        // successfully finished whole program
        return;
    } else {
        // RULE: 4
        functionDefinition(input, token, ast, symtable_stack);

        functionList(input, ast, symtable_stack);
    }
}

// RULE: 6
void functionDefinition(char *input, Token *token, AstNode *ast, symtable_stack_t *symtable_stack) {
    // INFO: comment already skipped in functionList
    if(token->type != TOKEN_KEYWORD_STATIC) {
        exit(SYNTAX_ERROR);
    }

    AstNode* funcNode = createNode(ast, NT_FUNC, true);
    //new symtable create in functionheader
    if(functionHeader(input, funcNode, symtable_stack)) {
        // true = rule 22: getter
        Token *token = malloc(sizeof(Token));
        if(token == NULL) {
            exit(INTERNAL_ERROR);
        }
        token->type = TOKEN_CBRACKET_OPEN;

        block(input, token, funcNode, symtable_stack);
    } else {
        // setter or normal function
        block(input, NULL, funcNode, symtable_stack);
    }

    symbol_t **popped_symtable = pop_from_stack(symtable_stack);

    delete_symtable(popped_symtable);
    free(popped_symtable);
}

// RULE: 7
bool functionHeader(char *input, AstNode *funcNode, symtable_stack_t *symtable_stack) {
    Token *token;
    if((token = check_token(input, TOKEN_LOCAL_IDENTIFIER, false)) == NULL) {
        exit(SYNTAX_ERROR);
    }        

    //AST: token->token_str is function name
    //funcNode->func.name = strdup(token->token_str);
    funcNode->func.name = malloc(sizeof(token->token_str));
    strcpy(funcNode->func.name, token->token_str);

    int param_cnt = 0;
    if(functionHeaderTail(input, token->token_str, funcNode, symtable_stack, &param_cnt)) {
        
        return true;
    }

    return false;
}

// RULE: 8, 9, 10
bool functionHeaderTail(char *input, char *token_id, AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt) {
    // AST: use token_id as function name
    Token *token = getTokenSyntax(input);
    if(token->type == TOKEN_BRACKET_OPEN) {
        // RULE: 8

        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);
        parameterList(input, funcNode, symtable_stack, param_cnt);
        // need to go one symtable before and change the param count
        new_symtable = pop_from_stack(symtable_stack);

        symbol_t **function_symbol = create_new_function_symbol(token_id, *param_cnt, T_FUNC, true);
        if(function_symbol == NULL) {
            exit(INTERNAL_ERROR);
        }
        // int return_val = insert_symbol_to_symtable(symtable_stack->top->symbol_table, function_symbol);
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, function_symbol) == 1) {
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
            exit(INTERNAL_ERROR);
        }
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, new_function_symbol) == 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }
        Token *token_after_possile_eol = EolRuleNext(input);
        if(token_after_possile_eol->type != TOKEN_BRACKET_OPEN) {
            exit(SYNTAX_ERROR);
        }

        free(token_after_possile_eol);
        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);


        Token *second_token = getTokenSyntax(input);
        if(second_token->type != TOKEN_LOCAL_IDENTIFIER) {
            exit(SYNTAX_ERROR);
        }
        symbol_t *param_symbol = create_new_variable_symbol(second_token->token_str, false, TYPE_PARAM);
        if(param_symbol == NULL) {
            exit(INTERNAL_ERROR);
        }

        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, param_symbol) == 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        // AST: this is setter

        if(check_token(input, TOKEN_BRACKET_CLOSE, false) == NULL) {
            exit(SYNTAX_ERROR);
        }
        return false;
    } else if (token->type == TOKEN_CBRACKET_OPEN) {
        // RULE: 10
        // this is getter
        symbol_t *new_function_symbol = create_new_function_symbol(token_id, 0, T_GETTER, true);
        if(new_function_symbol == NULL) {
            exit(INTERNAL_ERROR);
        }
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, new_function_symbol) == 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        return true;
    } else {
        exit(SYNTAX_ERROR);
    }
}

// RULE: 11, 12
void parameterList(char *input, AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt) {
    Token *token = getTokenSyntax(input);
    if(token->type == TOKEN_LOCAL_IDENTIFIER) {
        // RULE: 11
        symbol_t *param_symbol = create_new_variable_symbol(token->token_str, false, TYPE_PARAM);
        if(param_symbol == NULL) {
            exit(INTERNAL_ERROR);
        }
        if(insert_symbol_to_symtable(symtable_stack->top->symbol_table, param_symbol)== 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        AstNode *param = createNode(funcNode, NT_PARAM, true);
        //param->param.name = strdup(token->token_str);
        param->param.name = malloc(sizeof(token->token_str));
        strcpy(param->param.name, token->token_str);
        
        funcNode->func.params = param;
        funcNode->func.paramCnt = 1;
        free(token);
        (*param_cnt)++;
        parameterListTail(input, funcNode, symtable_stack, param_cnt);
    } else if(token->type == TOKEN_BRACKET_CLOSE) {
        // RULE: 12
        // this is eps
        // no params

        funcNode->func.paramCnt = 0;
        funcNode->func.params = NULL;
        return;
    } else {
        exit(SYNTAX_ERROR);
    }
}

// RULE: 13, 14
void parameterListTail(char *input, AstNode *funcNode, symtable_stack_t *symtable_stack, int *param_cnt) {
    Token *token = getTokenSyntax(input);

    if(token->type == TOKEN_COMMA) {
        // RULE: 13
        Token *paramToken = EolRuleNext(input);
        if(paramToken->type != TOKEN_LOCAL_IDENTIFIER) {
            exit(SYNTAX_ERROR);
        }
        symbol_t *param_symbol = create_new_variable_symbol(paramToken->token_str, false, TYPE_PARAM);
        if(param_symbol == NULL) {
            exit(INTERNAL_ERROR);
        }
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, param_symbol) == 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        // AST: this is param, token->token_str is param name
        AstNode *lastParam = funcNode->func.params;
        while(lastParam->next != NULL) {
            lastParam = lastParam->next;
        }
        AstNode *param = createNode(lastParam, NT_PARAM, false);
        //param->param.name = strdup(paramToken->token_str);
        param->param.name = malloc(sizeof(token->token_str));
        strcpy(param->param.name, token->token_str);

        funcNode->func.paramCnt++;
        free(paramToken);


        (*param_cnt)++;

        parameterListTail(input, funcNode, symtable_stack, param_cnt);

    } else if(token->type == TOKEN_BRACKET_CLOSE) {
        // RULE: 14
        // epsilon
        //  ) form parameterList
        return;
    } else {
        exit(SYNTAX_ERROR);
    }
}

// RULE: 15
void block(char *input, Token *first_token, AstNode *parentNode, symtable_stack_t *symtable_stack) {
    if(first_token == NULL) {
        if(check_token(input,TOKEN_CBRACKET_OPEN, false) == NULL) {
            exit(SYNTAX_ERROR);
        }
    } else if (first_token->type != TOKEN_CBRACKET_OPEN) {
        exit(SYNTAX_ERROR);
    }
    AstNode *blockNode = createNode(parentNode, NT_BLOCK, true);

    // possible comment after {
    // minimal one eol

    Token *token = eol_rule(input);
    statementList(input, token, blockNode, symtable_stack);

    // INFO: the closing bracket is being checked in statementList
}

// RULE: 16, 17
void statementList(char *input, Token *token, AstNode *ast, symtable_stack_t *symtable_stack) {
    // comments on its own line inside block skipped
    if(token->type == TOKEN_CBRACKET_CLOSE) {
        // RULE: 17
        // epsilon
        // this is the ending bracket from block
        return;
    } else {
        // RULE: 16
        Token *token_from_statement = statement(input, token, ast, symtable_stack);

        statementList(input, token_from_statement, ast, symtable_stack);
    }
}

// RULE: 18, 19, 20 ,21 ,22, 23
Token* statement(char *input, Token *first_token, AstNode *ast, symtable_stack_t *symtable_stack) {
    
    Token *token = NULL;
    if(first_token->type == TOKEN_KEYWORD_VAR) {
        // 18
        Token *token = getTokenSyntax(input);

        if(token->type != TOKEN_LOCAL_IDENTIFIER) {
            exit(SYNTAX_ERROR);
        }

        if(check_var_redefined(symtable_stack, token->token_str)) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        // AST: this is variable declaration, token->token_str is var name
        AstNode *varDeclNode = createNode(ast, NT_VAR, true);
        //varDeclNode->var.name = strdup(token->token_str);
        varDeclNode->var.name = malloc(sizeof(token->token_str));
        strcpy(varDeclNode->var.name, token->token_str);

        symbol_t *new_variable_symbol = create_new_variable_symbol(token->token_str, false, TYPE_UNKOWN);
        if(new_variable_symbol == NULL) {
            exit(INTERNAL_ERROR);
        }
        if (insert_symbol_to_symtable(symtable_stack->top->symbol_table, new_variable_symbol) == 1) {
            exit(SEMANTIC_REDEFINITION_ERROR);
        }

        free(token);

    } else if (first_token->type == TOKEN_KEYWORD_IF) {
        // 19
        Token *token = getTokenSyntax(input);
        if(token->type != TOKEN_BRACKET_OPEN) {
            exit(SYNTAX_ERROR);
        }

        AstNode *ifNode = createNode(ast, NT_IF_ELSE, true);
        //ifNode->ifelse.condition = createNode(ifNode, NT_LITERAL, true);
        //ifNode->ifelse.condition->string.value = "expr";

        Token *input_token = getTokenSyntax(input);
        Token *last_token = expression(input, input_token, symtable_stack, ifNode);
        if(last_token->type != TOKEN_BRACKET_CLOSE) {
            exit(SYNTAX_ERROR);
        }

        //INFO: else not needed only in the bonuses

        AstNode *trueBlock = createNode(ifNode, NT_BLOCK, true);

        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        block(input, NULL, trueBlock, symtable_stack);

        symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
        delete_symtable(symtable_from_stack);
        free(symtable_from_stack);

        ifNode->ifelse.true_body = trueBlock->block.firstStmt;

        Token *forwardCheck = getTokenSyntax(input);
        if (forwardCheck->type == TOKEN_KEYWORD_ELSE) {
            AstNode *falseBlock = createNode(ifNode, NT_BLOCK, true);

            symbol_t **new_symtable = create_new_symtable();
            if(new_symtable == NULL) {
                exit(INTERNAL_ERROR);
            }
            push_to_stack(symtable_stack, new_symtable);


            block(input, NULL, falseBlock, symtable_stack);

            symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
            delete_symtable(symtable_from_stack);
            free(symtable_from_stack);


            ifNode->ifelse.false_body = falseBlock->block.firstStmt;
            ifNode->ifelse.else_present = true;
        }

    } else if (first_token->type == TOKEN_KEYWORD_WHILE) {
        // RULE: 20
        Token *token = getTokenSyntax(input);
        if(token->type != TOKEN_BRACKET_OPEN) {
            exit(SYNTAX_ERROR);
        }

        AstNode *whileNode = createNode(ast, NT_WHILE, true);
        //whileNode->while_stmt.condition = createNode(whileNode, NT_LITERAL, true);
        //whileNode->while_stmt.condition->string.value = "expr";
        Token *input_token = getTokenSyntax(input);
        Token *last_token = expression( input, input_token, symtable_stack, whileNode); 
        if(last_token->type != TOKEN_BRACKET_CLOSE) {
            exit(SYNTAX_ERROR);
        }
        AstNode *whileBlock = createNode(whileNode, NT_BLOCK, true);
        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        block(input, NULL, whileBlock, symtable_stack);

        symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
        delete_symtable(symtable_from_stack);
        free(symtable_from_stack);

        whileNode->while_stmt.body = whileBlock->block.firstStmt;


    } else if (first_token->type == TOKEN_KEYWORD_RETURN) {
        // RULE: 21
        Token *input_token = getTokenSyntax(input);
        AstNode *returnNode = createNode(ast, NT_RETURN, true);
        token = expression(input, input_token, symtable_stack, returnNode);

    } else if (first_token->type == TOKEN_LOCAL_IDENTIFIER || first_token->type == TOKEN_GLOBAL_IDENTIFIER) {
        // RULE: 23, 24
        // ID info in token
        // TODO: handling of global/local in semantics

        AstNode *assignNode = createNode(ast, NT_ASSIGN, true);
        //printf("\nfirst_token->token_str: %s\n", first_token->token_str);
        assignNode->assign.left = createNode(assignNode, NT_VAR, true);
       //assignNode->assign.left->var.name = strdup(first_token->token_str);
        assignNode->assign.left->var.name = malloc(sizeof(first_token->token_str));
        strcpy(assignNode->assign.left->var.name, first_token->token_str);

        int function_call_args = 0;
        token = statementTail(input, assignNode, symtable_stack, ast, &function_call_args);
        if(first_token->type == TOKEN_GLOBAL_IDENTIFIER) {
            // TODO
            //if not defined yet, insert into the first table with functions
            symbol_t *global_symbol = create_new_variable_symbol(first_token->token_str, true, TYPE_UNKOWN);
            insert_into_certain_scope(symtable_stack, global_symbol, 1);
        } else {
            check_variable(symtable_stack, first_token->token_str);
        }
    } else if (first_token->type == TOKEN_KEYWORD_IFJ) {
        // RULE: 25
        //built in functions
        if(check_token(input, TOKEN_DOT, false) == NULL) {
            exit(SYNTAX_ERROR);
        }

        Token *token = EolRuleNext(input);
        if(token->type != TOKEN_LOCAL_IDENTIFIER) {
            exit(SYNTAX_ERROR);
        }
        if(check_token(input, TOKEN_BRACKET_OPEN, false) == NULL) {
            exit(SYNTAX_ERROR);
        }
        int function_call_args = 0;
        argumentList(input, symtable_stack, &function_call_args);

        int builtin_check =check_built_in_function_params(token->token_str, function_call_args);
        if (builtin_check == 1) {
            exit(SEMANTIC_PARAM_ERROR);
        } else if (builtin_check == 2) {
            exit(SEMANTIC_NONDEF_ERROR);
        }
        free(token);


        // INFO: the closing bracket is being checking in argumentListTail
    } else {
        // RULE: 22

        symbol_t **new_symtable = create_new_symtable();
        if(new_symtable == NULL) {
            exit(INTERNAL_ERROR);
        }
        push_to_stack(symtable_stack, new_symtable);

        block(input, first_token, ast, symtable_stack);

        symbol_t **symtable_from_stack = pop_from_stack(symtable_stack);
        delete_symtable(symtable_from_stack);
        free(symtable_from_stack);

    }
    
    if(token != NULL) {
        //checking eol in token from expr or return
        if(token->type == TOKEN_EOL) {
            free(token);
            token = EolRuleNext(input);
        } else if(token->type == TOKEN_ONELINE_COMMENT) {
            free(token);
            // throwing away the eol after comment as well
            token = getTokenSyntax(input);
            free(token);
            token = EolRuleNext(input);
        }
        return token;

    } else {
        Token *token_after = eol_rule(input);
        return token_after;
    }

}

// RULE: 26, 27
Token *statementTail(char *input, AstNode *assignNode, symtable_stack_t *symtable_stack, AstNode *ast, int *function_call_args) {
    Token *token = getTokenSyntax(input);
    //the eols are checked in the end of statement
    if(token->type == TOKEN_EQUALS) {
        // RULE: 26

        Token *token_after_eol = EolRuleNext(input);
        Token *token = expression(input, token_after_eol, symtable_stack, assignNode);
        //printf("single operand:\ntype: %s\nliteral int value: %d\nparent type: %s\n\n", nodeTypeToString(assignNode->assign.right->type),assignNode->assign.right->num.numValue.intValue, nodeTypeToString(assignNode->assign.right->parent->type));
        return token;

    } else if(token->type == TOKEN_BRACKET_OPEN) {
        // RULE: 27
        argumentList(input, symtable_stack, function_call_args);
        return NULL;
        //the closing bracket check in argumentListTail/argumentList

    } else {
        exit(SYNTAX_ERROR);
    }
}

// RULE: 28,29
void argumentList(char *input, symtable_stack_t *symtable_stack, int *function_call_args) {
    // term will deal with epsilon - rule 33

    Token *token = getTokenSyntax(input);
    if(!term(token, symtable_stack)) {
        // no argument
        if(token->type == TOKEN_BRACKET_CLOSE) {
            // RULE: 28
            return;
        }
    }

    // RULE: 29
    (*function_call_args)++;
    argumentListTail(input, symtable_stack, function_call_args);

    // if(check_token(input, TOKEN_BRACKET_CLOSE) == NULL) {
    //     exit(SYNTAX_ERROR);
    // }
}

// RULE: 32, 33, 34
bool term(Token *token, symtable_stack_t *symtable_stack) {
    if(token->type == TOKEN_LOCAL_IDENTIFIER) {
        // RULE: 32
        check_variable(symtable_stack, token->token_str);
        print_symbol_table(symtable_stack->top->symbol_table);
        return true;
    } else if(token->type == TOKEN_GLOBAL_IDENTIFIER) {
        // RULE: 33
        return true;
    } else {
        // RULE: 34
        if(literal(token, symtable_stack)) {
            return true;
        } else {
            return false;
        }
    }
}

// RULE: 30, 31
void argumentListTail(char *input, symtable_stack_t *symtable_stack, int *function_call_args) {
    Token *token = getTokenSyntax(input);
    if(token->type == TOKEN_COMMA) {
        // RULE: 30

        Token *next = EolRuleNext(input);
        if(!term(next, symtable_stack)) {
            exit(SYNTAX_ERROR);
        }
        (*function_call_args)++;
        argumentListTail(input, symtable_stack, function_call_args);

    } else if(token->type == TOKEN_BRACKET_CLOSE) {
        // RULE: 31
        return;
    } else {
        exit(SYNTAX_ERROR);
    }
}

// RULE: 35, 36, 37, 38, 39
// AST: add node
bool literal(Token *token, symtable_stack_t *symtable_stack) {
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


int parse(char *input, AstNode *ast, symtable_stack_t *symtable_stack) {
    program(input, ast, symtable_stack);
    return 0;
}
