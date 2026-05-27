/**
* @file generator.c
 * @brief Code generator for IFJcode25
 *
 * Authors:
 * - Natália Holbíková (xholbin00), 2025
 */

#define _DEFAULT_SOURCE
#include "generator.h"
#include "ast.h"
#include "symtable.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

// Global counter for generating unique labels
static int label_counter = 0;

// Returns unique ID for labels and increments counter
int getUniqueLabelId() {
    return label_counter++;
}

// Escapes special characters in string for IFJcode25 format
// Characters <= 32, '#', and '\' are escaped as \XXX
char* escapeStringForIFJcode(const char* str) {
    if (str == NULL) {
        char* empty = malloc(1);
        if (!empty) {
            fprintf(stderr, "ERROR: malloc failed\n");
            exit(99);
        }
        empty[0] = '\0';
        return empty;
    }
    
    // Allocate max possible size (each char could become 4 chars)
    size_t max_len = strlen(str) * 4 + 1;
    char* result = malloc(max_len);
    if (!result) {
        fprintf(stderr, "ERROR: malloc failed\n");
        exit(99);
    }
    
    char* out = result;
    for (const char* in = str; *in; in++) {
        unsigned char c = (unsigned char)*in;
        
        // Escape control chars, space, '#' and backslash
        if (c <= 32 || c == 35 || c == 92) {
            sprintf(out, "\\%03d", c);
            out += 4;
        } else {
            *out++ = c;
        }
    }
    *out = '\0';
    return result;
}

// Main entry point - generates complete IFJcode25 output
void generateCode(AstNode* ast, symbol_t* global_symtable) {
    printf(".IFJcode25\n\n");
    
    // Define helper variables for built-in functions
    printf("# === Global Helper Variables ===\n");
    printf("DEFVAR GF@_dummy_var_for_void_return\n\n");
    printf("DEFVAR GF@_builtin_s\n");
    printf("DEFVAR GF@_builtin_i\n");
    printf("DEFVAR GF@_builtin_j\n");
    printf("DEFVAR GF@_builtin_len\n");
    printf("DEFVAR GF@_builtin_k\n");
    printf("DEFVAR GF@_builtin_char\n");
    printf("DEFVAR GF@_builtin_result\n");
    printf("DEFVAR GF@_builtin_bool_result\n");

    // Define temporary variables for expression evaluation
    printf("\n# === Temporary Variables ===\n");
    printf("DEFVAR GF@_temp_op1\n");
    printf("DEFVAR GF@_temp_op2\n");
    printf("DEFVAR GF@_temp_type1\n");
    printf("DEFVAR GF@_temp_type2\n");

    // Generate user-defined global variables
    printf("\n# === User Global Variables (from Symtable) ===\n");
    generateGlobalsFromSymtable(global_symtable);

    // Generate all function definitions
    if (ast && ast->type == NT_PROGRAM) {
        generateProgram(ast);
    }
    
    // Generate main entry point that calls user's main function
    printf("\n# === Main Program Entry Point ===\n");
    printf("LABEL $main\n");
    printf("CREATEFRAME\n");
    printf("CALL main\n");
    printf("EXIT int@0\n");
}

// Traverses symbol table and defines global variables (prefix "__")
void generateGlobalsFromSymtable(symbol_t *node) {
    if (node == NULL) return;

    generateGlobalsFromSymtable(node->left);

    // Only define non-function symbols with "__" prefix
    if (!node->is_function && node->name != NULL) {
        if (strncmp(node->name, "__", 2) == 0) {
           printf("DEFVAR GF@%s\n", node->name);
        }
    }

    generateGlobalsFromSymtable(node->right);
}

// Generates jump to main and all function definitions
void generateProgram(AstNode* node) {
    if (node == NULL) return;
    printf("JUMP $main\n\n");
    
    printf("# === User Function Definitions ===\n");
    AstNode* func = node->block.firstStmt;
    
    // Iterate through all top-level function definitions
    while (func != NULL) {
        if (func->type == NT_FUNC) {
            generateFunction(func);
        }
        func = func->next;
    }
}

// Generates code for a single function definition
void generateFunction(AstNode* node) {
    if (node == NULL || node->func.name == NULL) return;
    
    printf("\n# === Function %s ===\n", node->func.name);
    printf("LABEL %s\n", node->func.name);
    printf("PUSHFRAME\n");
    
    // Pop parameters from stack into local variables
    AstNode* param = node->func.params;
    while (param != NULL) {
        if (param->param.name != NULL) {
            printf("DEFVAR LF@%s\n", param->param.name);
            printf("POPS LF@%s\n", param->param.name);
        }
        param = param->next;
    }
    
    bool has_explicit_return = false;

    // Generate function body statements
    if (node->func.body != NULL) {
        if (node->func.body->type == NT_BLOCK) {
            AstNode* stmt = node->func.body->block.firstStmt;
            while (stmt != NULL) {
                if (stmt->type == NT_RETURN) {
                    has_explicit_return = true;
                }
                generateStatement(stmt);
                stmt = stmt->next;
            }
        } else {
            generateStatement(node->func.body);
        }
    }
    
    // Add implicit return nil if no explicit return
    if (!has_explicit_return) {
        printf("PUSHS nil@nil\n");
        printf("POPFRAME\n");
        printf("RETURN\n");
    }
    printf("\n");
}

// Generates code for different statement types
void generateStatement(AstNode* node) {
    if (node == NULL) return;

    printf("\n# === Statement ===\n");
    
    switch (node->type) {
        case NT_VAR:
            // Variable declaration
            if (node->var.name != NULL) {
                printf("DEFVAR LF@%s\n", node->var.name);
            }
            break;
            
        case NT_ASSIGN:
            // Assignment: evaluate right side and store to left side
            if (node->assign.right != NULL && node->assign.left != NULL && 
                node->assign.left->var.name != NULL) {
                generateExpression(node->assign.right);
                
                // Global vars have "__" prefix, use GF, otherwise LF
                if (strncmp(node->assign.left->var.name, "__", 2) == 0) {
                    printf("POPS GF@%s\n", node->assign.left->var.name);
                } else {
                    printf("POPS LF@%s\n", node->assign.left->var.name);
                }
            }
            break;

        case NT_FUNC_CALL:
            // Standalone function call - discard return value
            generateFunctionCall(node);
            printf("POPS GF@_dummy_var_for_void_return\n");
            break;
        
        case NT_RETURN:
            // Return statement - push value and return
            if (node->ret.expr != NULL) {
                generateExpression(node->ret.expr);
            } else {
                printf("PUSHS nil@nil\n");
            }
            printf("POPFRAME\n");
            printf("RETURN\n");
            break;    

        case NT_IF_ELSE:
            generateIfElse(node);
            break;

        case NT_WHILE:
            generateWhile(node);
            break;

        case NT_BLOCK: {
            // Block of statements
            AstNode* stmt = node->block.firstStmt;
            while (stmt != NULL) {
                generateStatement(stmt);
                stmt = stmt->next;
            }
            break;
        }
        default:
            fprintf(stderr, "Warning: Statement type %d not implemented\n", node->type);
            break;
    }
}

// Converts stack top value to boolean (truthy/falsy check)
// null -> false, false -> false, everything else -> true
void generateTruthyCheck(int id) {
    printf("POPS GF@_temp_op1\n");
    printf("TYPE GF@_temp_type1 GF@_temp_op1\n");
    
    // nil -> false
    printf("JUMPIFEQ $cond_false_%d GF@_temp_type1 string@nil\n", id);
    
    // bool -> check actual value
    printf("JUMPIFNEQ $cond_not_bool_%d GF@_temp_type1 string@bool\n", id);
    printf("JUMPIFEQ $cond_false_%d GF@_temp_op1 bool@false\n", id);
    printf("JUMP $cond_true_%d\n", id);
    
    printf("LABEL $cond_not_bool_%d\n", id);
    // int, float, string -> always true
    
    printf("LABEL $cond_true_%d\n", id);
    printf("PUSHS bool@true\n");
    printf("JUMP $cond_check_end_%d\n", id);
    
    printf("LABEL $cond_false_%d\n", id);
    printf("PUSHS bool@false\n");
    
    printf("LABEL $cond_check_end_%d\n", id);
}

// Generates if-else statement with condition evaluation
void generateIfElse(AstNode* node) {
    if (node == NULL || node->ifelse.condition == NULL) return;
    
    int id = getUniqueLabelId();
    
    printf("\n# === IF Statement ===\n");
    generateExpression(node->ifelse.condition);

    generateTruthyCheck(id);
    
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS $else$%d\n", id);

    printf("# === THEN Branch ===\n");
    if (node->ifelse.true_body != NULL) {
        generateStatement(node->ifelse.true_body);
    }
    printf("JUMP $end$%d\n", id);

    printf("# === ELSE Branch ===\n");
    printf("LABEL $else$%d\n", id);
    if (node->ifelse.false_body != NULL) {
        generateStatement(node->ifelse.false_body);
    }

    printf("LABEL $end$%d\n", id);
}

// Generates while loop with condition check at start
void generateWhile(AstNode *node) {
    if(node == NULL || node->while_stmt.condition == NULL) {
        return;
    }
    int id = getUniqueLabelId();

    printf("\n# WHILE statement %d\n", id);

    printf("LABEL $while_start$%d\n", id);
    generateExpression(node->while_stmt.condition);

    generateTruthyCheck(id);

    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS $while_end$%d\n", id); 

    if(node->while_stmt.body != NULL) {
        generateStatement(node->while_stmt.body);
    }

    printf("JUMP $while_start$%d\n", id);
    printf("LABEL $while_end$%d\n", id);
}

// Helper: checks if operand is int/float and converts to float
// Exits with error 26 if wrong type
void generateCheckAndConvertToFloat(char* opVar, int id, int opNum) {
    printf("TYPE GF@_temp_type1 %s\n", opVar);
    printf("JUMPIFEQ $is_int_%d_%d GF@_temp_type1 string@int\n", id, opNum);
    printf("JUMPIFEQ $is_float_%d_%d GF@_temp_type1 string@float\n", id, opNum);
    printf("EXIT int@26\n");
    
    printf("LABEL $is_int_%d_%d\n", id, opNum);
    printf("INT2FLOAT %s %s\n", opVar, opVar);
    printf("LABEL $is_float_%d_%d\n", id, opNum);
}

// Generates code for expressions - result is pushed on stack
void generateExpression(AstNode* node) {
    if (node == NULL) return;

    printf("\n# === Expression ===\n");

    switch (node->type) {
        case NT_NUM: {
            // Numeric literal
            double val = node->num.numValue.floatValue;
            printf("PUSHS float@%a\n", val);
            break;
        }

        case NT_STRING: {
            // String literal with escaped characters
            char* escaped = escapeStringForIFJcode(node->string.value);
            printf("PUSHS string@%s\n", escaped);
            free(escaped);
            break;
        }

        case NT_VAR: {
            // Variable reference - check for global prefix
            if (node->var.name != NULL) {
                if (strncmp(node->var.name, "__", 2) == 0) {
                    printf("PUSHS GF@%s\n", node->var.name);
                } else {
                    printf("PUSHS LF@%s\n", node->var.name);
                }
            }
            break;
        }

        case NT_NULL: {
            printf("PUSHS nil@nil\n");
            break;
        }

        case NT_ARG: {
            // Function argument - just evaluate the expression
            if (node->arg.expression != NULL) {
                generateExpression(node->arg.expression);
            }
            break;
        }

        case NT_PLUS: {
            // Addition: handles both numeric add and string concatenation
            generateExpression(node->binOp.operand1);
            generateExpression(node->binOp.operand2);
            
            int id = getUniqueLabelId();
            printf("POPS GF@_temp_op2\n");
            printf("POPS GF@_temp_op1\n");
            
            printf("TYPE GF@_temp_type1 GF@_temp_op1\n");
            printf("TYPE GF@_temp_type2 GF@_temp_op2\n");
            
            // Check if string concatenation
            printf("JUMPIFNEQ $try_add_numeric_%d GF@_temp_type1 string@string\n", id);
            printf("JUMPIFNEQ $error_26_%d GF@_temp_type2 string@string\n", id);

            printf("CONCAT GF@_temp_op1 GF@_temp_op1 GF@_temp_op2\n");
            printf("PUSHS GF@_temp_op1\n");

            printf("JUMP $end_plus_%d\n", id);

            // Numeric addition
            printf("LABEL $try_add_numeric_%d\n", id);

            generateCheckAndConvertToFloat("GF@_temp_op1", id, 1);
            generateCheckAndConvertToFloat("GF@_temp_op2", id, 2);

            printf("PUSHS GF@_temp_op1\n");
            printf("PUSHS GF@_temp_op2\n");
            printf("ADDS\n");
            printf("JUMP $end_plus_%d\n", id);
            
            printf("LABEL $error_26_%d\n", id);
            printf("EXIT int@26\n");
            
            printf("LABEL $end_plus_%d\n", id);
            break;
        }

        case NT_MULT: {
            // Multiplication: numeric or string repetition
            generateExpression(node->binOp.operand1);
            generateExpression(node->binOp.operand2);
            
            int id = getUniqueLabelId();
            
            printf("POPS GF@_temp_op2\n"); 
            printf("POPS GF@_temp_op1\n"); 
            
            printf("TYPE GF@_temp_type1 GF@_temp_op1\n");
            printf("TYPE GF@_temp_type2 GF@_temp_op2\n");
            
            // Check for string repetition
            printf("JUMPIFEQ $mul_string_%d GF@_temp_type1 string@string\n", id);
            
            // Numeric multiplication
            generateCheckAndConvertToFloat("GF@_temp_op1", id, 1);
            generateCheckAndConvertToFloat("GF@_temp_op2", id, 2);
            
            printf("PUSHS GF@_temp_op1\n");
            printf("PUSHS GF@_temp_op2\n");
            printf("MULS\n");
            printf("JUMP $mul_end_%d\n", id);
            
            // String repetition: "abc" * 3 = "abcabcabc"
            printf("LABEL $mul_string_%d\n", id);
            
            printf("JUMPIFEQ $mul_chk_int_%d GF@_temp_type2 string@int\n", id);
            printf("JUMPIFEQ $mul_chk_float_%d GF@_temp_type2 string@float\n", id);
            printf("JUMP $error_26_%d\n", id);
            
            printf("LABEL $mul_chk_float_%d\n", id);
            printf("ISINT GF@_builtin_bool_result GF@_temp_op2\n");
            printf("JUMPIFNEQ $error_26_%d GF@_builtin_bool_result bool@true\n", id);
            printf("FLOAT2INT GF@_temp_op2 GF@_temp_op2\n");
            printf("JUMP $mul_loop_init_%d\n", id);
            
            printf("LABEL $mul_chk_int_%d\n", id);
            
            // String repetition loop
            printf("LABEL $mul_loop_init_%d\n", id);
            printf("MOVE GF@_builtin_result string@\n"); 
            
            printf("LABEL $mul_loop_start_%d\n", id);
            printf("GT GF@_builtin_bool_result GF@_temp_op2 int@0\n");
            printf("JUMPIFNEQ $mul_loop_end_%d GF@_builtin_bool_result bool@true\n", id);
            
            printf("CONCAT GF@_builtin_result GF@_builtin_result GF@_temp_op1\n");
            
            printf("SUB GF@_temp_op2 GF@_temp_op2 int@1\n");
            printf("JUMP $mul_loop_start_%d\n", id);
            
            printf("LABEL $mul_loop_end_%d\n", id);
            printf("PUSHS GF@_builtin_result\n");
            printf("JUMP $mul_end_%d\n", id);
            
            printf("LABEL $error_26_%d\n", id);
            printf("EXIT int@26\n");
            
            printf("LABEL $mul_end_%d\n", id);
            break;
        }

        case NT_MINUS: 
        case NT_DIV: {
            // Subtraction and division - numeric only
            generateExpression(node->binOp.operand1);
            generateExpression(node->binOp.operand2);
            
            int id = getUniqueLabelId();
            printf("POPS GF@_temp_op2\n");
            printf("POPS GF@_temp_op1\n");
            
            generateCheckAndConvertToFloat("GF@_temp_op1", id, 1);
            generateCheckAndConvertToFloat("GF@_temp_op2", id, 2);
            
            printf("PUSHS GF@_temp_op1\n");
            printf("PUSHS GF@_temp_op2\n");
            
            if (node->type == NT_MINUS) printf("SUBS\n");
            else {
                printf("DIVS\n"); 
            }
            break;
        }

        case NT_LT:
        case NT_GT:
        case NT_LE:
        case NT_GE: {
            // Comparison operators - numeric only
            generateExpression(node->binOp.operand1);
            generateExpression(node->binOp.operand2);
            
            int id = getUniqueLabelId();
            printf("POPS GF@_temp_op2\n");
            printf("POPS GF@_temp_op1\n");
            
            generateCheckAndConvertToFloat("GF@_temp_op1", id, 1);
            generateCheckAndConvertToFloat("GF@_temp_op2", id, 2);
            
            printf("PUSHS GF@_temp_op1\n");
            printf("PUSHS GF@_temp_op2\n");
            
            if (node->type == NT_LT) printf("LTS\n");
            else if (node->type == NT_GT) printf("GTS\n");
            else if (node->type == NT_LE) { printf("GTS\nNOTS\n"); }  // <= is NOT >
            else if (node->type == NT_GE) { printf("LTS\nNOTS\n"); }  // >= is NOT <
            break;
        }

        case NT_EQ:
        case NT_NEQ: {
            // Equality operators - works with any types
            generateExpression(node->binOp.operand1);
            generateExpression(node->binOp.operand2);
            int id = getUniqueLabelId();
            
            printf("POPS GF@_temp_op2\n");
            printf("POPS GF@_temp_op1\n");
            
            printf("TYPE GF@_temp_type1 GF@_temp_op1\n");
            printf("TYPE GF@_temp_type2 GF@_temp_op2\n");
            
            // Same types -> compare directly
            printf("JUMPIFEQ $eq_do_%d GF@_temp_type1 GF@_temp_type2\n", id);
            
            // nil comparison always allowed
            printf("JUMPIFEQ $eq_do_%d GF@_temp_type1 string@nil\n", id);
            printf("JUMPIFEQ $eq_do_%d GF@_temp_type2 string@nil\n", id);
            
            // Different types (non-nil) -> not equal
            printf("PUSHS bool@false\n");
            printf("JUMP $eq_end_%d\n", id);
            
            printf("LABEL $eq_do_%d\n", id);
            printf("PUSHS GF@_temp_op1\n");
            printf("PUSHS GF@_temp_op2\n");
            printf("EQS\n");
            
            printf("LABEL $eq_end_%d\n", id);
            
            if (node->type == NT_NEQ) printf("NOTS\n");
            break;
        }

        case NT_FUNC_CALL:
            // Function call in expression context
            generateFunctionCall(node);
            break;

        default:
            fprintf(stderr, "Warning: Expression type %d not implemented\n", node->type);
            break;
    }
}

// Generates function call - pushes args and calls function
void generateFunctionCall(AstNode* node) {
    if (node == NULL || node->call.name == NULL) return;
    
    // Built-in functions have "Ifj." prefix
    if (strncmp(node->call.name, "Ifj.", 4) == 0) {
        generateBuiltinFunction(node->call.name + 4, node->call.args);
        return;
    }
    
    printf("# Calling user function %s\n", node->call.name);
    
    // Count arguments
    AstNode* arg = node->call.args;
    int argCount = 0;
    while (arg != NULL) {
        argCount++;
        arg = arg->next;
    }
    
    // Push arguments in reverse order (last arg first)
    for (int i = argCount - 1; i >= 0; i--) {
        arg = node->call.args;
        for (int j = 0; j < i; j++) {
            arg = arg->next;
        }
        if (arg != NULL) {
            generateExpression(arg);
        }    
    }
    printf("CREATEFRAME\n");
    printf("CALL %s\n", node->call.name);
}

// Generates code for built-in IFJ functions
void generateBuiltinFunction(const char* funcName, AstNode* args) {
    if (funcName == NULL) return;
    
    // write(term) - prints value to stdout
    if (strcmp(funcName, "write") == 0) {
        if (args != NULL) {
            int id = getUniqueLabelId();

            generateExpression(args);
            printf("POPS GF@_builtin_result\n");

            printf("TYPE GF@_temp_type1 GF@_builtin_result\n");
            printf("JUMPIFNEQ $write_print_%d GF@_temp_type1 string@float\n", id);

            // Float that is whole number -> print as int
            printf("FLOAT2INT GF@_builtin_i GF@_builtin_result\n");
            printf("INT2FLOAT GF@_temp_op1 GF@_builtin_i\n");
            printf("JUMPIFNEQ $write_print_%d GF@_temp_op1 GF@_builtin_result\n", id);

            printf("WRITE GF@_builtin_i\n");
            printf("JUMP $write_end_%d\n", id);

            printf("LABEL $write_print_%d\n", id);

            printf("WRITE GF@_builtin_result\n");
            
            printf("LABEL $write_end_%d\n", id);
        }
        printf("PUSHS nil@nil\n");
    }
    // read_str() - reads string from stdin
    else if (strcmp(funcName, "read_str") == 0) {
        printf("READ GF@_builtin_result string\n");
        printf("PUSHS GF@_builtin_result\n");
    }
    // read_num() - reads number from stdin
    else if (strcmp(funcName, "read_num") == 0) {
        printf("READ GF@_builtin_result float\n");
        printf("PUSHS GF@_builtin_result\n");
    }
    // floor(n) - returns floor of number as float
    else if (strcmp(funcName, "floor") == 0) {
        if (args != NULL) {
            int id = getUniqueLabelId();
            generateExpression(args);
            printf("POPS GF@_builtin_s\n");
            
            // Type check - only numbers allowed (error 25)
            printf("TYPE GF@_temp_type1 GF@_builtin_s\n");
            printf("JUMPIFEQ $floor_ok_%d GF@_temp_type1 string@float\n", id);
            printf("JUMPIFEQ $floor_ok_%d GF@_temp_type1 string@int\n", id);
            printf("EXIT int@25\n");

            printf("LABEL $floor_is_int_%d\n", id);
            printf("INT2FLOAT GF@_builtin_s GF@_builtin_s\n");
            printf("PUSHS GF@_builtin_s\n");
            printf("JUMP $floor_end_%d\n", id);
            
            printf("LABEL $floor_is_float_%d\n", id);
            printf("PUSHS GF@_builtin_s\n");
            printf("FLOAT2INTS\n");
            printf("INT2FLOATS\n");
            printf("JUMP $floor_end_%d\n", id);

            printf("LABEL $floor_end_%d\n", id);
        } else { 
            printf("PUSHS nil@nil\n");
        }
    }
    // str(term) - converts value to string
    else if (strcmp(funcName, "str") == 0) {
        if (args != NULL) {
            int id = getUniqueLabelId();

            generateExpression(args);
            printf("POPS GF@_builtin_s\n");
            
            printf("TYPE GF@_temp_type1 GF@_builtin_s\n");

            printf("JUMPIFEQ $str_is_str_%d GF@_temp_type1 string@string\n", id);
            printf("JUMPIFEQ $str_is_nil_%d GF@_temp_type1 string@nil\n", id);
            printf("JUMPIFEQ $str_is_int_%d GF@_temp_type1 string@int\n", id);
            printf("JUMPIFEQ $str_is_float_%d GF@_temp_type1 string@float\n", id);
            
            printf("PUSHS string@\n");
            printf("JUMP $str_end_%d\n", id);

            printf("LABEL $str_is_str_%d\n", id);
            printf("PUSHS GF@_builtin_s\n");
            printf("JUMP $str_end_%d\n", id);
            
            printf("LABEL $str_is_nil_%d\n", id);
            printf("PUSHS string@null\n");
            printf("JUMP $str_end_%d\n", id);
            
            printf("LABEL $str_is_int_%d\n", id);
            printf("PUSHS GF@_builtin_s\n");
            printf("INT2STRS\n");
            printf("JUMP $str_end_%d\n", id);
            
            printf("LABEL $str_is_float_%d\n", id);
            printf("PUSHS GF@_builtin_s\n");
            printf("FLOAT2STRS\n");
            printf("JUMP $str_end_%d\n", id);
            
            printf("LABEL $str_end_%d\n", id);
        } else {
            printf("PUSHS nil@nil\n");
        }
    }
    // length(s) - returns string length
    else if (strcmp(funcName, "length") == 0) {
        if (args != NULL) {
            int id = getUniqueLabelId();
            generateExpression(args);
            printf("POPS GF@_builtin_s\n");
            
            // Type check - string only (error 25)
            printf("TYPE GF@_temp_type1 GF@_builtin_s\n");
            printf("JUMPIFNEQ $len_err_%d GF@_temp_type1 string@string\n", id);
            
            printf("STRLEN GF@_builtin_i GF@_builtin_s\n");
            printf("INT2FLOAT GF@_builtin_result GF@_builtin_i\n");
            printf("PUSHS GF@_builtin_result\n");
            printf("JUMP $len_end_%d\n", id);
            
            printf("LABEL $len_err_%d\n", id);
            printf("EXIT int@25\n");
            
            printf("LABEL $len_end_%d\n", id);
        } else {
            printf("PUSHS nil@nil\n");
        }
    }
    // strcmp(s1, s2) - compares two strings, returns -1, 0, or 1
    else if (strcmp(funcName, "strcmp") == 0) {
        if (args != NULL && args->next != NULL) {
            int id = getUniqueLabelId();

            generateExpression(args);
            printf("POPS GF@_temp_op1\n");
            generateExpression(args->next);
            printf("POPS GF@_temp_op2\n");
            
            // Type check - both must be strings
            printf("TYPE GF@_temp_type1 GF@_temp_op1\n");
            printf("TYPE GF@_temp_type2 GF@_temp_op2\n");

            printf("JUMPIFNEQ $cmp_err_%d GF@_temp_type1 string@string\n", id);
            printf("JUMPIFNEQ $cmp_err_%d GF@_temp_type2 string@string\n", id);
            
            // Check equality first
            printf("PUSHS GF@_temp_op1\n");
            printf("PUSHS GF@_temp_op2\n");
            printf("EQS\n");
            printf("POPS GF@_builtin_bool_result\n");
            printf("JUMPIFEQ $cmp_0_%d GF@_builtin_bool_result bool@true\n", id);
            
            // Check less than
            printf("PUSHS GF@_temp_op1\n");
            printf("PUSHS GF@_temp_op2\n");
            printf("LTS\n");
            printf("POPS GF@_builtin_bool_result\n");
            printf("JUMPIFEQ $cmp_m1_%d GF@_builtin_bool_result bool@true\n", id);
            
            // Otherwise greater than
            printf("PUSHS float@1.0\n");
            printf("JUMP $cmp_end_%d\n", id);
            
            printf("LABEL $cmp_0_%d\n", id);
            printf("PUSHS float@0.0\n");
            printf("JUMP $cmp_end_%d\n", id);
            
            printf("LABEL $cmp_m1_%d\n", id);
            printf("PUSHS float@-1.0\n");
            printf("JUMP $cmp_end_%d\n", id);
            
            printf("LABEL $cmp_err_%d\n", id);
            printf("EXIT int@25\n");
            
            printf("LABEL $cmp_end_%d\n", id);
        } else {
            printf("PUSHS nil@nil\n");
        }
    }
    // substring(s, i, j) - returns substring from index i to j
    else if (strcmp(funcName, "substring") == 0) {
        if (args != NULL && args->next != NULL && args->next->next != NULL) {
            int id = getUniqueLabelId();
            generateExpression(args);
            printf("POPS GF@_builtin_s\n");
            generateExpression(args->next);
            printf("POPS GF@_builtin_i\n");
            generateExpression(args->next->next);
            printf("POPS GF@_builtin_j\n");
            
            // Type check - s must be string
            printf("TYPE GF@_temp_type1 GF@_builtin_s\n");
            printf("JUMPIFNEQ $sub_err_%d GF@_temp_type1 string@string\n", id);

            // Convert indices to int
            generateCheckAndConvertToFloat("GF@_builtin_i", id, 11); 
            printf("FLOAT2INT GF@_builtin_i GF@_builtin_i\n");
            generateCheckAndConvertToFloat("GF@_builtin_j", id, 22);
            printf("FLOAT2INT GF@_builtin_j GF@_builtin_j\n");
            
            printf("STRLEN GF@_builtin_len GF@_builtin_s\n");

            // Bounds checking: i<0 or j<0 or i>j or i>=len or j>len -> nil
            printf("LT GF@_builtin_bool_result GF@_builtin_i int@0\n");
            printf("JUMPIFEQ $sub_nil_%d GF@_builtin_bool_result bool@true\n", id);

            printf("LT GF@_builtin_bool_result GF@_builtin_j int@0\n");
            printf("JUMPIFEQ $sub_nil_%d GF@_builtin_bool_result bool@true\n", id);

            printf("GT GF@_builtin_bool_result GF@_builtin_i GF@_builtin_j\n");
            printf("JUMPIFEQ $sub_nil_%d GF@_builtin_bool_result bool@true\n", id);

            printf("LT GF@_builtin_bool_result GF@_builtin_i GF@_builtin_len\n");
            printf("JUMPIFEQ $sub_nil_%d GF@_builtin_bool_result bool@false\n", id);

            printf("GT GF@_builtin_bool_result GF@_builtin_j GF@_builtin_len\n");
            printf("JUMPIFEQ $sub_nil_%d GF@_builtin_bool_result bool@true\n", id);

            // Build substring character by character
            printf("MOVE GF@_builtin_result string@\n");
            printf("LABEL $sub_loop_%d\n", id);
            printf("LT GF@_builtin_bool_result GF@_builtin_i GF@_builtin_j\n");
            printf("JUMPIFEQ $sub_loop_end_%d GF@_builtin_bool_result bool@false\n", id);

            printf("GETCHAR GF@_builtin_char GF@_builtin_s GF@_builtin_i\n");
            printf("CONCAT GF@_builtin_result GF@_builtin_result GF@_builtin_char\n");
            printf("ADD GF@_builtin_i GF@_builtin_i int@1\n");
            printf("JUMP $sub_loop_%d\n", id);

            printf("LABEL $sub_loop_end_%d\n", id);
            printf("PUSHS GF@_builtin_result\n");
            printf("JUMP $sub_end_%d\n", id);

            printf("LABEL $sub_nil_%d\n", id);
            printf("PUSHS nil@nil\n");
            printf("JUMP $sub_end_%d\n", id);

            printf("LABEL $sub_err_%d\n", id);
            printf("EXIT int@25\n");

            printf("LABEL $sub_end_%d\n", id);
        } else {
            printf("PUSHS nil@nil\n");
        }
    }
    // ord(s, i) - returns ASCII value of character at index i
    else if (strcmp(funcName, "ord") == 0) {
        if (args != NULL && args->next != NULL) {
            int id = getUniqueLabelId();
            generateExpression(args);
            printf("POPS GF@_builtin_s\n");
            generateExpression(args->next);
            printf("POPS GF@_builtin_i\n");

            // Type check - s must be string
            printf("TYPE GF@_temp_type1 GF@_builtin_s\n");
            printf("JUMPIFNEQ $ord_err_%d GF@_temp_type1 string@string\n", id);

            generateCheckAndConvertToFloat("GF@_builtin_i", id, 1);
            printf("FLOAT2INT GF@_builtin_i GF@_builtin_i\n");
            
            printf("STRLEN GF@_builtin_len GF@_builtin_s\n");
            
            // Bounds check: i < 0 or i >= len -> return 0
            printf("LT GF@_builtin_bool_result GF@_builtin_i int@0\n");
            printf("JUMPIFEQ $ord_ret0_%d GF@_builtin_bool_result bool@true\n", id);
            printf("LT GF@_builtin_bool_result GF@_builtin_i GF@_builtin_len\n");
            printf("JUMPIFEQ $ord_ret0_%d GF@_builtin_bool_result bool@false\n", id);
            
            printf("STRI2INT GF@_builtin_result GF@_builtin_s GF@_builtin_i\n");
            printf("INT2FLOAT GF@_builtin_result GF@_builtin_result\n");
            printf("PUSHS GF@_builtin_result\n");
            printf("JUMP $ord_end_%d\n", id);
            
            printf("LABEL $ord_ret0_%d\n", id);
            printf("PUSHS float@%a\n", 0.0);
            printf("JUMP $ord_end_%d\n", id);
            
            printf("LABEL $ord_err_%d\n", id);
            printf("EXIT int@25\n");
            
            printf("LABEL $ord_end_%d\n", id);
        } else {
            printf("PUSHS nil@nil\n");
        }
    }
    // chr(i) - returns character with given ASCII value
    else if (strcmp(funcName, "chr") == 0) {
        if (args != NULL) {
            int id = getUniqueLabelId();
            generateExpression(args);
            printf("POPS GF@_builtin_i\n");
            
            generateCheckAndConvertToFloat("GF@_builtin_i", id, 1);
            printf("FLOAT2INT GF@_builtin_i GF@_builtin_i\n");

            // Range check: 0-255
            printf("LT GF@_builtin_bool_result GF@_builtin_i int@0\n");
            printf("JUMPIFEQ $chr_err_%d GF@_builtin_bool_result bool@true\n", id);
            printf("GT GF@_builtin_bool_result GF@_builtin_i int@255\n");
            printf("JUMPIFEQ $chr_err_%d GF@_builtin_bool_result bool@true\n", id);
            
            printf("INT2CHAR GF@_builtin_result GF@_builtin_i\n");
            printf("PUSHS GF@_builtin_result\n");
            printf("JUMP $chr_end_%d\n", id);
            
            printf("LABEL $chr_err_%d\n", id);
            printf("EXIT int@58\n");
            
            printf("LABEL $chr_end_%d\n", id);
        } else {
            printf("PUSHS nil@nil\n");
        }
    }
    else {
        fprintf(stderr, "Warning: Built-in function %s not implemented\n", funcName);
        printf("PUSHS nil@nil\n");
    }
}