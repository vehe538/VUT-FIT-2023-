/**
* @file symtable.c
 * @brief Implementation of symtable.
 *
 *
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */

#pragma once
#include "symtable.h"
#include <stdbool.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokeniser.h"
#include <string.h>
#include "stdbool.h"
#include "error.h"


/*creates hash from name of variable*/
int get_hash(const char *name) {
    int hash_value = 0;
    int ascii_value = name[0];
    for (int i = 0; ascii_value != 0; i++) {

        hash_value = ((hash_value * 31 + ascii_value) % 1021);
        ascii_value = name[i+1];
    }
    return hash_value;
}

/*Inicializes hash table*/
void initHash(ht_table_t *table) {
    for (int i = 0; i< SYMTABLE_SIZE; i++) {
        (table->hashTable)[i] = NULL;
    }
}

/*looks at each item in the table and finds item with matching name*/
ht_item_t *searchHash(ht_table_t *table, char *name) {
    ht_item_t *found = NULL;
    int hash = get_hash(name);
    ht_item_t *item = (table->hashTable[hash]);
    while (item != NULL)
    {
        if (item != NULL && (strcmp(item->name, name) == 0)) {
            found = item;
            break;
        }
        item = item->next;
    }
    return found;
}

/*if there is function declaration, it will be added to bottom of stack*/
ht_item_t *insertHash(tStack *stack, ht_table_t *table, char *name,int type, bool func, bool isconst, bool defined, bool isnullable, bool used, bool changed) {
    if (func) {
        return insertHashSecondPhase(sBottom(stack)->table, name, type, func, isconst, defined, isnullable, used, changed);
    }else {
        return insertHashSecondPhase(table, name, type, func, isconst, defined, isnullable, used, changed);
    }
}

/*searches table for identical item, if it doesn't find collision it creates new item, if it finds synonym, it creates the new item in front of the synonym*/
ht_item_t *insertHashSecondPhase(ht_table_t *table, char* name, int type, bool func, bool isconst, bool defined, bool isnullable, bool used, bool changed) {
    ht_item_t *tmp = searchHash(table, name);
    if (tmp == NULL) {
        ht_item_t *new = (ht_item_t *)malloc(sizeof(ht_item_t));
        if (new == NULL) {pushError("memory allocation error", 99);}

        int index = get_hash(name);
        new->next = NULL;
        for (int i = 0; i < 99; i++) {
            new->name[i] = name[i];
        }
        new->type = type;
        new->func = func;
        new->isconst = isconst;
        new->defined = defined;
        new->isnullable = isnullable;
        new->used = used;
        new->changed = changed;

        ht_item_t *syn = (table->hashTable)[index];
        if (syn != NULL) {
            if ((strcmp(syn->name, name) == 0))
            {
                pushError("Two variables share same identificator", 5);
                return NULL;
            }
            new->next = syn;
        }
        (table->hashTable)[index] = new;
        return new;
    } else {
        pushError("Two variables share same identificator", 5);
        return NULL;
    }
}

/*deletes item from hash table*/
void deleteHash(ht_table_t *table, char *name) {
    int index = get_hash(name);
    ht_item_t *tmp = (table->hashTable)[index];
    if(tmp != NULL ) {
        if (strcmp(tmp->name, name) == 0) {
            (table->hashTable)[index] = tmp->next;
            free(tmp);
            return;
        } else {
            ht_item_t *tmp_prev;
            while (tmp->next != NULL) {
                tmp_prev = tmp;
                tmp = tmp->next;
                if (strcmp(tmp->name, name) == 0) {
                    tmp_prev->next = tmp->next;
                    free(tmp);
                    break;
                }
            }
        }
    }
}

/*deletes hash table and its items*/
void deleteTable(ht_table_t *table) {
    ht_item_t *tmp, *tmp2;
    for (int i = 0; i < SYMTABLE_SIZE; i++) {
        if ((table->hashTable)[i] != NULL) {
          tmp2 = (table->hashTable)[i];
            while (tmp2->next != NULL) {
                tmp = tmp2;
                tmp2 = tmp2->next;
                free(tmp);
            }
            free(tmp2);
            (table->hashTable)[i] = NULL;
        }
    }
}

/*Initializes stack for hashtables*/
void sInit (tStack *stack) {
    stack->top = NULL;
    stack->lenght = 0;
}

/*Pops stack*/
void sPop (tStack *stack) {
    if (stack->top != NULL && stack->lenght > 0) {
        sElem *firstElement = stack->top;
        stack->top = firstElement->next;
        free(firstElement);
        stack->lenght--;
    }
}

/*Pushes stack*/
void sPush (tStack *stack, ht_table_t *table) {
    sElem *newElement = (sElem *)malloc(sizeof(*newElement));
    if (newElement == NULL) {
        pushError("memory allocation failed\n", 99);
        return;
    }
    newElement->table = table;
    newElement->next = stack->top;
    stack->top = newElement;
    stack->lenght++;
    newElement->table->global = false;
}

/*returns hashtable thats on top of stack*/
sElem *sTop (tStack *stack) {
    return stack->top;
}

/*returns hashtable thats on the bottom of stack*/
sElem *sBottom(tStack *stack) {
    sElem *tmp= stack->top;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    return tmp;
}

/*checks if stack is empty or not*/
bool sEmpty (tStack *stack) {
    return (stack->top->table->global == true);
}

/*Pops until the stack is empty*/
void sDispose (tStack *stack) {
    while (sTop(stack) != NULL) {
        sPop(stack);
    }
    stack->top = NULL;
    stack->lenght = 0;
}