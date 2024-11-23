#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lanc_parser.h"


#define MAX_BUFFER_BY_LINE 1024


// Função para imprimir a AST
int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>.lanc\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");

    size_t cap = 1;
    size_t size = 0;
    char single_line[MAX_BUFFER_BY_LINE];
    char** path = (char**) calloc(cap, sizeof(char*));

    while (fgets(single_line, sizeof(single_line), fp)) {
        if (size == cap) {
            cap *= 2;
            path = realloc(path, cap * sizeof(char*));
        }
        path[size++] = strdup(single_line);
    }


   ArrayTokenAllocator* allocator = lanc_lexer(path, size);
 
   free(allocator->data);  // Free the memory allocated for the tokens.
   return 0;
}
