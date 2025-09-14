/**
 * @file tokeniser.h
 * @brief Header for tokeniser.
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */


#ifndef TOKENISER_H
#define TOKENISER_H

#include <stdio.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_KEYWORD_CONST,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_FN,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_I32,
    TOKEN_KEYWORD_F64,
    TOKEN_KEYWORD_NULL,
    TOKEN_KEYWORD_PUB,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_U8,
    TOKEN_KEYWORD_VAR,
    TOKEN_KEYWORD_VOID,
    TOKEN_KEYWORD_WHILE,
    TOKEN_STRING_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_INT_LITERAL,
    TOKEN_SEPARATOR,
    TOKEN_OPERATOR,
    TOKEN_EQUAL, // ==
    TOKEN_NOT_EQUAL, // !=
    TOKEN_LESS, // <
    TOKEN_LESS_EQUAL, // <=
    TOKEN_GREATER, // >
    TOKEN_GREATER_EQUAL, // >=
    TOKEN_ASSIGN, // =
    TOKEN_ADD, // +
    TOKEN_SUB, // -
    TOKEN_MUL, // *
    TOKEN_DIV,  // /
    TOKEN_LEFT_PAREN, // (
    TOKEN_RIGHT_PAREN, // )
    TOKEN_LEFT_BRACE, // {
    TOKEN_RIGHT_BRACE, // }
    TOKEN_LEFT_BRACKET, // [
    TOKEN_RIGHT_BRACKET, // ]
    TOKEN_COMMA, // ,
    TOKEN_DOT, // .
    TOKEN_COL, // :
    TOKEN_SEMICOL, // ;
    TOKEN_PIPE, // |
    TOKEN_QM, // ?
    TOKEN_STACK_BOTTOM,
    TOKEN_QUOTEMARK,
    TOKEN_IMPORT,
    TOKEN_EOF,
} token_type;

typedef struct {
    token_type type;
    int param_length;
    char param[];
} Token;

Token* getToken(FILE *file);
Token* createToken(token_type type, const char *text, int length);
int isKeyword(const char *text);

#endif
