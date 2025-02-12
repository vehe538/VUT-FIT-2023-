/**
 * @file tokeniser.c
 * @brief Tokeniser implementation.
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
#include <ctype.h>
#include <string.h>
#include "error.h"
#include "tokeniser.h"

const char *zig_keywords[] = {
        "const", "else", "fn", "if", "i32", "f64", "null", "pub", "return", "u8", "var", "void", "while",
};

// create a new token
Token* createToken(token_type type, const char *text, int length) {
    Token *token = malloc(sizeof(Token) + length + 1);
    if (!token) return NULL;

    token->type = type;
    token->param_length = length;

    strncpy(token->param, text, length);
    token->param[length] = '\0';
    return token;
}

// check if text is a keyword
int isKeyword(const char *text) {
    for (size_t i = 0; i < sizeof(zig_keywords) / sizeof(zig_keywords[0]); i++) {
        if (strcmp(text, zig_keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static char lookahead = 0;  // lookahead character

char nextChar(FILE *file) {
    if (lookahead != 0) {
        char temp = lookahead;
        lookahead = 0;
        return temp;
    }
    return fgetc(file);
}

void pushBack(char c) {
    lookahead = c;
}

Token* getToken(FILE *file) {
    int index = 0;
    char buffer[256] = {0};

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        index = 0;

        // getting next character
        char c = nextChar(file);

        // EoF
        if (c == EOF) {
            return NULL;
        }

        // skip whitespaces
        if (isspace(c)) {
            continue;
        }


        // comments
        if (c == '/') {
            char next = nextChar(file);
            if (next == '/') {
                while ((c = nextChar(file)) != '\n' && c != EOF);
                continue;
            } else if (next == '*') {   // multiline comment
                while ((c = nextChar(file)) != EOF) {
                    if (c == '*') {
                        if ((next = nextChar(file)) == '/') {   // end multiline of comment
                            break;
                        }
                        pushBack(next);
                    }
                }
                continue;
            }
            pushBack(next);
            buffer[index++] = c;
        }

        // identifier
        else if (isalpha(c) || c == '_' || c == '@') {
            buffer[index++] = c;

            while (isalnum(c = nextChar(file)) || c == '_') {
                buffer[index++] = c;
            }

            pushBack(c);
            buffer[index] = '\0';

            // keyword check
            if (isKeyword(buffer)) {    // We have a keyword, now return the correct token
                if (strcmp(buffer, "const") == 0) return createToken(TOKEN_KEYWORD_CONST, buffer, 0);
                if (strcmp(buffer, "else") == 0) return createToken(TOKEN_KEYWORD_ELSE, buffer, 0);
                if (strcmp(buffer, "fn") == 0) return createToken(TOKEN_KEYWORD_FN, buffer, 0);
                if (strcmp(buffer, "if") == 0) return createToken(TOKEN_KEYWORD_IF, buffer, 0);
                if (strcmp(buffer, "i32") == 0) return createToken(TOKEN_KEYWORD_I32, buffer, 0);
                if (strcmp(buffer, "f64") == 0) return createToken(TOKEN_KEYWORD_F64, buffer, 0);
                if (strcmp(buffer, "null") == 0) return createToken(TOKEN_KEYWORD_NULL, buffer, 0);
                if (strcmp(buffer, "pub") == 0) return createToken(TOKEN_KEYWORD_PUB, buffer, 0);
                if (strcmp(buffer, "return") == 0) return createToken(TOKEN_KEYWORD_RETURN, buffer, 0);
                if (strcmp(buffer, "u8") == 0) return createToken(TOKEN_KEYWORD_U8, buffer, 0);
                if (strcmp(buffer, "var") == 0) return createToken(TOKEN_KEYWORD_VAR, buffer, 0);
                if (strcmp(buffer, "void") == 0) return createToken(TOKEN_KEYWORD_VOID, buffer, 0);
                if (strcmp(buffer, "while") == 0) return createToken(TOKEN_KEYWORD_WHILE, buffer, 0);
            }

            if (strcmp(buffer, "@import") == 0) return createToken(TOKEN_IMPORT, buffer, 0); // prolog
            return createToken(TOKEN_IDENTIFIER, buffer, index);
        }
        
        // number / float
        else if (isdigit(c)) {
            buffer[index++] = c;
            int is_float = 0;

            // check if next character is a digit or a dot
            while (isdigit(c = nextChar(file)) || c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-') {
                if (c == '.') { // now we know it is a float
                    if (is_float) break;  
                    is_float = 1;
                } else if (c == 'e' || c == 'E') { // float in exponent form
                    if (buffer[index - 1] == '.' || buffer[index - 1] == 'e' || buffer[index - 1] == 'E') break;
                    is_float = 1;
                }
                buffer[index++] = c;
            }

            pushBack(c); 
            buffer[index] = '\0';

            if (is_float) {
                return createToken(TOKEN_FLOAT_LITERAL, buffer, index);
            } else {
                return createToken(TOKEN_INT_LITERAL, buffer, index);
            }
        }
        
        // operators and separators
        else if (ispunct(c)) {
            buffer[index++] = c;

            // check for two-character operators
            char next = nextChar(file);
            if ((c == '=' || c == '!' || c == '<' || c == '>') && next == '=') {
                buffer[index++] = next;
                buffer[index] = '\0';

                if (strcmp(buffer, "==") == 0) return createToken(TOKEN_EQUAL, buffer, 0);
                if (strcmp(buffer, "!=") == 0) return createToken(TOKEN_NOT_EQUAL, buffer, 0);
                if (strcmp(buffer, "<=") == 0) return createToken(TOKEN_LESS_EQUAL, buffer, 0);
                if (strcmp(buffer, ">=") == 0) return createToken(TOKEN_GREATER_EQUAL, buffer, 0);
            } else {
                pushBack(next);
                buffer[index] = '\0';

                // qoutes
                if(c == '"') {               
                    while ((c = nextChar(file)) != '"') {
                        if (c == EOF) {
                            pushError("Unexpected EOF in string literal", 1);
                        }

                        // escapeing
                        if (c == '\\') {
                            char next = nextChar(file);
                            if (next == '"') {
                                buffer[index++] = next;
                            } else {
                                buffer[index++] = '\\';
                                buffer[index++] = next;
                            }
                        } else {
                            buffer[index++] = c;
                        }
                    }

                    buffer[index] = '\0';
                    memmove(buffer, buffer + 1, index - 1);
                    buffer[index - 1] = '\0';
                    return createToken(TOKEN_STRING_LITERAL, buffer, index);
                }

                // single character operators
                switch (c) {
                    case '=': return createToken(TOKEN_ASSIGN, buffer, 0);
                    case '<': return createToken(TOKEN_LESS, buffer, 0);
                    case '>': return createToken(TOKEN_GREATER, buffer, 0);
                    case '+': return createToken(TOKEN_ADD, buffer, 0);
                    case '-': return createToken(TOKEN_SUB, buffer, 0);
                    case '*': return createToken(TOKEN_MUL, buffer, 0);
                    case '/': return createToken(TOKEN_DIV, buffer, 0);
                    case '(': return createToken(TOKEN_LEFT_PAREN, buffer, 0);
                    case ')': return createToken(TOKEN_RIGHT_PAREN, buffer, 0);
                    case '{': return createToken(TOKEN_LEFT_BRACE, buffer, 0);
                    case '}': return createToken(TOKEN_RIGHT_BRACE, buffer, 0);
                    case '[': return createToken(TOKEN_LEFT_BRACKET, buffer, 0);
                    case ']': return createToken(TOKEN_RIGHT_BRACKET, buffer, 0);
                    case ',': return createToken(TOKEN_COMMA, buffer, 0);
                    case '.': return createToken(TOKEN_DOT, buffer, 0);
                    case ':': return createToken(TOKEN_COL, buffer, 0);
                    case ';': return createToken(TOKEN_SEMICOL, buffer, 0);
                    case '|': return createToken(TOKEN_PIPE, buffer, 0);
                    case '?': return createToken(TOKEN_QM, buffer, 0);
                    default: return createToken(TOKEN_SEPARATOR, buffer, index);
                }
            }
        }
    }
}
