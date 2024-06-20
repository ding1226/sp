// compiler.h

#ifndef COMPILER_H
#define COMPILER_H

#define MAX_TOKENS 1000

typedef enum {
    TOKEN_INT,
    TOKEN_IDENT,
    TOKEN_DO,
    TOKEN_WHILE,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_EOF,
} TokenType;

typedef enum {
    NODE_INT,
    NODE_IDENT,
    NODE_ASSIGN,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_DO_WHILE,
    NODE_STATEMENTS,
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *condition;
    struct ASTNode *body;
    int value;
    char name[MAX_TOKEN_LENGTH];
} ASTNode;

void tokenize(char *input);
Token get_next_token();
Token peek_token();

ASTNode *parse_program();
void interpret(ASTNode *node);

#endif 
