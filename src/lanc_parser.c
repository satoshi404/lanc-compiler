#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lanc_parser.h"

static const char* to_kind_string(Tokind kind) {
    switch (kind) {
        case KIND_TOKEN_LET: return "LET"; break;
        case KIND_TOKEN_PLUS: return "+"; break;
        case KIND_TOKEN_OPENPAREN: return "("; break;
        case KIND_TOKEN_CLOSEPAREN: return ")"; break;
        case KIND_TOKEN_EQ: return "="; break;
        case KIND_TOKEN_SEMI: return ";"; break;
        case KIND_TOKEN_ID: return "ID"; break;
        case KIND_TOKEN_INT: return "NUMBER"; break;
        default: return "UNKNOWN"; break;
    }
}

static Token* lanc_parser_get_token(unsigned int *cursor, Token** tokens) {
    Token* token = tokens[*cursor];
    return token;
}

static Token* lanc_parser_expect(Token* token, Tokind kind) {
    if (token->self_kind != kind) {
        printf("Error: Expected `%s` but got `%s`\n", to_kind_string(kind), to_kind_string(token->self_kind));
        exit(1);
    }
    return token; 
}

static Node* create_node(Stmt stmt_kind) {
    Node* node = (Node*)calloc(1, sizeof(Node));
    if (node == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    node->stmt_kind = stmt_kind;
    node->id = NULL;
    return node;
}

static Node* parse_primary_expression(unsigned int* cursor, Token** tokens) {
    Node* node = NULL;
    Token* token = lanc_parser_get_token(cursor, tokens);

    if (token->self_kind == KIND_TOKEN_INT) {
        node = create_node(KIND_STMT_ASSIGN);
        node->lit.i32_ = token->lit_int32;
        (*cursor)++;
    } else if (token->self_kind == KIND_TOKEN_ID) {
        node = create_node(KIND_STMT_ASSIGN);
        node->id = strdup(token->id);
        (*cursor)++;
    } else {
        printf("Error: Not a primary expression `%s`\n", to_kind_string(token->self_kind));
        exit(1);
    }

    return node;
}

static Node* parse_expression(unsigned int* cursor, Token** tokens);

static Node* parse_binary_op(unsigned int* cursor, Token** tokens, Node* left) {
    Token* token = lanc_parser_get_token(cursor, tokens);

    if (token->self_kind == KIND_TOKEN_PLUS) {
        (*cursor)++;
        Node* node = create_node(KIND_STMT_ASSIGN);
        node->stmt_kind = KIND_OP_PLUS;
        node->left = left;
        node->right = parse_expression(cursor, tokens);
        return node;
    } else {
        printf("Error: Unsupported binary operator `%s`\n", to_kind_string(token->self_kind));
        exit(1);
    }
}

static Node* parse_expression(unsigned int* cursor, Token** tokens) {
    Node* left = parse_primary_expression(cursor, tokens);
    Token* token = lanc_parser_get_token(cursor, tokens);

    if (token->self_kind == KIND_TOKEN_PLUS) {
        return parse_binary_op(cursor, tokens, left);
    } else {
        return left;
    }
}

static Node* parse_variable_declaration(unsigned int* cursor, Token** tokens) {
    (*cursor)++; // Consume "let"
    Token* id = lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_ID);
    (*cursor)++;
    lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_EQ);
    (*cursor)++;
    Node* expr = parse_expression(cursor, tokens);
    lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_SEMI);
    (*cursor)++;

    Node* node = create_node(KIND_STMT_DEC_LET);
    node->id = strdup(id->id); // Use strdup to copy the identifier
    node->right = expr;

    return node;
}

static Node* parse_variable_assign(unsigned int* cursor, Token** tokens) { 
    Token* id = lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_ID);
    (*cursor)++;
    lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_EQ);
    (*cursor)++;
    Node* expr = parse_expression(cursor, tokens);
    lanc_parser_expect(lanc_parser_get_token(cursor, tokens), KIND_TOKEN_SEMI);
    (*cursor)++;

    Node* node = create_node(KIND_STMT_ASSIGN);
    node->id = strdup(id->id); // Use strdup to copy the identifier
    node->right = expr;

    return node;
}

Node* lanc_parser_parse(Token** tokens) {
    Node* root = NULL;
    Node* current = NULL;
    unsigned int cursor = 0;

    while (lanc_parser_get_token(&cursor, tokens)->self_kind != KIND_TOKEN_EOF) {
        Token* token = lanc_parser_get_token(&cursor, tokens);
        Node* node_expr = NULL;

        if (token->self_kind == KIND_TOKEN_LET) {
            node_expr = parse_variable_declaration(&cursor, tokens);
        } else if (token->self_kind == KIND_TOKEN_ID) {
            node_expr = parse_variable_assign(&cursor, tokens);
        } else {
            printf("Error: Unexpected token `%s`\n", to_kind_string(token->self_kind));
            exit(1);
        }

        if (root == NULL) {
            root = node_expr;
            current = root;
        } else {
            current->next = node_expr;
            current = node_expr;
        }
    }

    return root;
}

// Função para imprimir a AST
void print_ast(Node* node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    switch (node->stmt_kind) {
        case KIND_STMT_DEC_LET:
            printf("let %s = ", node->id ? node->id : "(null)");
            if (node->right) {
                if (node->right->stmt_kind == KIND_STMT_ASSIGN) {
                    printf("%d\n", node->right->lit.i32_);
                } else {
                    printf("(complex expression)\n");
                }
            } else {
                printf("null\n");
            }
            break;

        case KIND_STMT_ASSIGN:
            printf("ASSIGNMENT\n");
            if (node->id) {
                printf("%s = ", node->id);
            } else {
                printf("(null) = ");
            }
            if (node->right) {
                printf("%d\n", node->right->lit.i32_);
            } else {
                printf("null\n");
            }
            break;

        default:
            printf("Unknown statement kind\n");
            break;
    }

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
    print_ast(node->next, depth);
}
