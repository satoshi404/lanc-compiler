#ifndef _LANC_LEXER_H_
#define _LANC_LEXER_H_
#include <stdio.h>

typedef struct LancToken LancToken;
typedef struct ArrayMemoryAllocator ArrayMemoryAllocator;

typedef enum {
    KIND_TOKEN_INT, // The keyword 'int'
    // -----------------------------------------------------------------------------
    KIND_TOKEN_PLUS, // The operator '+' 
    // -----------------------------------------------------------------------------
    KIND_TOKEN_OPENPAREN, // The operator '(' 
    KIND_TOKEN_CLOSEPAREN, // The operator ')'
    KIND_TOKEN_EQUALS, // The operator '='
    KIND_TOKEN_SEMICOLON, // The operator ';'
    KIND_TOKEN_EOF, // The 'EOF' for limitation tokens list
    // -----------------------------------------------------------------------------
    KIND_TOKEN_IDENTIFIER, // The identifier 'identifier'
    KIND_TOKEN_NUMBER, // The number 'number'
} Tokind;


struct LancToken {
    Tokind tkind; // Token kind Tokind lol
    char* tkstr; // Token to string
    char* name; // If token is identifier
    int value; // If token is number
    int line, column; // Token position (line, column)
};

struct ArrayMemoryAllocator {
    LancToken* items; // Items of the array
    size_t capacity; // Capacity of memory allocated
    size_t counter; // Size of memory allocated
};

ArrayMemoryAllocator* lanc_lexing(char** path);


const char* to_kind_string(Tokind kind) {
    switch (kind) {
        case KIND_TOKEN_INT: return "INT"; break;
        case KIND_TOKEN_PLUS: return "+"; break;
        case KIND_TOKEN_OPENPAREN: return "("; break;
        case KIND_TOKEN_CLOSEPAREN: return ")"; break;
        case KIND_TOKEN_EQUALS: return "="; break;
        case KIND_TOKEN_SEMICOLON: return ";"; break;
        case KIND_TOKEN_IDENTIFIER: return "ID"; break;
        case KIND_TOKEN_NUMBER: return "NUMBER"; break;
        default: return "UNKNOWN"; break;
    }
}


#endif /* _LANC_LEXER_H_ */