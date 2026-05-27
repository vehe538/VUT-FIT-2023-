/**
* @file precedence_table.h
 * @brief Header file for precedence table; declaration of precedence-typed tokens.
 *
 *
 * Authors:
 * - Juraj Mesík (xmesikj00), 2025
 */

#include <stdio.h>
#include <string.h>

#ifndef PREC_TABLE_H
#define PREC_TABLE_H

#define TABLE_SIZE 19

/* 

EOE - End Of Expression - special case where last bracket of epxression is returned
to parser, mainly if expressions that must be wrapped in one set of brackets ( if/while conditions).

In precedence table, 'o' means EOE.

*/

typedef enum {

    REDUCTION,  
    SHIFT,
    EQUAL,
    ERROR,
    EOE         

} prec_table_op;

typedef enum {

    T_PLUS,         // + 
    T_MINUS,        // -
    T_MULTIPLY,     // *
    T_DIVIDE,       // /
    T_BRACKET_OPEN, // (
    T_BRACKET_CLOSE,// )
    T_LESS,         // <
    T_GREATER,       // >
    T_LESS_EQUALS,  // <=
    T_GREATER_EQUALS,// >=
    T_EQUAL,        // ==
    T_NOT_EQUAL,    // !=
    T_STACK_BOTTOM, // $
    T_NUM,          // literal of type Num
    T_STRING,       // literal of type String
    T_NULL,         // null
    T_IDENTIFIER,   // identifier of global/local var or static getter
    T_IS,           // type-checking keyword 'is' 
    T_TYPE_KEYWORD, // keywords: 'String', 'Null', 'Num'

    T_HANDLE_START, // 
    T_ERROR,        //
    T_BOOL          //

}  prec_table_token;

extern const prec_table_op table[TABLE_SIZE][TABLE_SIZE];


#endif