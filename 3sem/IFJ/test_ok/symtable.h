/**
* @file symtable.h
 * @brief Header for symtable.c
 *
 *
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */

#pragma once
#include <stdbool.h>
#include "tokeniser.h"

#ifndef SYMTABLE_H
#define SYMTABLE_H
#define SYMTABLE_SIZE 1021

/*hash table structures*/

typedef struct ht_item {                       //item structure
    struct ht_item *next;                      //pointer to next synonym
    char name[100];                                //name of item, doubles as key
    int type;                                  //data type
    bool func;                                  //true if item is a function
    bool defined;                              //true if item has been defined
    bool isconst;                              //true if item is constant
    bool isnullable;                           //true if item can be NULL
    bool used;
    bool changed;
} ht_item_t;

typedef struct ht_table {                      //hash table
    ht_item_t *hashTable[SYMTABLE_SIZE];       //array of hash items
    bool global;                               //global = true, when the hashtable is global, and therefore is at the bottom of the stack
} ht_table_t;

/*stack structures*/

typedef struct sElem {                         //element in stack
    ht_table_t *table;                         //pointer to hash table
    struct sElem *next;                        //pointer to next element
} sElem;

typedef struct Stack {                         //stack
    sElem *top;                                //pointer to top of the stack
    int lenght;                                //length of stack
} tStack;


/*hash table functions definitions*/
int get_hash(const char *name);
void initHash(ht_table_t *table);
ht_item_t *searchHash(ht_table_t *table, char *name);
ht_item_t *insertHash(tStack *stack, ht_table_t *table, char *name,int type, bool func, bool isconst, bool defined, bool isnullable, bool used, bool changed);
ht_item_t *insertHashSecondPhase(ht_table_t *table, char* name, int type, bool func, bool isconst, bool defined, bool isnullable, bool used, bool changed);
void deleteHash(ht_table_t *table, char *name);
void deleteTable(ht_table_t *table);

/*stack functions declaration*/
void sInit(tStack *stack);
void sPop( tStack *stack);
void sPush( tStack *stack, ht_table_t *table);
sElem *sTop( tStack *stack);
sElem *sBottom(tStack *stack);
bool sEmpty( tStack *stack);
void sDispose (tStack *stack);

#endif //SYMTABLE_H
