
/**
 * @file parser.h
 * @brief Header for parser.
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */

#ifndef PARSER_H
#define PARSER_H

#include "tokeniser.h"
#include "symtable.h"
#include <stdbool.h>
#include <stdio.h>




typedef struct {

    tStack *hashStack;
    ht_table_t *table;
    Token *current_token;
    Token *next_token;
    FILE *file;

} tParser ;

void start(FILE *file);
void update(tParser **current);
bool checkForProlog(Token *token, FILE *file);
void funcDefs(tParser **current);
void funcDef(tParser **current);
void param(tParser **current);
void nextparam(tParser **current);
void type(tParser **current, ht_item_t *variable);
void rtype(tParser **current, ht_item_t *func);
void stlist(tParser **current);
void vardecl(tParser **current);
void typing(tParser **current, ht_item_t *variable);
bool vartype(tParser **current);
void idst(tParser **current);
void assign(tParser **current, ht_item_t *variable);
void value(tParser **current, ht_item_t *variable);
bool builtin(tParser **current);
void funccall(tParser **current, ht_item_t *func, bool isbuiltin);
void callparam(tParser **current, ht_item_t *func, bool isbuiltin);
void ifcond(tParser **current);
void whileloop(tParser **current);
void nonnull_result(tParser **current);
void returnst(tParser **current);


#endif
