#ifndef _LANC_PARSER_H_
#define _LANC_PARSER_H_

#include <lanc_lexer.h>

typedef struct Node Node;

typedef enum {
	KIND_OP_PLUS,    // `+`
	KIND_OP_ASSIGN   // `=`
} KindBinOp;

typedef enum {
	KIND_EXPR_VAR, 
} KindNodeExpr;


struct Node {
	struct Node* left;
	struct Node* right;
	KindNodeExpr expr;
	const char* name;
	union {
		int   i32_t;
		char  ichr_t;
	} value;
};


Node* lanc_parser_parse(LancToken* tokens);

#endif // _LANCE_PARSER_H_