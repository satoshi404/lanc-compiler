#ifndef _LANC_PARSER_H_
#define _LANC_PARSER_H_

#include <lanc_lexer.h>

typedef struct Node Node;
typedef struct StmtExpr StmtExpr;

typedef enum {
    KIND_OP_PLUS,
} KindOp;

typedef enum {
    KIND_STMT_DEC_LET,
    KIND_STMT_ASSIGN,                      
} Stmt;

struct StmtExpr { 
    Stmt statement;
    Node* expr;
};

struct Node {
    struct Node* left;
    struct Node* right;
	struct Node* next;
    Stmt stmt_kind;
    char* id;
    union {
        int   i32_;
        char  char_;
    } lit;
};

Node* lanc_parser_parse(Token** tokens);

#endif // _LANC_PARSER_H_
