#include <lanc_lexer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_IDENTIFIER_BUFFER 256

static Token* lanc_lexer_make_token(Tokind tkind, unsigned int line, unsigned int column,  char * id, int lint32) {
    Token* token = calloc(1, sizeof(Token));
    token->self_kind = tkind;
    token->line = line + 1;
    token->column = column + 1;
    token->lit_int32 = lint32;
    token->id = id;
    return token;
}

static void lanc_lexer_allocator_append(ArrayTokenAllocator* allocator, Token* token) {
    if (allocator->size == allocator->capacity) {
        allocator->capacity *= 2;
        allocator->data = realloc(allocator->data, allocator->capacity * sizeof(Token*));
    }

    allocator->data[allocator->size++] = token;
}


static void lanc_lexer_tokenizer(ArrayTokenAllocator* allocator, char** path, size_t size) {

    for (unsigned int line = 0; line < size; ++line) {
        for (unsigned int column = 0; column < strlen(path[line]);) {
            if (path[line][column] == '\n' || path[line][column] == '\t' || path[line][column] == ' ') {
                column++;
                continue;
            } 
            if (strncmp(&path[line][column], "//", 2) == 0)  {
                while (path[line][column] != '\n') { column++; }
                continue;
            } else if (strncmp(&path[line][column], "let", 3) == 0) {
               lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_LET, line, column, NULL, 0));
               column+=3;
               continue;
            } else if (strncmp(&path[line][column], "proc", 4) == 0) {
               lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_PROC, line, column, NULL, 0));
               column+=4;
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
                printf("a = %s\n", name);

                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_ID, line, column - (int)ptr, name, 0));
                
                continue;
            } else if (isdigit(path[line][column])) {
                int value = 0;
                while (isdigit(path[line][column])) {
                    value = value * 10 + (path[line][column] - '0');
                    column++;
                }
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_INT, line, column - (int)strlen(path[line] + column - value), NULL, value));
                continue;
            } else if (path[line][column] == '=') {
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_EQ, line, column, NULL, 0));
                column++;
                continue;
            } else if (path[line][column] == '(') {
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_OPENPAREN, line, column, NULL, 0));
                column++;
                continue;
            } else if (path[line][column] == ')') {
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_CLOSEPAREN, line, column, NULL, 0));
                column++;
                continue;
            } else if (path[line][column] == '{') {
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_OPENBRACE, line, column, NULL, 0));
                column++;
                continue;
            } else if (path[line][column] == '}') {
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_CLOSEBRACE, line, column, NULL, 0));
                column++;
                continue;
            } else if (path[line][column] == ';') {
                lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_SEMI, line, column, NULL, 0));
                column++;
                continue;
            }
            column++; 
        }
    }
    // Append the EOF token.  This is a special token that signals the end of the input.
    lanc_lexer_allocator_append(allocator, lanc_lexer_make_token(KIND_TOKEN_EOF, 0, 0, NULL, 0));

}

static ArrayTokenAllocator* init_allocator() {
    ArrayTokenAllocator* allocator =  (ArrayTokenAllocator*) calloc(1, sizeof(ArrayTokenAllocator));
    allocator->size = 0;
    allocator->capacity = 3;
    allocator->data = (Token**) malloc(allocator->capacity * sizeof(Token*));
    return allocator;
}

ArrayTokenAllocator* lanc_lexer(char** path, size_t size) {
    ArrayTokenAllocator* allocator = init_allocator();

    lanc_lexer_tokenizer(allocator, path, size);
    return allocator;
}