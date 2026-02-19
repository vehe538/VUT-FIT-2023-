#include "scanner.h"

// helper function for creating tokens 
Token  *createToken(token_type type, char *token_str, int size) {

    Token *new_token = malloc(sizeof(*new_token));
    new_token->type = type;
    new_token->token_str = token_str;
    new_token->size = size;

    return new_token;

}

// helper function to check if identifier is one of keywords
token_type isKeyword(char *word) {


    if (strcmp(word, "class") == 0) {
        return TOKEN_KEYWORD_CLASS;
    }
    if (strcmp(word, "if") == 0) {
        return TOKEN_KEYWORD_IF;
    }
    if (strcmp(word, "else") == 0) {
        return TOKEN_KEYWORD_ELSE;
    }
    if (strcmp(word, "is") == 0) {
        return TOKEN_KEYWORD_IS;
    }
    if (strcmp(word, "null") == 0) {
        return TOKEN_KEYWORD_NULL_SMALL;
    }
    if (strcmp(word, "return") == 0) {
        return TOKEN_KEYWORD_RETURN;
    }
    if (strcmp(word, "var") == 0) {
        return TOKEN_KEYWORD_VAR;
    }
    if (strcmp(word, "while") == 0) {
        return TOKEN_KEYWORD_WHILE;
    }
    if (strcmp(word, "Ifj") == 0) {
        return TOKEN_KEYWORD_IFJ;
    }
    if (strcmp(word, "static") == 0) {
        return TOKEN_KEYWORD_STATIC;
    }
    if (strcmp(word, "true") == 0) {
        return TOKEN_KEYWORD_TRUE;
    }
    if (strcmp(word, "false") == 0) {
        return TOKEN_KEYWORD_FALSE;
    }
    if (strcmp(word, "Num") == 0) {
        return TOKEN_KEYWORD_NUM;
    }
    if (strcmp(word, "String") == 0) {
        return TOKEN_KEYWORD_STRING;
    }
    if (strcmp(word, "Null") == 0) {
        return TOKEN_KEYWORD_NULL_BIG;
    }    
    return -1;
}

// helper function for checking previous character from a text buffer
char checkPrev(char *input_buffer, int index) {
    return input_buffer[index-1];
}

// helper function to concatenate string with a character
void strcatChar(char *destination, char c) {

    size_t len = strlen(destination);

    destination[len] = c;
    destination[len+1] = '\0';
    return;
}

// global vars definitions
int current_index = 0;
bool in_string = false;
bool in_hex = false;
bool in_hex_str = false;
bool in_oneline_comment = false;
bool in_multiline_comment = false;

// main function of scanner for obtaining tokens from input
Token *getToken(char *input_buffer) {


    Token *token = malloc(sizeof(*token));
    if (!token) {
        printError(99, "malloc fail");
    }


    char *str_repr = malloc(sizeof(char)*1000);
    str_repr[0] = '\0';
    char *c = malloc(sizeof(char));
    
    if (!c || !str_repr) {
        printError(99, "malloc fail");
    }

    fsm_state current_state = START;
    while((*c = input_buffer[current_index]) != EOF) {

        // FSMs implementation
        switch (current_state) {

            case START:


                if (isspace(*c)) {
                    current_index++;
                    continue;
                
                } else if (in_oneline_comment && *c != '\n') {

                    current_state = q16;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (in_oneline_comment && *c == '\n') {

                    current_state = q18;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (in_multiline_comment) {

                    current_state = q17;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                }
                else if (*c >= '0' && *c <= '9') {
                    
                    current_state = q1;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;
                } 
                else if (((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z')) && !in_string) {

                    current_state = q7;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '"') {

                    current_state = q8;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '\\' && in_string) {

                    current_state = q9;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c > 31 && in_string) {

                    current_state = q10;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '/' && !in_string && !in_hex_str) {

                    current_state = q15;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '+' || *c == '-' || *c == '*' || *c == '(' || *c == ')' || *c == '{' || *c == '}' || *c == '.') {

                    current_state = q25;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '<') {

                    current_state = q21;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '>') {

                    current_state = q22;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '=') {

                    current_state = q23;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '!') {

                    current_state = q24;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (((*c >= 'a' && *c <= 'f') || (*c >= 'A' && *c <= 'F') || (*c >= '0' && *c <= '9')) && in_hex) {

                    current_state = q0;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '_') {

                    current_state = q27;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;
                } else {

                    token = createToken(TOKEN_EOF, "EOF", 0);
                    return token;
                }
                
                break;

            case q0:

                if (checkPrev(input_buffer, current_index) == 'x') {
                    in_hex = true;
                }
                if ((*c >= 'a' && *c <= 'f') || (*c >= 'A' && *c <= 'F') || (*c >= '0' && *c <= '9')) {

                    current_state = q0;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    in_hex = false;
                    token = createToken(TOKEN_LITERAL_HEX, str_repr, strlen(str_repr));
                    return token;

                }
                break;

            case q1:
                if (*c >= '0' && *c <= '9') {
                    
                    current_state = q1;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == '.') {

                    current_state = q2;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c == 'e' || *c == 'E') {

                    current_state = q4;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if(*c == 'x' && checkPrev(input_buffer, current_index) == '0') {

                    current_state = q0;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;


                } else {

                    token = createToken(TOKEN_LITERAL_INT, str_repr, strlen(str_repr));
                    return token;                    
                
                }
                break;
            case q2: 
                if (*c >= '0' && *c <= '9') {

                    current_state = q3;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    printError(2, "syntax error: Invalid token");
                    free(token);

                }
                break;

            case q3:

                if ( *c >= '0' && *c <= '9') {
                
                    current_state = q3;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;
                
                } else if (*c == 'e' || *c == 'E') {

                    current_state = q4;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    current_index++;
                    token = createToken(TOKEN_LITERAL_FLOAT, str_repr, strlen(str_repr));
                    return token;

                }
                break;
            case q4:

                if (*c == '+' || *c == '-') {

                    current_state = q5;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else if (*c >= '0' && *c <= '9') {

                    current_state = q6;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    free(token);
                    printError(2, "syntax error: Invalid token");
                    
                }
                break;
            case q5:
                if ( *c >= '0' && *c <= '9') {

                    current_state = q6;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    free(token);
                    printError(2, "syntax error: Invalid token");

                }
                break;
            case q6:
                if ( *c >= '0' && *c <= '9') {

                    current_state = q6;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else  {

                    token = createToken(TOKEN_LITERAL_FLOAT, str_repr, strlen(str_repr));
                    return token;

                }
                break;

            case q7:

                if ( *c == '_' || ( *c >= '0' && *c <= '9') || (*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z')) {

                    current_state = q7;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {
                    
                    // check for keywords
                    token_type tmp = isKeyword(str_repr);
                    if(tmp <= TOKEN_KEYWORD_NULL_BIG && tmp >= TOKEN_KEYWORD_CLASS) {
                        
                        token = createToken(tmp, str_repr, strlen(str_repr));
                    } else {

                        token = createToken(TOKEN_LOCAL_IDENTIFIER, str_repr, strlen(str_repr));
                    }
                    return token;

                }
                break;
            case q8:
                
                in_string = !in_string;
                token = createToken(TOKEN_QUOTES, str_repr, strlen(str_repr));
                return token;
                
                
            case q9:
                
                if (in_string) {
                    if (*c == 'n' || *c == 'r' || *c == 't' || *c == '\\' || *c == '*' || *c == '"') {

                        current_state = q14;
                        strcatChar(str_repr, *c);
                        continue;
                    
                    } else if (*c == 'x') {

                        current_state = q11;
                        strcatChar(str_repr, *c);
                        current_index++;
                        continue;

                    } else {
                        free(token);
                        printError(1, "lexical error: Invalid escape sequence");
                    }
                }

                break;

            case q10:

                token = createToken(TOKEN_STRING_SYMBOL, str_repr, strlen(str_repr));
                return token;    
                break;
            
            case q11:

                if (*c >= 'a' || *c <= 'f' || *c >= 'A' || *c <= 'F' || *c <= '9' || *c >= '0') {
                    
                    current_state = q12;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    free(token);
                    printError(1, "lexical error: Invalid hexadecimal value");

                }
                break;

            case q12:

                if (*c >= 'a' || *c <= 'f' || *c >= 'A' || *c <= 'F' || *c >= '0' || *c <= '9') {
                        
                    current_state = q13;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    free(token);
                    printError(1, "lexical error: Invalid hexadecimal value");

                }
                break;

            case q13:
                
                in_hex_str = false;
                token = createToken(TOKEN_ESCAPED_HEX, str_repr, strlen(str_repr));
                return token;

                break;
            
            case q14:
                
                switch(*c) {
                    case 'n':

                        current_index++;

                        token = createToken(TOKEN_ESCAPED_N, str_repr, strlen(str_repr));
                        return token;    
                        break;
                    
                    case 'r':

                        current_index++;

                        token = createToken(TOKEN_ESCAPED_R, str_repr, strlen(str_repr));
                        return token;    
                        break; 
                    
                    case 't':

                        current_index++;

                        token = createToken(TOKEN_ESCAPED_T, str_repr, strlen(str_repr));
                        return token;    
                        break;
                    
                    case '\\':
                        
                        current_index++;
                    
                        token = createToken(TOKEN_ESCAPED_BACKSLASH, str_repr, strlen(str_repr));
                        return token;    
                        break;   

                    case '*':
                    
                        current_index++;
                    
                        token = createToken(TOKEN_ESCAPED_N, str_repr, strlen(str_repr));
                        return token;    
                        break;

                    case '"':
                     
                        current_index++;
                    
                        token = createToken(TOKEN_ESCAPED_QUOTES, str_repr, strlen(str_repr));
                        return token;    
                        break;

                    default:
                        break;
                }
                break;

            case q15:

                if (*c == '/') {

                    in_oneline_comment = true;

                    current_state = q16;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                }  else if (*c == '*') {
                    
                    in_multiline_comment = true;

                    current_state = q17;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    token = createToken(TOKEN_DIVIDE, str_repr, strlen(str_repr));
                    return token;

                }
                break;
            
            case q16:
                
                if (*c == '\n') {

                    current_state = q18;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    current_state = q16;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;
                }
                break;
            
            case q17:

                if (*c == '*') {

                    current_state = q19;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    current_state = q17;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                }
                break;
            
            case q18:
                
                in_oneline_comment = !in_oneline_comment;

                token = createToken(TOKEN_ONELINE_COMMENT, str_repr, strlen(str_repr));
                return token;
                break;
            
            case q19:

                if (*c == '/') {

                    
                    current_state = q20;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    current_state = q17;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                }
                break;

            case q20:
                
                in_multiline_comment = !in_multiline_comment;

                token = createToken(TOKEN_MULTILINE_COMMENT, str_repr, strlen(str_repr));
                return token;
                break;

            case q21:

                if (*c == '=') {

                    current_state = q26;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    token = createToken(TOKEN_LESS, str_repr, strlen(str_repr));
                    return token;

                }
                break;
            
            case q22:

                if (*c == '=') {

                    current_state = q26;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    token = createToken(TOKEN_GREATER, str_repr, strlen(str_repr));
                    return token;

                }
                break;
            
            case q23:

                if (*c == '=') {

                    current_state = q26;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    token = createToken(TOKEN_EQUALS, str_repr, strlen(str_repr));
                    return token;

                }
                break;

            case q24:

                if (*c == '=') {

                    current_state = q26;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    free(token);
                    printError(2, "syntax error: Invalid token");

                }
                break;
            
            case q25:

                switch(checkPrev(input_buffer, current_index)) {
                    case '+':
                        token = createToken(TOKEN_PLUS, str_repr, strlen(str_repr));
                        break;
                    case '-':
                        token = createToken(TOKEN_MINUS, str_repr, strlen(str_repr));
                        break;
                    case '*':
                        token = createToken(TOKEN_MULTIPLY, str_repr, strlen(str_repr));
                        break;
                    case '.':
                        token = createToken(TOKEN_DOT, str_repr, strlen(str_repr));
                        break;
                    case '(':
                        token = createToken(TOKEN_BRACKET_OPEN, str_repr, strlen(str_repr));
                        break;
                    case ')':
                        token = createToken(TOKEN_BRACKET_CLOSE, str_repr, strlen(str_repr));
                        break;  
                    case '{':
                        token = createToken(TOKEN_CBRACKET_OPEN, str_repr, strlen(str_repr));
                        break;
                    case '}':
                        token = createToken(TOKEN_CBRACKET_CLOSE, str_repr, strlen(str_repr));
                        break;                     
                    default:
                        break;
                }
                return token;
                break;

            case q26:

                switch(checkPrev(input_buffer, (current_index)-1)) {

                    case '<':
                        token = createToken(TOKEN_LESS_EQUAL, str_repr, strlen(str_repr));
                        break;
                    case '>':
                        token = createToken(TOKEN_GREATER_EQUAL, str_repr, strlen(str_repr));
                        break;
                    case '=':
                        token = createToken(TOKEN_DOUBLE_EQUAL, str_repr, strlen(str_repr));
                        break;
                    case '!':
                        token = createToken(TOKEN_NOT_EQUAL, str_repr, strlen(str_repr));
                        break;
                    default:
                        break;
                }
                return token;
            
            case q27:

                if (*c == '_') {

                    current_state = q28;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    free(token);
                    printError(2, "syntax error: invalid token");

                }
                break;
            
            case q28:

                if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9') || *c == '_') {

                    current_state = q28;
                    strcatChar(str_repr, *c);
                    current_index++;
                    continue;

                } else {

                    token = createToken(TOKEN_GLOBAL_IDENTIFIER, str_repr, *c);
                    return token;
                }
                break;
            
            default:
                printf("??\n");
                break;

        }


    } 

    return createToken(-1, "", 0);
}