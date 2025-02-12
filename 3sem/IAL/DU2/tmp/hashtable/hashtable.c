/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {

  
  for (int i = 0; i < MAX_HT_SIZE; i++) {

    (*table)[i] = NULL;                           /* kazdy prvok v tabulke nastavim na NULL */

  }

}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */

ht_item_t *ht_search(ht_table_t *table, char *key) {

  ht_item_t *found = NULL;                        /* premenna 'found' mi urci ci sa nasiel prvok so zadanym klucom */

  for (int i = 0; i < HT_SIZE; i++) {             /* prechadzam prvky tabulky... */

    ht_item_t *item = (*table)[i];                /* do premennej 'item' si ulozim aktualny prvok (hlavne kvôli prehladnosti kodu) */
    if (item != NULL && item->key == key) {       /* ak najdem prvok, ktory ukazuje na realnu hodnotu a jeho kluc sa zhoduje s klucom z argumentu... */

      item = found;                               /* do premennej 'found' ulozim prvok, ktory vyhovuje podmienkam */
      break;
    
    }

  }

  return found;                                   /* funkcia vracia hodnotu 'found' - ukazatel na prvok alebo NULL */
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */

void ht_insert(ht_table_t *table, char *key, float value) {

  if (ht_search(table, key) == NULL) {                          /* ak prvok v tabulke este neexistuje... */

    ht_item_t *new = (ht_item_t *)malloc(sizeof(ht_item_t));    /* alokujem miesto v pameti pre novy prvok */

    if (new == NULL) {
      return;
    }

    int index = get_hash(key);                                  /* do premennej 'index' si ulozim idealnu poziciu pre dany kluc */
    new->next = NULL;                                           /* nasledovny prvok nastavim zatial na NULL (je mozne, za nim v tabulke nebude ziadny prvok) */
    new->value = value;                                         
    new->key = key;                                             

    (*table)[index] = new;                             /* nakoniec pridam na idealnu poziciu do tabulky novy prvok */

  } else {

    ht_item_t *replace = ht_search(table, key);       /* v pripade, ze uz dany prvok existuje v tabulke, iba prepisem jeho hodnotu 'value' */
    replace->value = value;

  }


}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {

  ht_item_t *item = ht_search(table, key);
  if (item != NULL) {

    return &item->value;                    /* ak prvok s danym klucom existuje v tabulke, fcia ukazatel na jeho hodnotu */

  } else {

    return NULL;                          /* v opacnom pripade vracia NULL */

  }

}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {

  int index = get_hash(key);                    /* ziskam hodnotu prvku s idealnou poziciou pre dany kluc */
  ht_item_t *tmp = (*table)[index];

  if(tmp != NULL ) {                            /* ak prvok s danym klucom existuje... */

    if (tmp->key == key) {                      /* ak sa kluc s tymto prvok zhoduje s tym, ktory idem vymazat... */

      (*table)[index] = tmp->next;              /* na danu poziciu premiestnim nasledujuci od toho, ktory mazem */
      free(tmp);                                /* uvolnim pamet daneho prvku */
      

    } else {                                    /* ak sa nezhoduju kluce, musim prehladat zoznam synonym */

      ht_item_t *tmp_prev;
      while (tmp->next != NULL) {               /* cyklus prechadza zoznam synonym... */

        tmp_prev = tmp;
        tmp = tmp->next;
        if (tmp->key == key) {                  /* ak najde zhodu kluca */
          
          tmp_prev->next = tmp->next;           /* predchadzajuci bude mat nasledujuceho prvok za prvok, ktory idem mazat - preskoci ho */
          free(tmp);                            /* uvolnim pamet s danym prvkom a prerusim cyklus */
          break;

        }
      }
    }

  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */

void ht_delete_all(ht_table_t *table) {

  ht_item_t *tmp, *tmp2;
  for (int i = 0; i < HT_SIZE; i++) {       /* prechadzam prvky tabulky */

    tmp = (*table)[i];

    if (tmp != NULL) {                      /* ked najdem prvok, ktory ma nejaku realnu hodnotu, musim uvolnit aj cely jeho zoznam synonym */
      
      while (tmp->next != NULL) {           /* cyklus while prechadza zoznam syn. a uvolnuje pamet jednotlivych prvkov, okrem toho prveho */

        tmp2 = tmp;
        tmp = tmp->next; 
        free(tmp2);

      }

      free(tmp);                            /* az nakoniec uvolnim pamet prvku, ktory bol na zaciatku daneho zoznamu syn.*/
      (*table)[i] = NULL;                   /* aby tabulka bola ako po inicializacii, nastavim hodnotu aktualneho prvku na NULL */

    }
    
  }
}
