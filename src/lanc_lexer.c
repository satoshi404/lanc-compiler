#include <lanc_lexer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static LancToken lanc_lexer_make_token(Tokind tkind, unsigned int line, unsigned int column, char* tkstr, char * name, int value) {
    LancToken token = {0};
    token.tkind = tkind;
    token.line = line;
    token.column = column;
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
        for (unsigned int column = 0; column < strlen(path[line]); ++column) {
            if (path[line][column] == '\n' || path[line][column] == '\t' || path[line][column] == ' ') {
                column++;
                continue;
            } 
            if (strncmp(&path[line][column], "int", 3) == 0) {
               lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_INT, line, column, "<INT>", NULL, 0));
               column+=2;
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