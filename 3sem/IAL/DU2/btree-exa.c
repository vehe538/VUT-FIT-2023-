/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/

bool is_letter(char c) {

    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == ' ') {
        return true;
    }
    return false;

}

void letter_count(bst_node_t **tree, char *input) {

    bst_init(tree);
    bst_node_content_t tmp;
    bst_node_content_t **tmp2 = (bst_node_content_t**)malloc(sizeof(bst_node_content_t));
    int *ptr = malloc(sizeof(int));

    if (ptr == NULL || tmp2 == NULL) {
        return;
    }    

    *ptr = 1;
    tmp.type = INTEGER;                                     /* na zaciatku vlozim ako prvy uzol hned prvy znak, aby bol v strome nejaky koren */
    tmp.value = ptr;

    bst_insert(tree, input[0], tmp);

    for (size_t i = 1; i < strlen(input); i++) {

        
        if (bst_search(*tree, input[i], tmp2)) {            /* ak uzol v strome existuje ulozim do 'tmp2' jeho hodnotu*/

            //ptr = &(*tmp2)->value + 1;
            //tmp.value = ptr;

            bst_insert(tree, input[i], tmp);                /* aktualizuje sa hodnotota daneho uzla +1 */

        } else {
            
            if (!is_letter(input[i])) {                     /* ak znak zo vstupu nie je pismeno alebo medzera... */

                //ptr = &(*tmp2)-> + 1;
                //tmp.value = ptr
                bst_insert(tree, input[i], tmp);
 
            } else {

                *ptr = 1;                                   /* ak v uzla este dany znak nie je, vytvorim novy uzol s hodnotou 1 */
                tmp.value = ptr;
                tmp.type = INTEGER;

                bst_insert(tree, input[i], tmp);

            }
        }
        

    }    
    free(tmp2);
}