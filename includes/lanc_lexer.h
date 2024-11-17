#ifndef _LANC_LEXER_H_
#define _LANC_LEXER_H_

typedef struct LanceToken LancToken;

typedef enum {
    KIND_TOKEN_INT, // The keyword 'int'
    KIND_TOKEN_EOF, // The 'EOF' for limitation tokens list
    // -----------------------------------------------------------------------------
    KIND_TOKEN_PLUS, // The operator '+' 
    // -----------------------------------------------------------------------------
    KIND_TOKEN_IDENTIFIER, // The identifier 'identifier'
    KIND_TOKEN_NUMBER, // The number 'number'
} Tokind;


struct LanceToken {
    Tokind tkind; // Token kind Tokind lol
    char* name; // If token is identifier
    int value; // If token is number
    int line, column; // Token position (line, column)
};

LancToken* lanc_lexing(char* input);


#endif /* _LANC_LEXER_H_ */