#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "error.h"

#ifndef SCANNER_H
#define SCANNER_H

typedef enum {

    TOKEN_KEYWORD_CLASS,            // 'class'
    TOKEN_KEYWORD_IF,               // 'if'
    TOKEN_KEYWORD_ELSE,             // 'else'
    TOKEN_KEYWORD_IS,               // 'is'
    TOKEN_KEYWORD_NULL_SMALL,       // 'null'
    TOKEN_KEYWORD_RETURN,           // 'return'
    TOKEN_KEYWORD_VAR,              // 'var'
    TOKEN_KEYWORD_WHILE,            // 'while'
    TOKEN_KEYWORD_IFJ,              // 'Ifj'
    TOKEN_KEYWORD_STATIC,           // 'static'
    TOKEN_KEYWORD_TRUE,             // 'true'
    TOKEN_KEYWORD_FALSE,            // 'false'
    TOKEN_KEYWORD_NUM,              // 'Num'
    TOKEN_KEYWORD_STRING,           // 'String'
    TOKEN_KEYWORD_NULL_BIG,         // 'Null'
    
    TOKEN_LOCAL_IDENTIFIER,         // local variables, user functions, getters      
    TOKEN_GLOBAL_IDENTIFIER,        // global variables
    TOKEN_DOT,                      // '.'
    TOKEN_LITERAL_INT,              // integer
    TOKEN_LITERAL_HEX,              // hex number - '0X456'
    TOKEN_LITERAL_FLOAT,            // floating point number (also numbers with exponent)    
    TOKEN_PLUS,                     // '+'
    TOKEN_MINUS,                    // '-'
    TOKEN_MULTIPLY,                 // '*'
    TOKEN_DIVIDE,                   // '/'
    TOKEN_EQUALS,                   // '='

    TOKEN_ESCAPED_N,                // newline
    TOKEN_ESCAPED_R,                // carriage return
    TOKEN_ESCAPED_T,                // tab
    TOKEN_ESCAPED_BACKSLASH,        // '\\'
    TOKEN_ESCAPED_QUOTES,           // '\"'
    TOKEN_ESCAPED_HEX,              // '\x...' (complete hex number)

    TOKEN_GREATER,                   // '>'
    TOKEN_LESS,                     // '<'
    TOKEN_DOUBLE_EQUAL,             // '=='
    TOKEN_GREATER_EQUAL,            // '>='
    TOKEN_LESS_EQUAL,              // '<='
    TOKEN_NOT_EQUAL,                // '!='

    TOKEN_QUOTES,                   // ' " '  
    TOKEN_STRING_SYMBOL,            // one symbol in string literal
    TOKEN_ONELINE_COMMENT,          // ' // ... \n'
    TOKEN_MULTILINE_COMMENT,        // ' /* ... */ '

    TOKEN_BRACKET_OPEN,             // '('
    TOKEN_BRACKET_CLOSE,            // ')'
    TOKEN_CBRACKET_OPEN,            // '{'
    TOKEN_CBRACKET_CLOSE,           // '}'

    TOKEN_EOF                       // end of file

} token_type;

// token structure
typedef struct {

    token_type type; 
    char *token_str;
    int size;       

} Token;

// FSM states 
typedef enum {

    START, 
    q0, q1, q2, q3, q4,
    q5, q6, q7, q8, q9,
    q10,q11,q12,q13,q14,
    q15,q16,q17,q18,q19,
    q20,q21,q22,q23,q24,
    q25,q26,q27,q28

} fsm_state;


Token *createToken(token_type type, char *token_str, int size);
Token *getToken(char *input_buffer);
void strcatChar(char *destination, char c);
char checkPrev(char *input_buffer, int index);
token_type isKeyword(char *word);

extern int current_index;
extern bool in_string;
extern bool in_hex;
extern bool in_hex_string;
extern bool in_oneline_comment;
extern bool in_multiline_comment;

#endif