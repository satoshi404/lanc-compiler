#include <stdio.h>
#include <stdlib.h>
#include "lanc_parser.h"

// Função para imprimir a AST
int main() {
    char* path[] = {"let a = 88; a = 3;"};
    ArrayTokenAllocator* allocator = lanc_lexer(path);
    for (int i = 0; i < allocator->size ; i++) { 
        printf("Token: %s, Kind: %d, Line: %d, Column: %d\n", allocator->data[i]->id, allocator->data[i]->self_kind, allocator->data[i]->line, allocator->data[i]->column);
    }

    Node* ast = lanc_parser_parse(allocator->data);

    // Imprimir a AST
    printf("AST:\n");
    print_ast(ast, 0);

    //free(allocator->items);  // Free the memory allocated for the tokens.
    return 0;
}
