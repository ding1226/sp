#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"


ASTNode *parse_expression();
ASTNode *parse_statement();
ASTNode *parse_block();
ASTNode *parse_do_while();

ASTNode *parse_expression() {
    Token token = get_next_token();
    ASTNode *node = malloc(sizeof(ASTNode));
    if (token.type == TOKEN_INT) {
        node->type = NODE_INT;
        node->value = atoi(token.text);
        node->left = node->right = NULL;
    } else if (token.type == TOKEN_IDENT) {
        node->type = NODE_IDENT;
        strcpy(node->name, token.text);
        node->left = node->right = NULL;
    }
    return node;
}

ASTNode *parse_statement() {
    Token token = peek_token();
    if (token.type == TOKEN_DO) {
        return parse_do_while();
    } else {
        return parse_expression(); 
    }
}

ASTNode *parse_block() {
    Token token = get_next_token();
    if (token.type != TOKEN_LBRACE) {
        fprintf(stderr, "Expected '{'\n");
        exit(1);
    }
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_STATEMENTS;
    node->left = node->right = NULL;
    ASTNode *current = node;
    while (peek_token().type != TOKEN_RBRACE) {
        ASTNode *stmt = parse_statement();
        if (current->left == NULL) {
            current->left = stmt;
        } else {
            current->right = stmt;
            ASTNode *new_node = malloc(sizeof(ASTNode));
            new_node->type = NODE_STATEMENTS;
            new_node->left = current;
            new_node->right = stmt;
            current = new_node;
        }
    }
    token = get_next_token();
    if (token.type != TOKEN_RBRACE) {
        fprintf(stderr, "Expected
