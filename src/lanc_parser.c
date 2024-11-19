#include <lanc_parser.h>


static LancToken lanc_parser_get_token(unsigned int cursor, LancToken* tokens) {
	return tokens[cursor];
}


static LancToken lanc_parser_expect(LancToken token, Tokind kind) {
	if (token.tkind != kind) {
		printf("Error: Expected `%s` but got `%s`\n", to_kind_string(token.tkind), to_kind_string(kind));
		exit(1);
	}
	return token; 
}


static Node* lanc_parser_parser_int_expression(unsigned int *cursor, LancToken* tokens) {
	Node* node_expr;
	*cursor++; // Consume the int kind
	LancToken id = lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_IDENTIFIER);
	*cursor++; // Consume identifier
	lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_EQUALS);
	*cursor++; // Consume equals
	LancToken value = lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_NUMBER);
	*cursor++; // Consume the value
	lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_SEMICOLON);

	node_expr->expr = KIND_EXPR_VAR;
	node_expr->

	return node_expr;

}

static Node* lanc_parse_primary_expresion(unsigned int* cursor, LancToken* tokens) {
	if (lanc_parser_get_token(cursor, tokens).tkind == KIND_TOKEN_INT) {
		return lanc_parser_parser_int_expression(cursor, tokens);
	} else {
		printf("Error: Not a primary expression `%s`\n", lanc_parser_get_token(cursor, tokens).tkstr);
		exit(1);
	}
}

Node* lanc_parser_parse(LancToken* tokens) {
		Node* root = NULL;

		unsigned int cursor = 0;

		while (lanc_parser_get_token(cursor, tokens).tkind != KIND_TOKEN_EOF) {
			Node* node_expr = lanc_parse_primary_expresion(&cursor, tokens);
			cursor++;
		}
		

		return root;
}