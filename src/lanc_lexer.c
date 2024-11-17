#include <lanc_lexer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_IDENTIFIER_BUFFER 256

static LancToken lanc_lexer_make_token(Tokind tkind, unsigned int line, unsigned int column, char* tkstr, char * name, int value) {
    LancToken token = {0};
    token.tkind = tkind;
    token.line = line + 1;
    token.column = column + 1;
    token.tkstr = tkstr;  // Don't copy the string, just keep a reference.  The caller will have to free the string if needed.
    token.value = value;
    token.name = name;
    return token;
}

static void lanc_lexer_allocator_append(ArrayMemoryAllocator* allocator, LancToken token) {
    if (allocator->counter == allocator->capacity) {
        allocator->capacity *= 2;
        allocator->items = realloc(allocator->items, allocator->capacity * sizeof(LancToken));
    }

    allocator->items[allocator->counter++] = token;
} 

static void  lanc_lexer_tokenizer(ArrayMemoryAllocator* allocator, char** path ,size_t size) {
    unsigned int line = 0;
    unsigned int column = 0;

    for (unsigned int line = 0; line < size; ++line) {
        for (unsigned int column = 0; column < strlen(path[line]);) {
            if (path[line][column] == '\n' || path[line][column] == '\t' || path[line][column] == ' ') {
                column++;
                continue;
            } 
            if (strncmp(&path[line][column], "int", 3) == 0) {
               lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_INT, line, column, "<INT>", NULL, 0));
               column+=3;
               continue;
            } else if (isalpha(path[line][column])) {
                char name[MAX_IDENTIFIER_BUFFER] = {0};
                size_t ptr = 0;
                while (isalnum(path[line][column])) {
                    if (ptr >= MAX_IDENTIFIER_BUFFER - 1) {
                        fprintf(stderr, "Identifier too long at line %u, column %u\n", line+1, (column - (int)ptr) + 1);
                        exit(1);
                    }
                    name[ptr++] = path[line][column];
                    column++;
                }

                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_IDENTIFIER, line, column - (int)ptr, "<IDENTIFIER>", name, 0));
                continue;
            } else if (isdigit(path[line][column])) {
                int value = 0;
                while (isdigit(path[line][column])) {
                    value = value * 10 + (path[line][column] - '0');
                    column++;
                }
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_NUMBER, line, column - (int)strlen(path[line] + column - value), "<NUMBER>", NULL, value));
                continue;
            }
            column++; 
        }
    }
}

ArrayMemoryAllocator* lanc_lexing(char** path) {
    ArrayMemoryAllocator* allocator = malloc(sizeof(ArrayMemoryAllocator));
    allocator->counter = 0;
    allocator->capacity = 1;
    allocator->items = malloc(allocator->capacity * sizeof(LancToken));

    lanc_lexer_tokenizer(allocator, path, sizeof(path)/sizeof(path[0]));
    return allocator;
}