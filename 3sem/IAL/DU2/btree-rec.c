/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
  if ((*tree) != NULL) {
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{

  if (tree == NULL) {               /* ak zadany strom neexistuje, vrati false */

    return false;

  } else if (tree->key == key) {    /* ak zadany strom existuje a jeho kluc sa rovna klucu, ktory hladam... */ 
    
    *value = &tree->content;        /* do 'value' zapisem hodnotu daneho stromu (uzla) a vratim true */
    return true;
  
  }

  if (tree->key < key) {           /* ak je kluc zadaneho stromu mensi, ako ten, ktory hladam, presuniem sa do praveho podstromu */
    bst_search(tree->right, key, value);

  } else {                        /* ak je kluc zadaneho stromu naopak vacsi, ako ten, ktory hladam, presuniem sa do laveho podstromu */
    bst_search(tree->left, key, value);
  
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{

  bst_node_t *tmp = *tree;

  if (tmp == NULL) {                                /* ak dany uzol este v strome neexistuje... */
  
    bst_node_t *new = malloc(sizeof(bst_node_t));   /* naalokujem pamet pre novy uzol*/
    if (new == NULL) {
      return;
    }

    new->content = value;                           /* priradim novemu uzlu zadane hodnoty */
    new->key = key;
    new->right = NULL;
    new->left = NULL;

    (*tree) = new;                                  /* nahradim povodny uzol novym */

  } else {                                          /* ak uzol z parametru uz existuje v strome... */
    
    if (tmp->key > key) {                           /*  ak ma kluc mensiu hodnotu, presuniem sa do laveho podstromu  */
      bst_insert(&(*tree)->left, key, value);
      return;
    }

    if (tmp->key < key) {                           /* ak ma kluc vacsiu hodnotu, presuniem sa do praveho podstromu */
      bst_insert(&(*tree)->right, key, value);
      return;
    }
    
    if (tmp->key == key) {                          /* kluce sa rovnaju => prepisem hodnotu existujuceho uzla na 'value' */
      (*tree)->content = value;
      return;
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{

  bst_node_t *tmp = *tree;
  bst_node_t *tmp2;

  if (tmp == NULL) {
    return;
  }

  if (tmp->right->right != NULL) {  /* ak je aspon jeden uzol smerom doprava... */
    
    bst_replace_by_rightmost(target, &tmp->right);
    return;

  }


  if (tmp->right != NULL) {             /* ak nastane pripad kedy ma 'target' IBA pravy podstrom, ktory uz doprava nepokracuje... */

    tmp2 = target;                      /* 'tmp2' nastavim na 'target' pre lepsi pristup*/

    if (tmp->right->left == NULL) {     /* ak neexistuje lavy podstrom... */  

      tmp2 = tmp;                       /* do 'tmp2' si ulozim 'tmp' */
      tmp = tmp->right;                 /* tmp zmenim na najpravejsi */

      target->content = tmp->content;   /* zmenim hodotu a kluc 'targetu' na hodnotu najpravejsieho prvku */
      target->key = tmp->key;

      
      free(tmp);                        /* uvolnim pamet povodneho stromu */
      tmp2->right = NULL;               /* zariadim aby novy koren ('target') nemal pravy podstrom */
      return;

    } else {                            /* ak existuje lavy podstrom najpravejsieho uzla... */

      tmp2 = tmp;
      tmp = tmp->right;                 /* nastavim hodnoty ako v predoslej podmienke */

      target->content = tmp->content;
      target->key = tmp->key;
    
      tmp2->right = tmp->left;          /* lavy podstrom povodneho stromu sa stane pravym podstromov pre 'targer'  */
      free(tmp->content.value);
      free(tmp);
      return;
      
    }

  } else {      /* ak target nema pravy podstrom - najpravejsi je koren */

    target->content = tmp->content;
    target->key = tmp->key;

    if (tmp->left != NULL) {              /* ak ma target lavy koren, presuniem ho napravo */
      target->right = tmp->left;
    }

    free(tmp->content.value);
    free(tmp);
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *tmp = *tree;

  if (tmp == NULL) {
    return;
  }

  if (tmp->key == key) {                          /* ak som v spravnom podstrome... */

    if (tmp->left == NULL && tmp->right == NULL) { /* nema ani jedneho syna */

      free((*tree)->content.value);
      free(*tree);
      *tree = NULL;
      return;

    }

    if (tmp->left == NULL && tmp->right != NULL) { /* ak ma iba praveho syna... */

      bst_node_t *parent = tmp;                     /* do 'parent' si ulozim strom, ktory idem mazat*/ 
      (*tree) = tmp->right;                         /* aktualizujem hodnotu 'tmp', aby som mohol vymazat 'parent' */
      free(parent->content.value);
      free(parent);                                 /* uvolnim uzol nad pravym podstrom (parent) */



    }

    if (tmp->left != NULL && tmp->right == NULL) { /* ak ma iba laveho syna... */
      
      bst_node_t *parent = tmp;
      (*tree) = tmp->left;
      free(parent->content.value);
      free(parent);

    }

    if (tmp->left != NULL && tmp->right != NULL) { /*ak ma obidvoch synov... */

      bst_replace_by_rightmost(tmp, &tmp->left);   /* do 'tmp' ulozim najpravejsieho syna laveho podstromu */
      return;
    }

  } else if (tmp->key < key) { /* podstrom, ktory chcem vymazat je v pravom podstrome */

    if (tmp->right != NULL) {
      bst_delete(&tmp->right, key);
      return;
    }

  } else if (tmp->key > key) {  /* podstrom, ktory chcem vymazat je v lavom podstrome */
    
    if (tmp->left != NULL) {
      bst_delete(&tmp->left, key);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{

  if (*tree != NULL) {

    bst_dispose(&(*tree)->left);        /* kazdy strom, ktory chcem uvolnit, musi mat uvolnene obidva podstromy */
    bst_dispose(&(*tree)->right);
    free((*tree)->content.value);
    free(*tree);
    (*tree) = NULL;                     /* hodnotu stromu nastavim na taku aka bola po inicializacii */
  }

}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  
  if (tree != NULL) {
    bst_add_node_to_items(tree, items);     /* najprv nacitam rodicovsky uzol, potom nasleduje lavy syn a nakoniec pravy syn */
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }

}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{

  if (tree != NULL) {
    bst_inorder(tree->left, items);       /* najprv nacitam laveho syna, potom nasleduje rodicovsky uzol a nakoniec pravy syn */       
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
  } 

}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree != NULL) {
    bst_postorder(tree->left, items);     /* najprv nacitam laveho syna, potom nasleduje pravy syn a nakoniec rodicovsky uzol */
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
}
