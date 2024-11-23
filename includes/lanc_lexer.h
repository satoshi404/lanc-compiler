#ifndef _LANC_LEXER_H_
#define _LANC_LEXER_H_
#include <stdio.h>

typedef struct Token Token;
typedef struct ArrayTokenAllocator ArrayTokenAllocator;

typedef enum {
    KIND_TOKEN_LET, // The keyword 'let'
    KIND_TOKEN_PROC,
    // -----------------------------------------------------------------------------
    KIND_TOKEN_PLUS, // The operator '+' 
    // -----------------------------------------------------------------------------
    KIND_TOKEN_OPENPAREN, // The operator '(' 
    KIND_TOKEN_CLOSEPAREN, // The operator ')'
    KIND_TOKEN_OPENBRACE, // The operator '{'
    KIND_TOKEN_CLOSEBRACE, // The operator '}'
    KIND_TOKEN_EQ, // The operator '='
    KIND_TOKEN_SEMI, // The operator ';'
    KIND_TOKEN_EOF, // The 'EOF' for limitation tokens list
    // -----------------------------------------------------------------------------
    KIND_TOKEN_ID, // The identifier 'identifier'
    KIND_TOKEN_INT, // The number 'number'
} Tokind;


struct Token {
    Tokind self_kind; // Token kind Tokind lol
    char* id; // If token is identifier
    int lit_int32; // If token is number literal 32 bit integer
    int line, column; // Token position (line, column)
};

struct ArrayTokenAllocator {
    Token** data; // Items of the array
    size_t capacity; // Capacity of memory allocated
    size_t size; // Size of memory allocated
};

ArrayTokenAllocator* lanc_lexer(char** path, size_t size);




#endif /* _LANC_LEXER_H_ */