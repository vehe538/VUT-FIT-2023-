#include "parser.h"
#include "ast.h"
#include "symtable.h"
#include "generator.h"
#include "error_codes.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    fprintf(stderr, "=== IFJ25 Compiler Starting ===\n");
    
    // Initialize AST
    AstNode *ast = initAst();
    if (ast == NULL) {
        fprintf(stderr, "ERROR: Failed to initialize AST\n");
        exit(INTERNAL_ERROR);
    }

    // Initialize symbol table stack
    symtable_stack_t *symtable_stack = init_stack();
    if (symtable_stack == NULL) {
        fprintf(stderr, "ERROR: Failed to initialize symbol table stack\n");
        exit(INTERNAL_ERROR);
    }

    // Parse the input
    parse(ast, symtable_stack);

    symbol_t *global_table = NULL;
    
    if (symtable_stack != NULL && symtable_stack->top != NULL) {
        stack_element_t *current = symtable_stack->top;
        while (current->next != NULL) {
            current = current->next;
        }
        
        if (current->symbol_table != NULL) {
            global_table = *(current->symbol_table);
        }
    }

    // Generate code
    generateCode(ast, global_table);

    // Clean up
    fprintf(stderr, "Cleaning up resources...\n");
    delete_stack(symtable_stack);
    deleteNode(ast);
    fprintf(stderr, "=== Compilation completed successfully ===\n");

    return 0;
}
