#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

#define MAX_TOKEN_LENGTH 100

typedef struct {
    TokenType type;
    char text[MAX_TOKEN_LENGTH];
} Token;

Token tokens[MAX_TOKENS];
int token_index = 0;
int token_count = 0;

void tokenize(char *input) {
    char *p = input;
    while (*p != '\0') {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isalpha(*p)) {
            char *start = p;
            while (isalnum(*p)) p++;
            int length = p - start;
            Token token;
            if (strncmp(start, "do", length) == 0) {
                token.type = TOKEN_DO;
                strcpy(token.text, "do");
            } else if (strncmp(start, "while", length) == 0) {
                token.type = TOKEN_WHILE;
                strcpy(token.text, "while");
            } else {
                token.type = TOKEN_IDENT;
                strncpy(token.text, start, length);
                token.text[length] = '\0';
            }
            tokens[token_count++] = token;
        } else if (isdigit(*p)) {
            char *start = p;
            while (isdigit(*p)) p++;
            int length = p - start;
            Token token;
            token.type = TOKEN_INT;
            strncpy(token.text, start, length);
            token.text[length] = '\0';
            tokens[token_count++] = token;
        } else {
            Token token;
            switch (*p) {
                case '{': token.type = TOKEN_LBRACE; strcpy(token.text, "{"); break;
                case '}': token.type = TOKEN_RBRACE; strcpy(token.text, "}"); break;
                case '(': token.type = TOKEN_LPAREN; strcpy(token.text, "("); break;
                case ')': token.type = TOKEN_RPAREN; strcpy(token.text, ")"); break;
                case ';': token.type = TOKEN_SEMICOLON; strcpy(token.text, ";"); break;
                case '=': token.type = TOKEN_ASSIGN; strcpy(token.text, "="); break;
                case '+': token.type = TOKEN_PLUS; strcpy(token.text, "+"); break;
                case '-': token.type = TOKEN_MINUS; strcpy(token.text, "-"); break;
                case '*': token.type = TOKEN_ASTERISK; strcpy(token.text, "*"); break;
                case '/': token.type = TOKEN_SLASH; strcpy(token.text, "/"); break;
                default: fprintf(stderr, "Unknown character: %c\n", *p); exit(1);
            }
            tokens[token_count++] = token;
            p++;
        }
    }
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.text, "EOF");
    tokens[token_count++] = eof_token;
}

Token get_next_token() {
    if (token_index < token_count) {
        return tokens[token_index++];
    }
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.text, "EOF");
    return eof_token;
}

Token peek_token() {
    if (token_index < token_count) {
        return tokens[token_index];
    }
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.text, "EOF");
    return eof_token;
}
