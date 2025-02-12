/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  if (*tree != NULL) {
    *tree = NULL;
  }

}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  bst_node_t *tmp = tree;

  while (tmp != NULL) {

    if (tmp->key == key) {            /* ak najdem zhodu hladaneho kluca nastavim 'value' a fcia skonci*/
      *value = &tmp->content;
      return true;
    }

    if (tmp->key < key) {
      tmp = tmp->right;
      continue;
    }

    if (tmp->key > key) {
      tmp = tmp->left;
      continue;
    }

    return false;

  } 
  return false;

}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{

  bst_node_t *tmp = *tree;
  bst_node_t *new = (bst_node_t*)malloc(sizeof(bst_node_t));
  if (new == NULL) {
    return;
  } 

  new->left = NULL;                     /* vytvorim novy prvok 'new' a priradim spravne hodnoty */
  new->right = NULL;
  new->content = value;
  new->key = key;

  if (tmp == NULL) {                   /* ak je 'tree' z parametru prazdny, novy prvok sa stane korenovym uzlom */

    (*tree) = new;
    return;

  }

  while (tmp != NULL) {             
  
    if (tmp->key == key) {      /* ak najdem uzol, ktoreho kluc je rovny klucu noveho uzla, nahradim ho hodnotou 'value' a fcia skonci */
                                    
      tmp->content = value;
      free(new);
      return;

    }

    if (tmp->key < key) {       /* ak je novy kluc vacsi ako kluc aktualneho uzla, na dalsiu iteraciu sa presuniem sa do PRAVEHO podstromu */
      if (tmp->right != NULL) {
        tmp = tmp->right;
      } else {
        tmp->right = new;
        return;
      }
      continue;
    }

    if (tmp->key > key) {       /* ak je novy kluc mensi ako kluc aktualneho uzla, na dalsiu iteraciu sa presuniem do LAVEHO podstromu */

      if (tmp->left != NULL){
        tmp = tmp->left;
      } else {
        tmp->left = new;
        return;
      }
      continue;
    }
  }


}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{

  bst_node_t *tmp = *tree;
  bst_node_t *tmp2 = target;

  if (tmp == NULL || tmp2 == NULL) {
    return;
  }

  while (tmp->right != NULL) {
    tmp2 = tmp;
    tmp = tmp->right;                 /* do 'tmp' ulozim najpravejsi uzol */
  }

  if (tmp->left == NULL) {           /* ak najpravejsi nema lavy podstrom...*/

    target->content = tmp->content;
    target->key = tmp->key;

    if (tmp2 != target) {
     
      tmp2->right = NULL;           /* nastavim hodnoty 'target' a podla toho, ci predosly uzol (tmp2) je aj targetom, nastavim kam bude predosly ukazovat*/
    } else {

      tmp2->left = NULL;
    }  
    
    free(tmp);
    tmp = NULL;
    return;

  } else {

    target->content = tmp->content;
    target->key = tmp->key;


    tmp2->right = tmp->left;        /* ak najpravejsi ma lavy podstrom nastavim, aby bol pravym podstromom predosleho uzla */

    free(tmp->content.value);
    free(tmp);
    tmp = NULL;
    return;

  }

}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{

  bst_node_t *tmp = *tree;
  bst_node_t *tmp2;

  while (tmp != NULL) {

    if (tmp->key < key) {
      
      tmp2 = tmp;                     
      tmp = tmp->right;
      continue;
    }                            /* vzdy ked sa presuvam do jedneho z podstromov, ulozim si predosly uzol do 'tmp2' */             

    if (tmp->key > key) {

      tmp2 = tmp;
      tmp = tmp->left;
      continue;
    }

    if (tmp->key == key) {      /* ked najdem zhodu klucov, musim osetrit podstromy... */

      
      if (tmp->left != NULL && tmp->right != NULL) {    /* ak hladany uzol ma obidva podstromy, zavolam 'bst_replace_by_rightmost' */

        bst_replace_by_rightmost(tmp, &tmp->left);
        continue;

      }

      if (tmp->left == NULL && tmp->right == NULL) {

        if (tmp2->key > key) {
          tmp2->left = NULL;                              
        }                                               /* ak hladany uzol nema ani jeden podstrom, nastavim danu vetvu predosleho uzla*/
        if (tmp2->key < key) {
          tmp2->right = NULL;
        }

        free(tmp->content.value);
        free(tmp);                                     /* nasledne uvolnim najpravejsi prvok */
        tmp = NULL;
        return;

      }

      if (tmp->left != NULL) {
      
        tmp2->left = tmp->left;                       /* ak ma najpravejsi uzol pravy podstrom, zdedi ho predosli uzol */

      }
      if (tmp->right != NULL) {
      
        tmp2->right = tmp->right;                     /* ak ma najpravejsi uzol lavy podstrom, zdedi ho predosli uzol */

      }

      free(tmp->content.value);
      free(tmp);
      tmp = NULL;
      return; 
    
    }
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  stack_bst_t stack;
  
  stack_bst_init(&stack);
  stack_bst_push(&stack, (*tree));

  while (stack_bst_empty(&stack) != false) {

    bst_node_t *to_delete = stack_bst_pop(&stack);

    if (to_delete != NULL) {

      stack_bst_push(&stack, to_delete->left);
      stack_bst_push(&stack, to_delete->right);

      free(to_delete->content.value);
      free(to_delete);
      to_delete = NULL;
    }

  }
  (*tree) = NULL;

}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{

  bst_node_t *tmp = tree;

  while (tmp != NULL) {

    bst_add_node_to_items(tmp, items);
    stack_bst_push(to_visit, tmp);
    tmp = tmp->left;

  }


}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{

  stack_bst_t stack;

  stack_bst_init(&stack);
  bst_node_t *tmp = tree;

  bst_leftmost_preorder(tmp, &stack, items);

  while (stack_bst_empty(&stack) == false) {

    tmp = stack_bst_pop(&stack);
    bst_leftmost_preorder(tmp->right, &stack, items);

  }

}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{

  bst_node_t *tmp = tree;

  while (tmp != NULL) {

    stack_bst_push(to_visit, tmp);
    tmp = tmp->left;

  }

}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{

  stack_bst_t stack;


  stack_bst_init(&stack);
  bst_node_t *tmp = tree;

  bst_leftmost_inorder(tmp, &stack);

  while (stack_bst_empty(&stack) == false) {

    tmp = stack_bst_pop(&stack);
    bst_add_node_to_items(tmp, items);    
    bst_leftmost_inorder(tmp->right, &stack);

  }

}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit)
{

  bst_node_t *tmp = tree;

  while (tmp != NULL) {

    stack_bst_push(to_visit, tmp);
    stack_bool_push(first_visit, true);
    tmp = tmp->left;

  }

}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{

  stack_bst_t stack;
  stack_bool_t stack_b;

  stack_bst_init(&stack);
  stack_bool_init(&stack_b);

  bst_node_t *tmp = tree;
  bool fromleft;

  bst_leftmost_postorder(tmp, &stack, &stack_b);

  while (stack_bst_empty(&stack) == false) {

    tmp = stack_bst_top(&stack);
    fromleft = stack_bool_top(&stack_b);
    stack_bool_pop(&stack_b);

    if (fromleft == true) {

      stack_bool_push(&stack_b, false);
      bst_leftmost_postorder(tmp->right, &stack, &stack_b);

    } else {
      stack_bst_pop(&stack);  
      bst_add_node_to_items(tmp, items);

    }

  }
}