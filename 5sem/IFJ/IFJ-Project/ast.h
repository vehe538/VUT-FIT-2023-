/**
 * @file ast.h
 * @brief declaration of structures and functions 
 * for the Abstract Syntax Tree (AST) implementation.
 * 
 * @author Adam Kinzel (xkinzea00)
 * @author Juraj Mesík (xmesikj00)
 * @author Natália Holbíková (xholbin00)
 */

#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"

#ifndef AST_H
#define AST_H

/**
 * Enumeration of different node types in AST.
 */
typedef enum AstNodeType {
    NT_PROGRAM,

    NT_PLUS,
    NT_MINUS,
    NT_DIV,
    NT_MULT,
    NT_GT,
    NT_LT,
    NT_GE,
    NT_LE,
    NT_EQ,
    NT_NEQ,
    NT_IS,

    NT_FUNC,
    NT_FUNC_CALL,
    NT_PARAM,
    NT_ARG,
    NT_RETURN,

    NT_IF_ELSE,
    NT_FOR,
    NT_WHILE,

    NT_LITERAL,

    NT_NUM,
    NT_STRING,
    NT_NULL,

    NT_VAR,
    NT_ASSIGN,

    NT_GETTER,
    NT_SETTER,
    NT_BLOCK
} AstNodeType;

/**
 * Forward declaration of AstNode structure to allow recursive pointers.
 */
struct AstNode;

/**
 * Structure representing an AST node with numeric value.
 */
typedef struct AstNum {
    union {
        int intValue;
        float floatValue;
    } numValue;
} AstNum;

/**
 * Structure representing an AST node with string value.
 */
typedef struct AstString {
    char *value;
} AstString;

/**
 * Structure representing an AST node with a binary operation.
 */
typedef struct AstBinOp {
    struct AstNode *operand1;
    struct AstNode *operand2;
    AstNodeType opType;
} AstBinOp;

/**
 * Structure representing an AST node with a variable.
 */
typedef struct AstVar {
    char *name;
} AstVar;

/**
 * Structure representing an AST node with a function parameter.
 */
typedef struct AstParam {
    char *name;
} AstParam;

/**
 * Structure representing an AST node with a function argument.
 */
typedef struct AstArg {
    struct AstNode *expression;  // pointer to AST node with argument expression
} AstArg;

/**
 * Structure representing an AST node with an assignment.
 */
typedef struct AstAssign {
    struct AstNode *left;   // pointer to AST node to which we are assigning
    struct AstNode *right;  // pointer to AST node which is being assigned
} AstAssign;

/**
 * Structure representing an AST node with a while cycle.
 */
typedef struct AstWhile {
    struct AstNode *condition;  // pointer to AST node with condition expression
    struct AstNode *body;       // pointer to AST node with body statements
} AstWhile;

/**
 * Structure representing an AST node with a function.
 */
typedef struct AstFunc {
    char *name;                     // name of function
    struct AstNode* return_node;    // pointer to AST node with return expression
    struct AstNode* params;         // pointer to AST node with parameters
    struct AstNode* body;           // pointer to AST node with body statements
    int paramCnt;                   // parameter count
} AstFunc;

/**
 * Structure representing an AST node with a function call.
 */
typedef struct AstCallFunc {
    struct AstNode *args;           // call arguments
    char *name;                     // name of function being called
    struct AstNode* return_val;     // pointer to AST node for storing return value
    int argCnt;                     // argument count
} AstCallFunc;

/**
 * Structure representing an AST node with an if-statement.
 */
typedef struct AstIfElse {
    bool else_present;          // checking whether 'else' branch is present
    struct AstNode *condition;  // pointer to AST node with condition expression
    struct AstNode *true_body;  // pointer to AST node with statements in the 'true' branch
    struct AstNode *false_body; // pointer to AST node with statements in the 'false' branch
} AstIfElse;

/**
 * Structure representing an AST node with a block.
 */
typedef struct AstBlock {
    struct AstNode* firstStmt; // pointer to AST node with the first statement in the block
} AstBlock;

/**
 * Structure representing an AST node with a return expression.
 */
typedef struct AstReturn {
    struct AstNode *expr; // pointer to AST node representing the return value
} AstReturn;

/**
 * Main structure representing an AST node.
 */
typedef struct AstNode {
    AstNodeType type;       // type of AST node given by enum
    data_type expr_type;    // semantic type of AST node
    struct AstNode *parent; // pointer to parent node
    struct AstNode *next;   // pointer to next (sibling) node
    struct AstNode *prev;   // pointer to previous (sibling) node

    // Union listing the different types of AST node - specifying the required properties.
    union {
        AstNum num;
        AstString string;
        AstBinOp binOp;
        AstFunc func;
        AstCallFunc call;
        AstIfElse ifelse;
        AstWhile while_stmt;
        AstAssign assign;
        AstVar var;
        AstParam param;
        AstArg arg;
        AstBlock block;
        AstReturn ret;
    };
} AstNode;

/**
 * Prints the structure of AST, starting at the 
 * given node and going to a certain depth.
 * 
 * @param node  pointer to AST node which will start the hierarchy for printing
 * @param depth level of depth for printing
 */
void printAst(AstNode* node, int depth);

/**
 * Creates an AST node.
 * 
 * @param node      pointer to AST serving as parent/sibling
 * @param type      type of AST node for creation
 * @param isInner   specifies whether the node for creation is going to be a child or a sibling - child when true, sibling when false
 * 
 * @return pointer to newly-created AST node
 */
AstNode* createNode(AstNode* node, AstNodeType type, bool isInner);

/**
 * Initializes the AST.
 */
AstNode* initAst();

/**
 * Deletes an AST node.
 * 
 * @param node pointer to AST node for deletion
 */
void deleteNode(AstNode* node);

/**
 * Helper function which converts the enum value of AST node's type to a string value.
 * 
 * @param type enum value of AST node type
 * 
 * @return pointer to node type as a string
 */
const char* nodeTypeToString(AstNodeType type);

/**
 * Helper function for deleting AST structure, starting at a given node.
 * 
 * @param node pointer to AST node from which the deletion will begin
 */
void deleteNodeList(AstNode* node);

#endif