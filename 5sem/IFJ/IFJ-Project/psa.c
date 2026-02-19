/**
* @file psa.c
 * @brief Implementation of precedence analysis (expression parsing)
 *
 *
 * Authors:
 * - Juraj Mesík (xmesikj00), 2025
 * - Natália Holbíková (xholbin00), 2025
 */

#define _POSIX_C_SOURCE 200809L
#include "psa.h"

//helper function for printing enum values of precedence-typed tokens as strings
char *tokenType(prec_table_token t) {

    switch (t) {
        case T_PLUS:          return("T_PLUS\n"); break;
        case T_MINUS:         return("T_MINUS\n"); break;
        case T_MULTIPLY:      return("T_MULTIPLY\n"); break;
        case T_DIVIDE:        return("T_DIVIDE\n"); break;
        case T_BRACKET_OPEN:  return("T_BRACKET_OPEN\n"); break;
        case T_BRACKET_CLOSE: return("T_BRACKET_CLOSE\n"); break;
        case T_GREATER:       return("T_GREATER\n"); break;
        case T_LESS:          return("T_LESS\n"); break;
        case T_LESS_EQUALS:   return("T_LESS_EQUALS\n"); break;
        case T_GREATER_EQUALS:return("T_GREATER_EQUAdLS\n"); break;
        case T_EQUAL:         return("T_EQUAL\n"); break;
        case T_NOT_EQUAL:     return("T_NOT_EQUAL\n"); break;
        case T_STACK_BOTTOM:  return("T_STACK_BOTTOM\n"); break;
        case T_NUM:           return("T_NUM\n"); break;
        case T_STRING:        return("T_STRING\n"); break;
        case T_NULL:          return("T_NULL\n"); break;
        case T_IDENTIFIER:    return("T_IDENTIFIER\n"); break;
        case T_HANDLE_START:  return("T_HANDLE_START\n"); break;
        case T_BOOL:          return("T_BOOL\n"); break;
        case T_IS:            return("T_IS\n"); break;
        case T_TYPE_KEYWORD:  return("T_TYPE_KEYWORD\n"); break;
        default:              return("UNKNOWN TOKEN\n"); break;
    }
    return "";
}

// function for converting scanner-typed tokens into expression tokens
prec_table_token convertToken(token_type scanner_token) {

    switch(scanner_token) {
        
        case TOKEN_PLUS: return T_PLUS;
        case TOKEN_MINUS: return T_MINUS;
        case TOKEN_MULTIPLY: return T_MULTIPLY;
        case TOKEN_DIVIDE: return T_DIVIDE;
        case TOKEN_BRACKET_OPEN: return T_BRACKET_OPEN;
        case TOKEN_BRACKET_CLOSE: return T_BRACKET_CLOSE;

        case TOKEN_LESS: return T_LESS;
        case TOKEN_LESS_EQUAL: return T_LESS_EQUALS;
        case TOKEN_GREATER: return T_GREATER;
        case TOKEN_GREATER_EQUAL: return T_GREATER_EQUALS;
        case TOKEN_NOT_EQUAL: return T_NOT_EQUAL;
        case TOKEN_DOUBLE_EQUAL: return T_EQUAL;
        

        case TOKEN_LITERAL_STRING: return T_STRING;        
        case TOKEN_LITERAL_FLOAT: return T_NUM;
        case TOKEN_LITERAL_INT: return T_NUM;
        case TOKEN_LITERAL_HEX: return T_NUM;

        case TOKEN_KEYWORD_NULL_SMALL: return T_NULL;
        case TOKEN_KEYWORD_NULL_BIG: return T_TYPE_KEYWORD;
        case TOKEN_KEYWORD_STRING: return T_TYPE_KEYWORD;
        case TOKEN_KEYWORD_NUM: return T_TYPE_KEYWORD;
        case TOKEN_KEYWORD_IS: return T_IS;

        case TOKEN_LOCAL_IDENTIFIER: return T_IDENTIFIER;        
        case TOKEN_GLOBAL_IDENTIFIER: return T_IDENTIFIER;
        default: return -1;
    }
}

// function for converting precedence-typed tokens into AST node typed tokens
AstNodeType convertToAst (prec_table_token ptoken) {
    switch(ptoken) {
        case T_PLUS:
            return NT_PLUS;
        case T_MINUS:
            return NT_MINUS;
        case T_MULTIPLY:
            return NT_MULT;
        case T_DIVIDE:
            return NT_DIV;
        case T_LESS:
            return NT_LT;
        case T_LESS_EQUALS:
            return NT_LE;
        case T_GREATER:
            return NT_GT;
        case T_GREATER_EQUALS:
            return NT_GE;
        case T_EQUAL:
            return NT_EQ;
        case T_NOT_EQUAL:
            return NT_NEQ;
        case T_IS:
            return NT_IS;
        case T_NUM:
            return NT_NUM;
        case T_STRING:
            return NT_STRING;
        case T_NULL:
            return NT_NULL;
        case T_IDENTIFIER:
            return NT_VAR;
        default:
            return -1;
    }
}

// function to verify if token that's being parsed is allowed in expressions
bool isExprToken(token_type type) {

    token_type arr[] = {
            
            TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE,
            TOKEN_BRACKET_OPEN, TOKEN_BRACKET_CLOSE, TOKEN_LESS,
            TOKEN_GREATER, TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL,
            TOKEN_DOUBLE_EQUAL, TOKEN_NOT_EQUAL, TOKEN_LOCAL_IDENTIFIER,
            TOKEN_GLOBAL_IDENTIFIER, TOKEN_LITERAL_INT, TOKEN_LITERAL_HEX, 
            TOKEN_LITERAL_FLOAT, TOKEN_LITERAL_STRING, TOKEN_KEYWORD_NULL_SMALL,
            TOKEN_KEYWORD_STRING, TOKEN_KEYWORD_NUM, TOKEN_KEYWORD_NULL_BIG, TOKEN_KEYWORD_IS

        };

    for (int i = 0; i < 23; i++) {
        if (arr[i] == type) return true;
    }

    return false;
}

// helper function for detecting arithmetic operators
bool isOperator(prec_table_token type) {

    switch(type) {
        case T_IDENTIFIER: return false;
        case T_NUM: return false;
        case T_STRING: return false;
        case T_TYPE_KEYWORD: return false;
        case T_NULL: return false;
        default: return true;
    }

}

// helper function for detecting relation operators without '!=' and '=='
bool isRelOperator(prec_table_token op) {
    switch(op) {
        case T_LESS: return true;
        case T_GREATER: return true;
        case T_LESS_EQUALS: return true;
        case T_GREATER_EQUALS: return true;
        default:
            return false;
    }
}

// Helper function to check if EOL is allowed after a token type
// According to IFJ25 specification: EOL is allowed after opening brackets, dots, commas, operators and '='
bool allows_eol_after(token_type type) {
    switch (type) {
        // Arithmetic operators
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE:
        // Comparison operators
        case TOKEN_LESS:
        case TOKEN_GREATER:
        case TOKEN_LESS_EQUAL:
        case TOKEN_GREATER_EQUAL:
        case TOKEN_DOUBLE_EQUAL:
        case TOKEN_NOT_EQUAL:
        // Other operators
        case TOKEN_KEYWORD_IS:
        // Brackets and delimiters
        case TOKEN_BRACKET_OPEN:
        case TOKEN_DOT:
        case TOKEN_COMMA:
        // Assignment
        case TOKEN_EQUALS:
            return true;
        default:
            return false;
    }
}

// function returns number converted from Wren syntax-valid form
double convertNum(char *token_str) {
    bool has_exponent = false;
    bool is_hex = false;
    bool is_float = false;
    
    char *exponent_str = malloc(strlen(token_str)+1);
    char *number_str = malloc(strlen(token_str)+1);
    int exp_index = 0;
    int num_index = 0;

    for (size_t i = 0; i < strlen(token_str); i++) {

        if (has_exponent) {  
            exponent_str[exp_index++] = token_str[i];
            continue;            
        } 
            
        if (token_str[i] == 'e' || token_str[i] == 'E') {
            has_exponent = true;
            continue;
        }
        if (token_str[i] == '.') {
            is_float = true;
        } 
        if (token_str[i] == 'x' || token_str[i] == 'X') {
            is_hex = true;
        }
        number_str[num_index++] = token_str[i];

    }
    number_str[num_index] = '\0';
    exponent_str[exp_index] = '\0';

    if (!has_exponent) {

        if (is_hex) {
            return strtol(token_str, NULL, 0);
        }
        else if (is_float) {
            return atof(token_str);
        } else {
            return atoi(token_str);
        }

    } else {
        double num = atof(number_str);
        int exp = atoi(exponent_str);
        free(number_str);
        free(exponent_str);
        
        double result = num * pow(10, exp);
        return result;
    }
}

// helper for assigning value to literal nodes of AST
void assignNodeValue(exprStackItem item, AstNodeType node_type, AstNode *ast_node) {

    if (!item.token) {
        return;
    }

    if (!ast_node) {
        printError(INTERNAL_ERROR, "cannot assign value to NULL node\n");
    }

    if (node_type == NT_NUM) {

        double num_repr = convertNum(item.token->token_str);
        switch(item.token->type) {
            // integers in decimal form
            case TOKEN_LITERAL_INT:
                ast_node->num.numValue.floatValue = (float)num_repr;
                break;
            
            // float numbers and integers with exponent 
            case TOKEN_LITERAL_FLOAT:
                ast_node->num.numValue.floatValue = (float)num_repr;
                break;
            
            // integers in hexadecimal form
            case TOKEN_LITERAL_HEX:
                ast_node->num.numValue.floatValue = (float)num_repr;
                break;
            default:
                printError(INTERNAL_ERROR, "lexer token must be of numeric type for assigning NT_NUM AST node value");
                break;
        }
        
    } else if (node_type == NT_STRING) {

        if (item.token->type == TOKEN_LITERAL_STRING) {
            // copy string representation of scanner token to right-side node 
            ast_node->string.value = malloc(strlen(item.token->token_str)+1);
            if (!ast_node->string.value) printError(INTERNAL_ERROR, "malloc fail");
            strcpy(ast_node->string.value, item.token->token_str);

        } else {
            printError(INTERNAL_ERROR, "lexer token must be of string type for assigning NT_STRING AST node value");
        }
    } else if (node_type == NT_VAR) {

        if (item.token->type == TOKEN_LOCAL_IDENTIFIER || item.token->type == TOKEN_GLOBAL_IDENTIFIER) {
            
            // copy string representation of scanner token to right-side node 
            ast_node->var.name = malloc(strlen(item.token->token_str)+1);
            if (!ast_node->var.name) printError(INTERNAL_ERROR, "malloc fail");
            strcpy(ast_node->var.name, item.token->token_str);

        } else {
            printError(INTERNAL_ERROR, "lexer token must be of identifier type for assigning NT_VAR AST node value");
        }

    } else {
        return;
    }
     
}

// Helper function to get the actual type of an identifier from symtable
prec_table_token get_identifier_type(exprStackItem item, symtable_stack_t *symtable_stack) {
    if (item.type != T_IDENTIFIER) {
        return item.type;  // Not an identifier, return as-is
    }
    
    if (item.token == NULL) {
        return T_IDENTIFIER;  // Unknown type at compile time
    }
    
    char *var_name = item.token->token_str;
    char *key = create_key(var_name, 0, T_UNKNOWN);
    if (key == NULL) {
        return T_IDENTIFIER;  // Unknown type
    }
    
    symbol_t *symbol = get_symbol_from_upper_scopes(symtable_stack, key);
    free(key);
    
    if (symbol == NULL) {
        return T_IDENTIFIER;  // Unknown type
    }
    
    // If it's a function (getter), treat as identifier with unknown type
    if (symbol->is_function) {
        return T_IDENTIFIER;
    }
    
    // Convert var_type_t to prec_table_token
    switch (symbol->var_type) {
        case TYPE_NUM:
            return T_NUM;
        case TYPE_STRING:
            return T_STRING;
        case TYPE_NULL:
            return T_NULL;
        case TYPE_PARAM:
        case TYPE_UNKNOWN:
        default:
            return T_IDENTIFIER;  // Unknown type
    }
}

// this function is an implementation of reduction according to LL rules and basic type checking
prec_table_token handleExpression(exprStackItem handle_items[], int len, AstNode *exprNode, AstNode **outputNode, symtable_stack_t *symtable_stack) {
    
    // exprNode is parent AST node passed from parser
    if (len == 1) {
        // Check if trying to reduce an operator as operand (syntax error)
        if (isOperator(handle_items[0].type)) {
            // free token before error
            if (handle_items[0].token) {
                free(handle_items[0].token->token_str);
                free(handle_items[0].token);
                handle_items[0].token = NULL;
            }
            printError(SYNTAX_ERROR, "operator cannot be used as operand");
        }
        
        // creating AST node for expressions that consist of single literal ( no operator )
        switch (exprNode->type) {

            case NT_IF_ELSE:
                if (handle_items[0].node != NULL) {
                    *outputNode = handle_items[0].node;
                } else {
                    AstNode *valueNode = createNode(NULL, convertToAst(handle_items[0].type), false);
                    assignNodeValue(handle_items[0], convertToAst(handle_items[0].type), valueNode);
                    *outputNode = valueNode;
                }
                break;
            case NT_WHILE:
                if (handle_items[0].node != NULL) {
                    *outputNode = handle_items[0].node;
                } else {
                    AstNode *valueNode = createNode(NULL, convertToAst(handle_items[0].type), false);
                    assignNodeValue(handle_items[0], convertToAst(handle_items[0].type), valueNode);
                    *outputNode = valueNode;
                }
                break;
            case NT_RETURN:
                if (handle_items[0].node != NULL) {
                    *outputNode = handle_items[0].node;
                } else {
                    AstNode *valueNode = createNode(NULL, convertToAst(handle_items[0].type), false);
                    assignNodeValue(handle_items[0], convertToAst(handle_items[0].type), valueNode);
                    *outputNode = valueNode;
                }
                break;
            case NT_ASSIGN:
                // If handle_items already has an AST node (e.g., from function call), use it
                if (handle_items[0].node != NULL) {
                    exprNode->assign.right = handle_items[0].node;
                    exprNode->assign.right->parent = exprNode;
                    *outputNode = exprNode->assign.right;
                } else {
                    exprNode->assign.right = createNode(exprNode, convertToAst(handle_items[0].type), true);
                    assignNodeValue(handle_items[0], convertToAst(handle_items[0].type), exprNode->assign.right);
                    *outputNode = exprNode->assign.right;
                }
                break;
            case NT_ARG:
                if (handle_items[0].node != NULL) {
                    *outputNode = handle_items[0].node;
                } else {
                    AstNode *valueNode = createNode(NULL, convertToAst(handle_items[0].type), false);
                    assignNodeValue(handle_items[0], convertToAst(handle_items[0].type), valueNode);
                    *outputNode = valueNode;
                }
                break;
            default:
               break;
        }

        // free the token copy
        if (handle_items[0].token) {
            free(handle_items[0].token->token_str);
            free(handle_items[0].token);
            handle_items[0].token = NULL;
        }
        return handle_items[0].type;

    } else if (len == 3) {

        prec_table_token right = handle_items[0].type;
        prec_table_token operator = handle_items[1].type;
        prec_table_token left = handle_items[2].type;

        if (left == T_BRACKET_OPEN && right == T_BRACKET_CLOSE && handle_items[1].term_type == NONTERMINAL) {
            *outputNode = handle_items[1].node;
            return handle_items[1].type;
        }

        switch(exprNode->type) {

            // AST nodes are already created from reduction of each operand terminals -- earlier function calls when len == 1.
            // those are now just reused as children of new operator node.  
            
            // also single child node of parent node (exprNode) is being replaced by operator node of binary operation
            // operator node is then parent of both operand nodes

            case NT_IF_ELSE:

                // create AST node based on used operator 
                exprNode->ifelse.condition = createNode(exprNode, convertToAst(operator), true);
                exprNode->ifelse.condition->binOp.operand1 = handle_items[2].node;
                exprNode->ifelse.condition->binOp.operand2 = handle_items[0].node;

                *outputNode = exprNode->ifelse.condition;
                break;

            case NT_WHILE:
                exprNode->while_stmt.condition = createNode(exprNode, convertToAst(operator), true);

                exprNode->while_stmt.condition->binOp.operand1 = handle_items[2].node;
                exprNode->while_stmt.condition->binOp.operand2 = handle_items[0].node;

                *outputNode = exprNode->while_stmt.condition;

                break;

            case NT_RETURN:
                exprNode->ret.expr = createNode(exprNode, convertToAst(operator), true);

                exprNode->ret.expr->binOp.operand1 = handle_items[2].node;
                exprNode->ret.expr->binOp.operand2 = handle_items[0].node;

                *outputNode = exprNode->ret.expr;

                break;

            case NT_ASSIGN:

                exprNode->assign.right = createNode(exprNode, convertToAst(operator), true);

                exprNode->assign.right->binOp.operand1 = handle_items[2].node;
                exprNode->assign.right->binOp.operand2 = handle_items[0].node;

                *outputNode = exprNode->assign.right;
                
                break;

            case NT_ARG: {
                AstNode *opNode = createNode(NULL, convertToAst(operator), false);
                opNode->binOp.operand1 = handle_items[2].node;
                opNode->binOp.operand2 = handle_items[0].node;
                
                if (handle_items[2].node) handle_items[2].node->parent = opNode;
                if (handle_items[0].node) handle_items[0].node->parent = opNode;
                
                *outputNode = opNode;
                break;
            }
            default:
                break;
        }
        
        // Check if operands are actually operators (syntax error: missing operand)
        if (isOperator(left) || isOperator(right)) {
            // free tokens before error
            for (int i = 0; i < len; i++) {
                if (handle_items[i].token) {
                    free(handle_items[i].token->token_str);
                    free(handle_items[i].token);
                    handle_items[i].token = NULL;
                }
            }
            printError(SYNTAX_ERROR, "invalid expression: operator used as operand");
        }
        
        // Resolve identifier types from symbol table BEFORE freeing tokens
        prec_table_token actual_left = left;
        prec_table_token actual_right = right;
        
        if (left == T_IDENTIFIER && symtable_stack != NULL) {
            actual_left = get_identifier_type(handle_items[2], symtable_stack);
        }
        if (right == T_IDENTIFIER && symtable_stack != NULL) {
            actual_right = get_identifier_type(handle_items[0], symtable_stack);
        }
        
        // free used token copies AFTER type resolution
        for (int i = 0; i < len; i++) {
            if (handle_items[i].token) {
                free(handle_items[i].token->token_str);
                free(handle_items[i].token);
                handle_items[i].token = NULL;
            }
        }
        
        // Type checking with resolved types
        switch(actual_left) {
            // "a" ...
            case T_STRING:
                switch(operator) {
                    case T_MULTIPLY:
                        // "a" * 5
                        if (actual_right == T_NUM) {
                            return T_STRING;
                        // "a" * var (unknown type)
                        } else if (right == T_IDENTIFIER && actual_right == T_IDENTIFIER) {
                            return T_IDENTIFIER;
                        } 
                        return T_ERROR;
                    case T_PLUS:
                        // "a" + "a"
                        if (actual_right == T_STRING) {
                            return T_STRING;
                        // "a" + var (unknown type)
                        } else if (right == T_IDENTIFIER && actual_right == T_IDENTIFIER) {
                            return T_IDENTIFIER;
                        }
                        return T_ERROR;
                    // "a" is String
                    case T_IS:
                        if (right != T_TYPE_KEYWORD) {
                            return T_ERROR;
                        }
                        return T_BOOL;
                    // "a" == anything
                    case T_EQUAL:
                        return T_BOOL;
                    // "a" != anything
                    case T_NOT_EQUAL:
                        return T_BOOL;
                    // "a" [-, /, <, >, <=, >=] anything
                    default:
                        return T_ERROR;
                }
                break;
            // 5 ...
            case T_NUM:
                switch(operator) {
                    // 5 is Num
                    case T_IS:
                        if (right != T_TYPE_KEYWORD) {
                            return T_ERROR;
                        }
                        return T_BOOL;
                    // 5 == anything
                    case T_EQUAL:
                        return T_BOOL;
                    // 5 != anything
                    case T_NOT_EQUAL:
                        return T_BOOL;
                    default:
                        // 5 + null or 5 + "string"
                        if (actual_right != T_NUM && !(right == T_IDENTIFIER && actual_right == T_IDENTIFIER)) {
                            return T_ERROR;
                         // 5 [<,>,<=,>=] 6
                        } else if (isRelOperator(operator)) {
                            return T_BOOL;
                        // 5 [+,-,*,/] var (unknown type)
                        } else if (right == T_IDENTIFIER && actual_right == T_IDENTIFIER){    
                            return T_IDENTIFIER;
                        // 5 [+,-,*,/] 2
                        } else {
                            return T_NUM;
                        }
                        break;
                }
                break;
            case T_NULL:
                switch (operator) {
                    // null is Null
                    case T_IS:
                        if (right != T_TYPE_KEYWORD) {
                            return T_ERROR;
                        }
                        return T_BOOL;
                    case T_EQUAL:
                        return T_BOOL;
                    case T_NOT_EQUAL:
                        return T_BOOL;
                    default:
                        return T_ERROR;
                        break;
                }
                break;
            
            // var ... (identifier with unknown type at compile time)
            case T_IDENTIFIER:
                // If we got here, the identifier type is unknown
                switch(operator) {
                    // var is String
                    case T_IS:
                        if (right != T_TYPE_KEYWORD) {
                            return T_ERROR;
                        }
                        return T_IDENTIFIER;
                    case T_EQUAL:
                        return T_BOOL;
                    case T_NOT_EQUAL:
                        return T_BOOL;
                    default:
                        // var [<, >, <=, >=] 3
                        // var1 [<, >, <=, >=] var2
                        // var1 [-,+,*/] var2
                        // var [-,+,*,/] 5
                        if (actual_right != T_NUM && actual_right != T_IDENTIFIER && actual_right != T_STRING) { 
                            return T_ERROR;
                        } else if (!isOperator(operator)) {
                            return T_ERROR;
                        }
                        return T_IDENTIFIER;
                }
                
            default:
                return T_ERROR;
        }

    }
    
    // If len is 2, it's typically E op or op E (missing operand) - syntax error
    if (len == 2) {
        // free tokens before error
        for (int i = 0; i < len; i++) {
            if (handle_items[i].token) {
                free(handle_items[i].token->token_str);
                free(handle_items[i].token);
                handle_items[i].token = NULL;
            }
        }
        printError(SYNTAX_ERROR, "invalid expression: missing operand");
    }
    
    return T_ERROR;
}

// helper for printing current state of stack
void printStack(exprStack *stack) {
    printf("---- expr stack ----\n");
    exprStackItem *it = stack->top;
    while (it != NULL) {
        printf("term_type=%s type: %snode: %p  next=%p\n---\n",
            (it->term_type == TERMINAL) ? "TERMINAL" : "NONTERMINAL", tokenType(it->type), (void*)it, (void*)it->next);
        it = it->next;
    }
    printf("--------------------\n");
}

// ...
void freeMemory(Token **input_token, exprStack *stack) {

    stackDelete(&stack);
    free((*input_token)->token_str);
    free((*input_token));
} 

// Pomocná funkcia na pridanie argumentu do linked list
void addArgumentToCall(AstNode *callNode, AstNode *argNode) {
    if (callNode->call.args == NULL) {
        // Prvý argument
        callNode->call.args = argNode;
    } else {
        // Nájdeme koniec linked listu
        AstNode *current = callNode->call.args;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = argNode;
        argNode->prev = current;
    }
    callNode->call.argCnt++;
}

Token *expressionArgumentList(Token *token, symtable_stack_t *symtable, AstNode *callNode) {
    // Empty argument list check
    if (token->type == TOKEN_BRACKET_CLOSE) {
        return token; 
    }

    while (true) {
        AstNode *argNode = createNode(NULL, NT_ARG, false);
        argNode->parent = callNode;
        
        token = expression(token, symtable, argNode);
        
        addArgumentToCall(callNode, argNode);

        if (token->type == TOKEN_COMMA) {
            // Preskočíme čiarku
            free(token->token_str);
            free(token);
            token = getToken();
            
            // Po čiarke môže byť EOL - preskočíme ho
            while (token->type == TOKEN_EOL) {
                free(token->token_str);
                free(token);
                token = getToken();
            }
            
            continue;
        } else if (token->type == TOKEN_BRACKET_CLOSE) {
            // Koniec zoznamu argumentov
            break;
        } else {
            printError(SYNTAX_ERROR, "Expected ',' or ')' in argument list");
        }
    }
    
    return token; // Vracia token ')'
}

// precedence parser function
void parseExpr(Token **input_token, exprStack *stack, prec_table_op operation,
    bool advance_token, bool expect_operator, AstNode *exprNode, symtable_stack_t *symtable_stack) {

    exprStackItem *input;
    switch(operation) {

        case SHIFT:
            // push handle start on stack
            ;
            exprStackItem *handle_start = createStackItem(T_HANDLE_START, TERMINAL, NULL, NULL, stack->top);
            if(!handle_start) {
                freeMemory(input_token, stack);
                printError(INTERNAL_ERROR, "stack item malloc fail");
            }
            stackPush(stack, handle_start);
            
            // push token from input on stack
            input = createStackItem(convertToken((*input_token)->type), TERMINAL, (*input_token), NULL, stack->top);
            
            if (!input) {
                freeMemory(input_token, stack);
                printError(INTERNAL_ERROR, "stack item malloc fail");
            }
            stackPush(stack, input);
            // check for correct operand/operator order
            if (expect_operator && !isOperator(input->type)) {
                freeMemory(input_token, stack);
                printError(SYNTAX_ERROR, "incorrect operator/operand order");
            }
            break;

        case REDUCTION:
            ;
            exprStackItem handleItems[10];
            int len = 0;

            // reduce items on stack until encountering first handle start 
            while (stack->top != NULL && stack->top->type != T_HANDLE_START) {

                exprStackItem tmp = *stack->top;
                tmp.token = copyToken(stack->top->token);
                handleItems[len++] = tmp;
                
                stackPop(stack);
            }
            if (!stackTop(stack)) {
                freeMemory(input_token, stack);
                printError(INTERNAL_ERROR, "expression stack malformed");
            }
            // pop handle start as well
            stackPop(stack);
            
            // we need to preserve identifier token BEFORE calling handleExpression (which frees tokens)
            Token *preserved_token = NULL;
            if (len == 1 && handleItems[0].token != NULL) {
                preserved_token = copyToken(handleItems[0].token);
            }
            
            // resultNode holds value of lastly reduced nonterminal, so it can be safely used when parsing E op E -> E
            AstNode *resultNode = NULL;
            prec_table_token reducedType = handleExpression(handleItems, len, exprNode, &resultNode, symtable_stack);
            if (reducedType == T_ERROR) {
                // if datatype combination is not allowed, free memory and exit
                freeMemory(input_token, stack);
                printError(SEMANTIC_TYPE_EXPR_ERROR, "detected incompatiple types in expression");
            }
    
            exprStackItem *nonterm = createStackItem(reducedType, NONTERMINAL, preserved_token, resultNode, NULL);
            if (!nonterm) {
                freeMemory(input_token, stack);
                printError(INTERNAL_ERROR, "stack item malloc fail");
            }
            stackPush(stack, nonterm);
            // after reduction, input stays the same
            return;

        case ERROR:
            // Check if this is a semantic type error rather than syntax error
            prec_table_token stack_term = stackTermTop(stack)->type;
            prec_table_token input_term = convertToken((*input_token)->type);
            
            // null with arithmetic operators (+, -, *, /) is semantic error 6
            if (stack_term == T_NULL && 
                (input_term == T_PLUS || input_term == T_MINUS || 
                 input_term == T_MULTIPLY || input_term == T_DIVIDE)) {
                freeMemory(input_token, stack);
                printError(SEMANTIC_TYPE_EXPR_ERROR, "null cannot be used in arithmetic operations");
            }
            
            // String with - or / is semantic error 6
            if (stack_term == T_STRING && 
                (input_term == T_MINUS || input_term == T_DIVIDE)) {
                freeMemory(input_token, stack);
                printError(SEMANTIC_TYPE_EXPR_ERROR, "strings do not support - or / operators");
            }
            
            // Operator - or / with String operand is semantic error 6
            if ((stack_term == T_MINUS || stack_term == T_DIVIDE) && 
                input_term == T_STRING) {
                freeMemory(input_token, stack);
                printError(SEMANTIC_TYPE_EXPR_ERROR, "strings do not support - or / operators");
            }
            
            // Otherwise it's a syntax error
            freeMemory(input_token, stack);
            printError(SYNTAX_ERROR, "expression components precedence error");
            break;

        case EQUAL:

            // push token from input on stack 
            input = createStackItem(convertToken((*input_token)->type), TERMINAL, (*input_token), NULL, stack->top);
            if (!input) {
                freeMemory(input_token, stack);
                printError(INTERNAL_ERROR, "stack item malloc fail");
            }
            stackPush(stack, input);
            // move to the next token
            break;
        case EOE:
            return;

        default:
            printError(INTERNAL_ERROR, "precedence implementation failed");   
            break;
    }

    // if there's more tokens in an expression, tokens are dynamically advancing each iteration 
    // if not, it means that there's been non-expression token --> end of expression --> reduction of stack items according to LL rules is happening
    if (advance_token) {
        *input_token = getToken();
    }

}

Token *expression(Token *input_token, symtable_stack_t *symtable_stack, AstNode *exprNode) {
    exprStack *stack = malloc(sizeof(*stack));
    if (!stack) {
        printError(INTERNAL_ERROR, "malloc fail");
    }
    stackInit(&stack);
    if(input_token == NULL) {
        input_token = getToken();
    }

    prec_table_op operation;    
    bool expect_operator = false;

    Token *last = NULL;

    while(true) {
        // === EOL HANDLING ===
        // Skip EOLs that are allowed after certain tokens (operators, dots, commas, brackets, =)
        while (input_token->type == TOKEN_EOL) {
            if (last != NULL && allows_eol_after(last->type)) {
                // EOL is allowed here - skip it and read next token
                free(input_token->token_str);
                free(input_token);
                input_token = getToken();
            } else {
                // EOL is not allowed here - this ends the expression
                break;
            }
        }

        // =================================================================
        // LOGIKA PRE FUNCTION CALLS (id(args) alebo Ifj.id(args))
        // =================================================================
        if (input_token->type == TOKEN_LOCAL_IDENTIFIER || 
            input_token->type == TOKEN_GLOBAL_IDENTIFIER || 
            input_token->type == TOKEN_KEYWORD_IFJ) {
            
            // Lookahead - pozrieme sa na ďalší token
            Token *next_token = getToken();
            bool is_func_call = false;
            char *func_name = NULL;

            // Prípad 1: id(...)
            if (next_token->type == TOKEN_BRACKET_OPEN) {
                is_func_call = true;
                
                func_name = malloc(strlen(input_token->token_str) + 1);
                if (!func_name) printError(INTERNAL_ERROR, "malloc fail for func_name");
                strcpy(func_name, input_token->token_str);
                
                free(next_token->token_str);
                free(next_token);
                
                next_token = getToken(); 

            // Prípad 2: Ifj.id(...) alebo id.id(...)
            } else if (next_token->type == TOKEN_DOT) {
                free(next_token->token_str);
                free(next_token);
                
                // Po bodke môže byť EOL
                Token *method = getToken();
                while (method->type == TOKEN_EOL) {
                    free(method->token_str);
                    free(method);
                    method = getToken();
                }
                
                if (method->type != TOKEN_LOCAL_IDENTIFIER) {
                    printError(SYNTAX_ERROR, "Expected method identifier after '.'");
                }
                
                // Po názve metódy môže byť EOL pred zátvorkou
                Token *lparen = getToken();
                while (lparen->type == TOKEN_EOL) {
                    free(lparen->token_str);
                    free(lparen);
                    lparen = getToken();
                }
                
                if (lparen->type != TOKEN_BRACKET_OPEN) {
                    printError(SYNTAX_ERROR, "Expected '(' after method name");
                }
                free(lparen->token_str);
                free(lparen);

                size_t len = strlen(input_token->token_str) + 1 + strlen(method->token_str) + 1;
                func_name = malloc(len);
                sprintf(func_name, "%s.%s", input_token->token_str, method->token_str);
                
                free(method->token_str);
                free(method);

                is_func_call = true;
                next_token = getToken();
            }

            if (is_func_call) {
                AstNode *funcCallNode = createNode(NULL, NT_FUNC_CALL, false);
                funcCallNode->call.name = func_name;
                funcCallNode->call.argCnt = 0;
                funcCallNode->call.args = NULL;

                // Spracujeme argumenty
                Token *closing_paren = expressionArgumentList(next_token, symtable_stack, funcCallNode);

                // Sémantická kontrola funkcie
                // Ak je to Ifj.*, kontrola built-in funkcií
                if (strncmp(func_name, "Ifj.", 4) == 0) {
                    char *method_name = func_name + 4; // Preskočíme "Ifj."
                    int builtin_check = check_built_in_function_params(method_name, funcCallNode->call.argCnt);
                    if (builtin_check == 1) {
                        printError(SEMANTIC_PARAM_ERROR, "Wrong number of arguments for built-in function");
                    } else if (builtin_check == 2) {
                        printError(SEMANTIC_NONDEF_ERROR, "Undefined built-in function");
                    }
                } else {
                    check_function(symtable_stack, func_name, funcCallNode->call.argCnt, T_FUNC);
                }

                exprStackItem *func_result = createStackItem(T_IDENTIFIER, NONTERMINAL, NULL, funcCallNode, NULL);
                if (!func_result) printError(INTERNAL_ERROR, "stack item malloc fail");
                stackPush(stack, func_result);

                // Clean up 'last' token
                if (last) free(last);
                last = malloc(sizeof(Token));
                last->type = TOKEN_BRACKET_CLOSE;
                last->token_str = NULL;

                // Načítame ďalší token
                free(closing_paren->token_str);
                free(closing_paren);
                free(input_token->token_str);
                free(input_token);
                input_token = getToken();
                
                expect_operator = true;
                continue;
            } else {
                // Nebolo to volanie funkcie - je to premenná/getter
                // Sémantická kontrola premennej (iba ak je to LOCAL_IDENTIFIER)
                if (input_token->type == TOKEN_LOCAL_IDENTIFIER) {
                    check_variable(symtable_stack, input_token->token_str);
                }
                
                // Spracujeme input_token a potom použijeme next_token
                operation = table[stackTermTop(stack)->type][convertToken(input_token->type)];
                if (operation == EOE) {
                    // Vrátime next_token späť
                    free(input_token->token_str);
                    free(input_token);
                    input_token = next_token;
                    break;
                }

                if (last) { free(last); last = NULL; }
                last = malloc(sizeof(Token));
                memcpy(last, input_token, sizeof(Token));
                last->token_str = strdup(input_token->token_str);

                parseExpr(&input_token, stack, operation, false, expect_operator, exprNode, symtable_stack);
                
                if (operation == SHIFT && stack->top && stack->top->type != T_BRACKET_OPEN) 
                    expect_operator = !expect_operator;

                // Posun na ďalší token
                free(input_token->token_str);
                free(input_token);
                input_token = next_token;
                continue;
            }
        }
        // =================================================================

        // first part of expression parsing happens until encountering non-expression token
        if(!isExprToken(input_token->type)) {
            // Check for empty expression (no tokens processed)
            if (last == NULL) {
                stackDelete(&stack);
                printError(SYNTAX_ERROR, "empty expression not allowed");
            }
            break;
        }

        operation = table[stackTermTop(stack)->type][convertToken(input_token->type)];        
        
        if (operation == EOE) break;
        if (last) {
            free(last);
            last = NULL;
        }
        last = malloc(sizeof(Token));
        if (!last) { printError(INTERNAL_ERROR, "malloc fail for last token"); exit(INTERNAL_ERROR); }
        memcpy(last, input_token, sizeof(Token));
        
        parseExpr(&input_token, stack, operation, true, expect_operator, exprNode, symtable_stack); 
        // operator is expected only if shifting to stack
        // when there's opening bracket, we need to shift again before expecting operator
        if (operation == SHIFT && stack->top->type != T_BRACKET_OPEN) expect_operator = !expect_operator;

        
        
    }
    
    // this cycle ensures that the remaining expression of nonterminals gets reduced to one nonterminal
    while(stackTermTop(stack)->type != T_STACK_BOTTOM) {
        
        operation = table[stackTermTop(stack)->type][T_STACK_BOTTOM];

        if (operation == EOE) break;
        parseExpr(&last ,stack, operation, false, expect_operator, exprNode, symtable_stack);
        expect_operator = !expect_operator;

    }

    // Link the result from the stack to the AST
    if (exprNode != NULL && stack->top != NULL && stack->top->node != NULL) {
        AstNode *resultNode = stack->top->node;

        switch (exprNode->type) {
            case NT_ASSIGN:
                if (exprNode->assign.right == NULL) {
                    exprNode->assign.right = resultNode;
                    resultNode->parent = exprNode;
                }
                break;

            case NT_RETURN:
                if (exprNode->ret.expr == NULL) {
                    exprNode->ret.expr = resultNode;
                    resultNode->parent = exprNode;
                }
                break;

            case NT_IF_ELSE:
                if (exprNode->ifelse.condition == NULL) {
                    exprNode->ifelse.condition = resultNode;
                    resultNode->parent = exprNode;
                }
                break;

            case NT_WHILE:
                if (exprNode->while_stmt.condition == NULL) {
                    exprNode->while_stmt.condition = resultNode;
                    resultNode->parent = exprNode;
                }
                break;
                
            case NT_ARG:
                if (exprNode->arg.expression == NULL) {
                    exprNode->arg.expression = resultNode;
                    resultNode->parent = exprNode;
                }
                break;
                 
            default:
                break;
        }
    }
    // =========================================================
    
    // Set the expression result type in the AST node
    if (exprNode != NULL) {
        prec_table_token result_type = stackTop(stack)->type;
        switch (result_type) {
            case T_NUM:
                exprNode->expr_type = DATA_TYPE_NUM;
                break;
            case T_STRING:
                exprNode->expr_type = DATA_TYPE_STRING;
                break;
            case T_NULL:
                exprNode->expr_type = DATA_TYPE_NULL;
                break;
            case T_BOOL:
                exprNode->expr_type = DATA_TYPE_BOOL;
                break;
            case T_IDENTIFIER:
                // If result is still identifier, type is unknown at compile time
                exprNode->expr_type = DATA_TYPE_UNKNOWN;
                break;
            default:
                exprNode->expr_type = DATA_TYPE_UNKNOWN;
                break;
        }
    }
    
    return input_token;
}





