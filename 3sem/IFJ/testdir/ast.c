/**
 * @file ast.c
 * @brief implementation of functions used by
 * the Abstract Syntax Tree (AST).
 * 
 * @author Adam Kinzel (xkinzea00)
 * @author Juraj Mesík (xmesikj00)
 * @author Natália Holbíková (xholbin00)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "error_codes.h"


AstNode* initAst() {
    // initialize AST with program node and return it
    AstNode* programNode = createNode(NULL, NT_PROGRAM, false);
    return programNode;
}

AstNode* createNode(AstNode* node, AstNodeType type, bool isInner) {
    AstNode* newNode = malloc(sizeof(AstNode)); // allocate memory for new AST node
    if(newNode == NULL) {
        exit(INTERNAL_ERROR);
    }

    newNode->type = type;
    newNode->expr_type = DATA_TYPE_UNKNOWN;  // initialize semantic type
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->parent = NULL;

    // initializing fields required by individual node types
    // fields described in ast.h
    switch(type) {  
        case NT_ASSIGN:
            newNode->assign.left = NULL;
            newNode->assign.right = NULL;
            break;
        case NT_FUNC:
            newNode->func.name = NULL;
            // return_node stores the implicit return value (if any),
            // for example, last expression of the function, or explicit 'return'.
            newNode->func.return_node = NULL;
            newNode->func.params = NULL;
            newNode->func.paramCnt = 0;
            newNode->func.body = NULL;
            break;
        case NT_FUNC_CALL:
            newNode->call.args = NULL;
            newNode->call.name = NULL;
            newNode->call.return_val = NULL;
            newNode->call.argCnt = 0;
            break;
        case NT_RETURN:
            newNode->ret.expr = NULL;
            break;
        case NT_WHILE:
            newNode->while_stmt.condition = NULL;
            newNode->while_stmt.body = NULL;
            break;
        case NT_IF_ELSE:
            newNode->ifelse.else_present = false;
            newNode->ifelse.condition = NULL;
            newNode->ifelse.true_body = NULL;
            newNode->ifelse.false_body = NULL;
            break;
        case NT_VAR:
            newNode->var.name = NULL;
            break;
        case NT_PARAM:
            newNode->param.name = NULL;
            break;
        case NT_ARG:
            newNode->arg.expression = NULL;
            break;
        case NT_NUM:
            newNode->num.numValue.intValue = -1;
            newNode->num.numValue.floatValue = (float)-1;
            break;
        case NT_STRING:
            newNode->string.value = NULL;
            break;
        case NT_PROGRAM:
        case NT_BLOCK:
            newNode->block.firstStmt = NULL;
            break;
        case NT_PLUS:
        case NT_MINUS:
        case NT_MULT:
        case NT_DIV:
        case NT_GT:
        case NT_LT:
        case NT_GE:
        case NT_LE:
        case NT_EQ:
        case NT_NEQ:
        case NT_IS:
            newNode->binOp.opType = type;
            newNode->binOp.operand1 = NULL;
            newNode->binOp.operand2 = NULL;
            break;
        case NT_NULL:
            break;
        default:
            break;
    }

    // if node is related to an existing node, attach it
    // as a child (isInner = true) or sibling (isInner = false)
    if(node != NULL) {
        if(isInner) { // attaching as a child node
            newNode->parent = node; // set parent node
            
            // determine which child list we need to insert into
            AstNode **children_head = NULL;
            if (node->type == NT_PROGRAM || node->type == NT_BLOCK) { // children are statements
                children_head = &(node->block.firstStmt);
            } else if (node->type == NT_FUNC) { // params go to params list, everything else to body
                if(type == NT_PARAM) {
                    children_head = &(node->func.params);
                } else {
                    children_head = &(node->func.body);
                }
            } else if (node->type == NT_FUNC_CALL) { // children are arguments
                children_head = &(node->call.args);
            }

            if (children_head != NULL) { // if parent node can have children, insert the new node
                if (*children_head == NULL) { // if there are no children yet, this node becomes first child
                    *children_head = newNode;
                } else { // otherwise append to the end the list of children
                    AstNode* current = *children_head;
                    while (current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newNode;
                    newNode->prev = current;
                }
            }
        } else { // attaching as a sibling node
            newNode->prev = node; // inserting after existing node
            newNode->next = node->next;
            newNode->parent = node->parent; // same parent

            // if an existing 'next' sibling exists, fix the 'prev' pointer
            if(node->next != NULL) {
                node->next->prev = newNode;
            }
            node->next = newNode; // link sibling
        }
    }

    return newNode;
}

void deleteNodeList(AstNode* node) {
    while (node != NULL) {
        AstNode* next = node->next;
        deleteNode(node); // calling deletion function on the current node
        node = next; // moving to the next node
    }
}

void deleteNode(AstNode* node) {
    if (node == NULL) return;

    // deleting fields based on AST node type
    switch(node->type) {
        case NT_PLUS:
        case NT_MINUS:
        case NT_DIV:
        case NT_MULT:
        case NT_GT:
        case NT_LT:
        case NT_GE:
        case NT_LE:
        case NT_EQ:
        case NT_NEQ:
        case NT_IS:
            deleteNode(node->binOp.operand1);
            deleteNode(node->binOp.operand2);
            break;

        case NT_FUNC:
            if(node->func.name) {
                free(node->func.name);
            }
            if(node->func.return_node) {
                deleteNode(node->func.return_node);
            }
            deleteNodeList(node->func.params);
            deleteNodeList(node->func.body);
            break;

        case NT_FUNC_CALL:
            deleteNodeList(node->call.args);
            if(node->call.return_val) {
                deleteNode(node->call.return_val);
            }
            if(node->call.name) {
                free(node->call.name);
            }
            break;

        case NT_RETURN:
            deleteNode(node->ret.expr);
            break;

        case NT_IF_ELSE:
            deleteNode(node->ifelse.condition);
            deleteNodeList(node->ifelse.true_body);
            deleteNodeList(node->ifelse.false_body);
            break;

        case NT_FOR:
            break;

        case NT_WHILE:
            deleteNode(node->while_stmt.condition);
            deleteNodeList(node->while_stmt.body);
            break;

        case NT_LITERAL:
            break;
        
        case NT_STRING:
            if(node->string.value) {
                free(node->string.value);
            }
            break;

        case NT_VAR:
            if(node->var.name) {
                free(node->var.name);
            }
            break;

        case NT_PARAM:
            if(node->param.name) {
                free(node->param.name);
            }
            break;

        case NT_ARG:
            if(node->arg.expression) {
                deleteNode(node->arg.expression);
            }
            break;

        case NT_ASSIGN:
            deleteNode(node->assign.left);
            deleteNode(node->assign.right);
            break;

        case NT_GETTER:
            break;
        case NT_SETTER:
            break;

        case NT_PROGRAM:
        case NT_BLOCK:
            deleteNodeList(node->block.firstStmt);
            break;

        default:
            break;
    }

    free(node);
}

const char* nodeTypeToString(AstNodeType type) {
    switch (type) { // returning string with type of AST node
        case NT_PROGRAM:    return "PROGRAM";

        case NT_PLUS:       return "PLUS";
        case NT_MINUS:      return "MINUS";
        case NT_DIV:        return "DIV";
        case NT_MULT:       return "MULT";
        case NT_GT:         return "GT";
        case NT_LT:         return "LT";
        case NT_GE:         return "GE";
        case NT_LE:         return "LE";
        case NT_EQ:         return "EQ";
        case NT_NEQ:        return "NEQ";
        case NT_IS:         return "IS";

        case NT_FUNC:       return "FUNC";
        case NT_FUNC_CALL:  return "FUNC_CALL";
        case NT_PARAM:      return "PARAM";
        case NT_ARG:        return "ARG";
        case NT_RETURN:     return "RETURN";

        case NT_IF_ELSE:    return "IF_ELSE";
        case NT_FOR:        return "FOR";
        case NT_WHILE:      return "WHILE";

        case NT_STRING:     return "LITERAL";
        case NT_NUM:        return "LITERAL";
        case NT_LITERAL:    return "LITERAL";
        case NT_NULL:       return "NULL";
        
        case NT_VAR:        return "VAR";
        
        case NT_ASSIGN:     return "ASSIGN";

        case NT_GETTER:     return "GETTER";
        case NT_SETTER:     return "SETTER";
        case NT_BLOCK:      return "BLOCK";
        
        default:            return "UNKNOWN";
    }
}

const char* dataTypeToString(data_type type) {
    switch (type) { // returning type of data
        case DATA_TYPE_NUM:     return "Num";
        case DATA_TYPE_STRING:  return "String";
        case DATA_TYPE_NULL:    return "Null";
        case DATA_TYPE_BOOL:    return "Bool";
        case DATA_TYPE_VOID:    return "Void";
        case DATA_TYPE_UNKNOWN: return "?";
        default:                return "?";
    }
}

void printAst(AstNode* node, int depth) {
    while (node != NULL) {
        for (int i = 0; i < depth; i++) { // indentation
            printf("  ");
        }
        
        printf("%s", nodeTypeToString(node->type)); // calling helper function

        switch(node->type) { // printing fields necessary for individual AST node types
            case NT_PROGRAM:
            case NT_BLOCK:
                printf("\n");
                printAst(node->block.firstStmt, depth + 1);
                break;

            case NT_VAR:
                if (node->var.name) printf(" (%s", node->var.name);
                if (node->expr_type != DATA_TYPE_UNKNOWN) printf(": %s", dataTypeToString(node->expr_type));
                printf(")\n");
                break;

            case NT_PARAM:
                if (node->param.name) printf(" (%s)", node->param.name);
                printf("\n");
                break;

            case NT_ARG:
                printf("\n");
                if (node->arg.expression) {
                    printAst(node->arg.expression, depth + 1);
                }
                break;

            case NT_NUM:
                printf(" (%f", node->num.numValue.floatValue);
                if (node->expr_type != DATA_TYPE_UNKNOWN) printf(": %s", dataTypeToString(node->expr_type));
                printf(")\n");
                break;

            case NT_STRING:
                printf(" (%s", node->string.value);
                if (node->expr_type != DATA_TYPE_UNKNOWN) printf(": %s", dataTypeToString(node->expr_type));
                printf(")\n");
                break;

            case NT_FUNC:
                if (node->func.name) printf(" (name = '%s', params = [", node->func.name);
                if(node->func.params) {
                    AstNode *param = node->func.params;
                    while(param != NULL) {
                        printf("%s", param->param.name);
                        if(param->next != NULL) {
                            printf(", ");
                        }
                        param = param->next;
                    }
                    printf("])\n");
                } else {
                    printf("])\n");
                }
                printAst(node->func.body, depth + 1);
                break;

                case NT_FUNC_CALL:
                if(node->call.name) {
                    printf(" (name = '%s'", node->call.name);
                } else {
                    printf(" (name = NULL");
                }

                AstNode *arg = node->call.args;
                if (arg) {
                    printf(", args = [");
                    bool first = true;
                    while (arg != NULL) {
                        if (!first) printf(", ");
                        first = false;

                        switch(arg->type) {
                            case NT_VAR:
                                printf("%s", arg->var.name ? arg->var.name : "VAR(NULL)");
                                break;
                            case NT_STRING:
                                printf("\"%s\"", arg->string.value ? arg->string.value : "");
                                break;
                            case NT_NUM:
                                printf("%f", arg->num.numValue.floatValue);
                                break;
                            case NT_LITERAL:
                                printf("null");
                                break;
                            default:
                                printf("%s", nodeTypeToString(arg->type));
                                break;
                        }
                        arg = arg->next;
                    }
                    printf("]");
                } else {
                    printf(", args = []");
                }
                
                printf(")\n");
                
                if (node->call.args) {
                    printAst(node->call.args, depth + 1);
                }
                
                break;

            case NT_RETURN:
                printf("\n");
                printAst(node->ret.expr, depth + 1);
                break;

            case NT_IF_ELSE:
                printf("\n");
                printAst(node->ifelse.condition, depth + 1);
                printAst(node->ifelse.true_body, depth + 1);
                printAst(node->ifelse.false_body, depth + 1);
                break;

            case NT_ASSIGN:
                printf("\n");
                printAst(node->assign.left, depth + 1);
                printAst(node->assign.right, depth + 1);
                break;

            case NT_PLUS:
            case NT_MINUS:
            case NT_MULT:
            case NT_DIV:
            case NT_GT:
            case NT_LT:
            case NT_GE:
            case NT_LE:
            case NT_EQ:
            case NT_NEQ:
            case NT_IS:
                if (node->expr_type != DATA_TYPE_UNKNOWN) printf(" (: %s)", dataTypeToString(node->expr_type));
                printf("\n");
                printAst(node->binOp.operand1, depth + 1);
                printAst(node->binOp.operand2, depth + 1);
                break;

            case NT_WHILE:
                printf("\n");
                printAst(node->while_stmt.condition, depth + 1);
                printAst(node->while_stmt.body, depth + 1);
                break;

            case NT_NULL:
                printf(" (null)\n");
                break;

            default:
                printf("\n");
                break;
        }
        node = node->next;
    }
}