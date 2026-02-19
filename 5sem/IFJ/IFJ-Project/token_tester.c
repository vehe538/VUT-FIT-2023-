#include "scanner.h"
#include "error.h"

int main() {

    // ZMENA: Upravená výzva, aby bolo jasné, ako ukončiť vstup (Ctrl+D alebo Ctrl+Z).
    printf("Zadajte kód na otestovanie (ukončite pomocou Ctrl+D/Ctrl+Z):\n");
    char *buffer = malloc(sizeof(char)*9999);
    if (!buffer) {
        printError(99, "malloc fail");
    }

    char c;
    int i = 0;
    // ZMENA: Cyklus teraz číta vstup až po EOF (koniec súboru), 
    // nie len po koniec riadku ('\n'). Umožňuje to testovať viacriadkové vstupy.
    while ((c = getchar()) != EOF && i < 9998) {
        buffer[i] = c;
        i++;
    }
    // ZMENA: Na koniec buffera pridáme EOF, aby funkcia getToken() vedela, kedy má skončiť.
    buffer[i] = EOF; 

    bool done = false;
    while(!done) {

        // ZMENA: Odstránená zbytočná alokácia a následný memory leak.
        // Pôvodne bolo: Token *first_token = malloc(sizeof(*first_token));
        // Funkcia getToken() už sama alokuje pamäť pre token.
        Token *first_token = getToken(buffer);
        
        if (first_token == NULL) {
            fprintf(stderr, "Chyba: getToken vrátil NULL, ukončujem.\n");
            break;
        }

        printf("Acquired token:\n{\n    type: ");    
        switch (first_token->type) {
            case TOKEN_KEYWORD_CLASS:      printf("TOKEN_KEYWORD_CLASS\n"); break;
            case TOKEN_KEYWORD_IF:         printf("TOKEN_KEYWORD_IF\n"); break;
            case TOKEN_KEYWORD_ELSE:       printf("TOKEN_KEYWORD_ELSE\n"); break;
            case TOKEN_KEYWORD_IS:         printf("TOKEN_KEYWORD_IS\n"); break;
            case TOKEN_KEYWORD_NULL_SMALL: printf("TOKEN_KEYWORD_NULL_SMALL\n"); break;
            case TOKEN_KEYWORD_RETURN:     printf("TOKEN_KEYWORD_RETURN\n"); break;
            case TOKEN_KEYWORD_VAR:        printf("TOKEN_KEYWORD_VAR\n"); break;
            case TOKEN_KEYWORD_WHILE:      printf("TOKEN_KEYWORD_WHILE\n"); break;
            case TOKEN_KEYWORD_IFJ:        printf("TOKEN_KEYWORD_IFJ\n"); break;
            case TOKEN_KEYWORD_STATIC:     printf("TOKEN_KEYWORD_STATIC\n"); break;
            case TOKEN_KEYWORD_TRUE:       printf("TOKEN_KEYWORD_TRUE\n"); break;
            case TOKEN_KEYWORD_FALSE:      printf("TOKEN_KEYWORD_FALSE\n"); break;
            case TOKEN_KEYWORD_NUM:        printf("TOKEN_KEYWORD_NUM\n"); break;
            case TOKEN_KEYWORD_STRING:     printf("TOKEN_KEYWORD_STRING\n"); break;
            case TOKEN_KEYWORD_NULL_BIG:   printf("TOKEN_KEYWORD_NULL_BIG\n"); break;
    
            case TOKEN_LOCAL_IDENTIFIER:   printf("TOKEN_LOCAL_IDENTIFIER\n"); break;
            case TOKEN_GLOBAL_IDENTIFIER:  printf("TOKEN_GLOBAL_IDENTIFIER\n"); break;
            case TOKEN_DOT:                printf("TOKEN_DOT\n"); break;
            case TOKEN_LITERAL_INT:        printf("TOKEN_LITERAL_INT\n"); break;
            case TOKEN_LITERAL_HEX:        printf("TOKEN_LITERAL_HEX\n"); break;
            case TOKEN_LITERAL_FLOAT:      printf("TOKEN_LITERAL_FLOAT\n"); break;
            case TOKEN_PLUS:               printf("TOKEN_PLUS\n"); break;
            case TOKEN_MINUS:              printf("TOKEN_MINUS\n"); break;
            case TOKEN_MULTIPLY:           printf("TOKEN_MULTIPLY\n"); break;
            case TOKEN_DIVIDE:             printf("TOKEN_DIVIDE\n"); break;
            case TOKEN_EQUALS:             printf("TOKEN_EQUALS\n"); break;
    
            case TOKEN_GREATER:            printf("TOKEN_GREATER\n"); break;
            case TOKEN_LESS:               printf("TOKEN_LESS\n"); break;
            case TOKEN_DOUBLE_EQUAL:       printf("TOKEN_DOUBLE_EQUAL\n"); break;
            case TOKEN_GREATER_EQUAL:      printf("TOKEN_GREATER_EQUAL\n"); break;
            case TOKEN_LESS_EQUAL:         printf("TOKEN_LESS_EQUAL\n"); break;
            case TOKEN_NOT_EQUAL:          printf("TOKEN_NOT_EQUAL\n"); break;
    
            case TOKEN_ONELINE_COMMENT:    printf("TOKEN_ONELINE_COMMENT\n"); break;
            case TOKEN_MULTILINE_COMMENT:  printf("TOKEN_MULTILINE_COMMENT\n"); break;
    
            case TOKEN_BRACKET_OPEN:       printf("TOKEN_BRACKET_OPEN\n"); break;
            case TOKEN_BRACKET_CLOSE:      printf("TOKEN_BRACKET_CLOSE\n"); break;
            case TOKEN_CBRACKET_OPEN:      printf("TOKEN_CBRACKET_OPEN\n"); break;
            case TOKEN_CBRACKET_CLOSE:     printf("TOKEN_CBRACKET_CLOSE\n"); break;
            case TOKEN_LITERAL_STRING: printf("TOKEN_LITERAL_STRING\n"); break;

            case TOKEN_EOF:                printf("TOKEN_EOF\n"); done = true; break;
            case TOKEN_EOL:                printf("TOKEN_EOL\n"); break;
            default:                       printf("UNKNOWN TOKEN (%d)\n", first_token->type); break;
        }

        printf("    str_representation: %s\n    token_size: %d\n}\n", first_token->token_str, first_token->size);    
        
        // ZMENA: Oprava memory leaku. Je nutné uvoľniť pamäť pre reťazec vnútri tokenu
        // predtým, ako uvoľníme samotnú štruktúru tokenu.
        free(first_token->token_str);
        free(first_token);
    }


    free(buffer);
    return 0;
}