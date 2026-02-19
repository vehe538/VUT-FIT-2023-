/**
 * @file error_codes.h
 * @brief List of macros for error handling.
 * 
 * 
 * @author
 *  - Eliška Krejčíková (xkrejce00)
 */


#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#define COMPILATION_OK 0

#define LEXER_ERROR 1
#define SYNTAX_ERROR 2

#define SEMANTIC_NONDEF_ERROR 3
#define SEMANTIC_REDEFINITION_ERROR 4
// wrong type with builtin or wrong number of params
#define SEMANTIC_PARAM_ERROR 5
// type in expressions
#define SEMANTIC_TYPE_EXPR_ERROR 6
#define SEMANTIC_OTHER_ERROR 10
#define INTERNAL_ERROR 99

// wrong param type in builtin functions during runtime
#define SEMANTIC_RUNTIME_TYPE_ERROR 25
// wrong type in expressions during runtime
#define SEMANTIC_RUNTIME_TYPE_EXPR_ERROR 26


#endif