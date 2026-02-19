/**
* @file scanner.c
 * @brief Main lexical analysis implementation; functions' definitions for tokenising input from stdin using FSM.
 *
 *
 * Authors:
 * - Juraj Mesík        (xmesikj00), 2025
 * - Natália Holbíková  (xholbin00), 2025
 * - Eliška Krejčíková  (xkrejce00), 2025
 */


#define _DEFAULT_SOURCE
#include "scanner.h"


// helper function for creating tokens with safe memory management
Token *createToken(token_type type, const char *token_str, int size) {
    Token *new_token = malloc(sizeof(Token));
    if (!new_token) {
        printError(99, "malloc for Token failed");
        exit(99);
    }
    new_token->type = type;
    new_token->token_str = strdup(token_str);
    if (!new_token->token_str) {
        printError(99, "strdup for token_str failed");
        free(new_token);
        exit(99);
    }
    new_token->size = size;
    
    // set data_type based on token type
    switch (type) {
        case TOKEN_LITERAL_INT:
        case TOKEN_LITERAL_HEX:
        case TOKEN_LITERAL_FLOAT:
            new_token->data_type = DATA_TYPE_NUM;
            break;
        case TOKEN_LITERAL_STRING:
        case TOKEN_LITERAL_MULTILINE_STRING:
            new_token->data_type = DATA_TYPE_STRING;
            break;
        case TOKEN_KEYWORD_NULL_SMALL:
            new_token->data_type = DATA_TYPE_NULL;
            break;
        case TOKEN_KEYWORD_TRUE:
        case TOKEN_KEYWORD_FALSE:
            new_token->data_type = DATA_TYPE_BOOL;
            break;
        default:
            new_token->data_type = DATA_TYPE_UNKNOWN;
            break;
    }
    
    return new_token;
}

// helper function to check if identifier is keyword
token_type isKeyword(char *word) {
    if (strcmp(word, "class") == 0) return TOKEN_KEYWORD_CLASS;
    if (strcmp(word, "if") == 0) return TOKEN_KEYWORD_IF;
    if (strcmp(word, "else") == 0) return TOKEN_KEYWORD_ELSE;
    if (strcmp(word, "is") == 0) return TOKEN_KEYWORD_IS;
    if (strcmp(word, "null") == 0) return TOKEN_KEYWORD_NULL_SMALL;
    if (strcmp(word, "return") == 0) return TOKEN_KEYWORD_RETURN;
    if (strcmp(word, "var") == 0) return TOKEN_KEYWORD_VAR;
    if (strcmp(word, "while") == 0) return TOKEN_KEYWORD_WHILE;
    if (strcmp(word, "Ifj") == 0) return TOKEN_KEYWORD_IFJ;
    if (strcmp(word, "static") == 0) return TOKEN_KEYWORD_STATIC;
    if (strcmp(word, "true") == 0) return TOKEN_KEYWORD_TRUE;
    if (strcmp(word, "false") == 0) return TOKEN_KEYWORD_FALSE;
    if (strcmp(word, "Num") == 0) return TOKEN_KEYWORD_NUM;
    if (strcmp(word, "String") == 0) return TOKEN_KEYWORD_STRING;
    if (strcmp(word, "Null") == 0) return TOKEN_KEYWORD_NULL_BIG;    
    return (token_type)-1;
}

// global vars
bool in_string = false;             // flag variable for string handling
bool in_multiline_comment = false;  // flag variable for handling multiline comments
bool in_multiline_string = false;   // flag variable for handling multiline strings
int multiline_comment_depth = 0;    // variable that indicates number of current line when handling  with multiline strings/comments

// safe dynamic string append
static void appendChar(char **str, size_t *len, size_t *capacity, char c) {
    if (*len + 1 >= *capacity) {
        *capacity = (*capacity == 0) ? 8 : *capacity * 2;
        char *new_str = realloc(*str, *capacity);
        if (!new_str) {
            printError(99, "realloc fail");
            if (*str) free(*str);
            exit(99);
        }
        *str = new_str;
    }
    (*str)[(*len)++] = c;
    (*str)[*len] = '\0';
}

// main function for retrieving tokens from input
Token *getToken() {
    Token *token = NULL;
    size_t str_len = 0;
    size_t str_capacity = 0;
    char *str_repr = NULL;
    char c;
    
    fsm_state current_state = START;
    
    // this loop implements FSM for lexixal analysis
    // includes definitions of FSM states from START to q29
    while(true) {
        c = getc(stdin);

        if (c == EOF) {
            switch (current_state) {
                case START:
                    if (str_repr) free(str_repr);
                    return createToken(TOKEN_EOF, "", 0);
                case q1:
                case q3:
                case q6:
                    if (current_state == q1) token = createToken(TOKEN_LITERAL_INT, str_repr, str_len);
                    else token = createToken(TOKEN_LITERAL_FLOAT, str_repr, str_len);
                    free(str_repr);
                    return token;
                case q7:
                case q28:
                    break; 
                
                case q2:
                case q4:
                case q5:
                case q0:
                case q27:
                case q8:
                case q29:
                case q17:
                case q19:
                    printError(1, "Lexical error: Unexpected EOF in incomplete token");
                    exit(1);
                
                default:
                    break;
            }
        }

        switch (current_state) {
            
            case START:
                if (c == EOF) {
                    return createToken(TOKEN_EOF, "", 0);
                }

                if (c == '\n') {
                    return createToken(TOKEN_EOL, "\n", 1);
                }

                if (isspace(c) && !in_string && !in_multiline_string) {

                    continue;
                }
                
                if (str_repr) {
                    free(str_repr);
                    str_repr = NULL;
                }
                str_len = 0;
                str_capacity = 0;
                
                if (in_string) {
                    current_state = q8;
                    ungetc(c, stdin);
                    continue;
                }
                
                if (in_multiline_string) {
                    current_state = q29;
                    ungetc(c, stdin);
                    continue;
                }
                
                if (c == '"') {
                    char c1 =  getc(stdin);
                    char c2 =  getc(stdin);
                    if (c1 == '"' && c2== '"') {
                        in_multiline_string = true;
                        current_state = q29;
                        continue;
                    } else {
                        ungetc(c2, stdin);
                        ungetc(c1, stdin);
                        in_string = true;
                        current_state = q8;
                        continue;
                    }
                }
                
                if (isalpha(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q7;
                    continue;
                }
                
                if (isdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    
                    if (c == '0') {
                        char c1 = getc(stdin);
                        if (c1 == 'x' || c1 == 'X') {
                            appendChar(&str_repr, &str_len, &str_capacity, c1);
                            current_state = q0;
                        } else if (c1 == '.') {
                            appendChar(&str_repr, &str_len, &str_capacity, c1);
                            current_state = q2;
                        } else if (isdigit(c1)) {
                            printError(1, "Lexical error: Leading zeros not allowed");
                            exit(1);
                        } else {
                            ungetc(c1, stdin);
                            token = createToken(TOKEN_LITERAL_INT, str_repr, str_len);
                            free(str_repr);
                            return token;
                        }
                    } else {
                        current_state = q1;
                    }
                    continue;
                }
                
                if (c == '_') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q27;
                    continue;
                }
                
                if (c == '/') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q15;
                    continue;
                }
                if (c == '<') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q21;
                    continue;
                }
                if (c == '>') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q22;
                    continue;
                }
                if (c == '=') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q23;
                    continue;
                }
                if (c == '!') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q24;
                    continue;
                }
                
                token_type type = (token_type)-1;
                if (c == '+') type = TOKEN_PLUS;
                else if (c == '-') type = TOKEN_MINUS;
                else if (c == '*') type = TOKEN_MULTIPLY;
                else if (c == '.') type = TOKEN_DOT;
                else if (c == ',') type = TOKEN_COMMA;
                else if (c == '(') type = TOKEN_BRACKET_OPEN;
                else if (c == ')') type = TOKEN_BRACKET_CLOSE;
                else if (c == '{') type = TOKEN_CBRACKET_OPEN;
                else if (c == '}') type = TOKEN_CBRACKET_CLOSE;
                
                if (type != (token_type)-1) {
                    free(str_repr);
                    return createToken(type, (char[]){c, '\0'}, 1);
                }
                
                printError(1, "Lexical error: Invalid character");
                exit(1);
                
            case q0:  // Hex number body
                if (isxdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                } else {
                    if (str_len <= 2) {
                        printError(1, "Hex literal must have digits");
                        exit(1);
                    }
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_LITERAL_HEX, str_repr, str_len);
                    free(str_repr);
                    return token;
                }
                break;
                
            case q1:
                if (isdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                } else if (c == '.') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q2;
                } else if (c == 'e' || c == 'E') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q4;
                } else if (isalpha(c) || c == '_') {
                    printError(1, "Lexical error: Invalid number format (letter after number)");
                    exit(1);
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_LITERAL_INT, str_repr, str_len);
                    free(str_repr);
                    return token;
                }
                break;
                
            case q2:
                if (isdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q3;
                } else {
                    printError(1, "Lexical error: Invalid float format (digit expected after dot)");
                    exit(1);
                }
                break;
                
            case q3:
                if (isdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                } else if (c == 'e' || c == 'E') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q4;
                } else if (c == '.') {
                    printError(1, "Lexical error: Multiple decimal points");
                    exit(1);
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_LITERAL_FLOAT, str_repr, str_len);
                    free(str_repr);
                    return token;
                }
                break;
                
            case q4:
                if (c == '+' || c == '-') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q5;
                } else if (isdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q6;
                } else {
                    printError(1, "Lexical error: Invalid exponent (digit or sign expected)");
                    exit(1);
                }
                break;
                
            case q5:
                if (isdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q6;
                } else {
                    printError(1, "Lexical error: Invalid exponent (digit expected)");
                    exit(1);
                }
                break;
                
            case q6:
                if (isdigit(c)) {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_LITERAL_FLOAT, str_repr, str_len);
                    free(str_repr);
                    return token;
                }
                break;
                
            case q7:
                if (isalnum(c) || c == '_') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                } else {
                    token_type kw = isKeyword(str_repr);
                    if (kw != (token_type)-1) {
                        token = createToken(kw, str_repr, str_len);
                    } else {
                        token = createToken(TOKEN_LOCAL_IDENTIFIER, str_repr, str_len);
                    }
                    free(str_repr);
                    ungetc(c, stdin);
                    return token;
                }
                break;
                
            case q8:
                if (c == '"') {
                    in_string = false;
                    token = createToken(TOKEN_LITERAL_STRING, str_repr, str_len);
                    free(str_repr);
                    return token;
                } else if (c == '\\') {
                    char c1 = getc(stdin);
                    if (c1 == EOF) {
                        printError(1, "Unterminated string");
                        current_state = START;
                        in_string = false;
                        continue;
                    }
                    
                    if (c1 == 'n') {
                        appendChar(&str_repr, &str_len, &str_capacity, '\n');
                    } else if (c1 == 'r') {
                        appendChar(&str_repr, &str_len, &str_capacity, '\r');
                    } else if (c1 == 't') {
                        appendChar(&str_repr, &str_len, &str_capacity, '\t');
                    } else if (c1 == '\\') {
                        appendChar(&str_repr, &str_len, &str_capacity, '\\');
                    } else if (c1 == '"') {
                        appendChar(&str_repr, &str_len, &str_capacity, '"');
                    
                    } else if (c1 == 'x') {
                        char h1 = getc(stdin);
                        char h2 = getc(stdin);

                        if (h1 == EOF || h2 == EOF) {
                            printError(1, "Invalid hex escape: unexpected EOF");
                            current_state = START;
                            in_string = false;
                        } else if (isxdigit(h1) && isxdigit(h2)) {
                            char hex_str[3] = {h1, h2, '\0'};
                            int value = (int)strtol(hex_str, NULL, 16);
                            appendChar(&str_repr, &str_len, &str_capacity, (char)value);
                        } else {
                            printError(1, "Invalid hex escape: \\x must be followed by two hex digits");
                        }
                    } else {
                        printError(1, "Invalid escape sequence");
                    }
                    
                    continue;

                } else if (c == '\n' || c == EOF) {
                    printError(1, "Unterminated string");
                    in_string = false;
                    current_state = START;
                    continue;
                } else if (c >= 0 && c <= 31) {
                    printError(1, "Lexical error: Unescaped control character in string");
                    exit(1);
                } else {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                }
                break;
                
            case q15:
                if (c == '/') {
                    str_len = 0; 
                    current_state = q16;
                } else if (c == '*') {
                    str_len = 0;
                    in_multiline_comment = true;
                    multiline_comment_depth = 1;
                    current_state = q17;
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_DIVIDE, str_repr, str_len);
                    free(str_repr);
                    return token;
                }
                break;
                
            case q16:
                if (c == '\n' || c == EOF) {
                    free(str_repr);
                    if (c == EOF) {
                        return createToken(TOKEN_EOF, "", 0);
                    }
                    return createToken(TOKEN_EOL, "\n", 1);
                }
                break;
                
            case q17:
                if (c == '/') {
                    char c1 = getc(stdin);
                    if (c1 == '*') {
                        multiline_comment_depth++;
                    } else {
                        if (c1 != EOF) {
                            ungetc(c1, stdin);
                        }
                    }
                } else if (c == '*') {
                    current_state = q19;
                }
                break;
                
            case q19:
                if (c == '/') {
                    multiline_comment_depth--;
                    if (multiline_comment_depth == 0) {
                        in_multiline_comment = false;
                        if (str_repr) { 
                            free(str_repr); str_repr = NULL;
                        }
                        str_len = 0;
                        str_capacity = 0;
                        current_state = START;
                        continue;
                    } else {
                        current_state = q17;
                    }
                } else if (c == '*') {
                    // Stay in q19
                } else {
                    current_state = q17;
                }
                break;
                
            case q21:
                if (c == '=') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    token = createToken(TOKEN_LESS_EQUAL, str_repr, str_len);
                    free(str_repr);
                    return token;
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_LESS, str_repr, str_len);
                    free(str_repr);
                    
                    return token;
                }
                break;
                
            case q22:
                if (c == '=') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    token = createToken(TOKEN_GREATER_EQUAL, str_repr, str_len);
                    free(str_repr);
                    return token;
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_GREATER, str_repr, str_len);
                    free(str_repr);
                    
                    return token;
                }
                break;
                
            case q23:
                if (c == '=') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    token = createToken(TOKEN_DOUBLE_EQUAL, str_repr, str_len);
                    free(str_repr);
                    return token;
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_EQUALS, str_repr, str_len);
                    free(str_repr);
                    return token;
                }
                break;
                
            case q24:
                if (c == '=') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    token = createToken(TOKEN_NOT_EQUAL, str_repr, str_len);
                    free(str_repr);
                    return token;
                } else {
                    printError(1, "Lexical error: '!' must be followed by '='");
                    exit(1);
                }
                break;
                
            case q27:
                if (c == '_') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                    current_state = q28;
                } else {
                    printError(1, "Lexical error: Invalid identifier (global must start with __)");
                    exit(1);
                }
                break;
                
            case q28:
                if (isalnum(c) || c == '_') {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                } else {
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    token = createToken(TOKEN_GLOBAL_IDENTIFIER, str_repr, str_len);
                    free(str_repr);
                    return token;
                }
                break;
                
            case q29:
                if (c == '"') {
                    char c1 = getc(stdin);
                    if (c1 == '"') {
                        char c2 = getc(stdin);
                        if (c2 == '"') {
                            in_multiline_string = false;
                            if (str_len > 0) {
                                size_t i = 0;
                                bool empty_first_line = true;
                                while (i < str_len) {
                                    if (str_repr[i] == '\n') {
                                        break;
                                    }
                                    if (!isspace(str_repr[i])) {
                                        empty_first_line = false;
                                        break;
                                    }
                                    i++;
                                }

                                if (empty_first_line && i < str_len && str_repr[i] == '\n') {
                                    size_t shift = i + 1;
                                    size_t new_len = str_len - shift;
                                    memmove(str_repr, str_repr + shift, new_len);
                                    str_len = new_len;
                                    str_repr[str_len] = '\0';
                                }

                                if (str_len > 0) {
                                    size_t j = str_len;
                                    bool empty_last_line = true;
                                    while (j > 0) {
                                        j--;
                                        if (str_repr[j] == '\n') {
                                            break;
                                        }
                                        if (!isspace(str_repr[j])) {
                                            empty_last_line = false;
                                            break;
                                        }
                                    }

                                    if (empty_last_line && str_repr[j] == '\n') {
                                        str_len = j; 
                                        str_repr[str_len] = '\0';
                                    }
                                    else if (empty_last_line && j == 0 && isspace(str_repr[0])) {
                                        str_len = 0;
                                        str_repr[0] = '\0';
                                    }
                                }
                            }

                            if (str_len == 0) token = createToken(TOKEN_LITERAL_STRING, "", 0);
                            else token = createToken(TOKEN_LITERAL_STRING, str_repr, str_len);
                            
                            free(str_repr);
                            return token;
                        } else {
                            if (c2 != EOF) {
                                ungetc(c2, stdin);
                            }
                            if (c1 != EOF) {
                                ungetc(c1, stdin);
                            }
                            appendChar(&str_repr, &str_len, &str_capacity, c);
                        }
                    } else {
                        if (c1 != EOF) {
                            ungetc(c1, stdin);
                        }
                        appendChar(&str_repr, &str_len, &str_capacity, c);
                    }
                } else {
                    appendChar(&str_repr, &str_len, &str_capacity, c);
                }
                break;
                
            default:
                printError(99, "Internal error: Unknown state in lexer");
                exit(99);
        }
    }
    
    if (in_string || in_multiline_string) {
        printError(1, "Unterminated string at EOF");
    }
    if (in_multiline_comment) {
        printError(1, "Unterminated multiline comment at EOF");
    }

    free(str_repr);
    return createToken(TOKEN_EOF, "", 0);
}