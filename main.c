// main.c

#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

void print_ast(ASTNode *node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) printf("  ");
    switch (node->type) {
        case NODE_INT:
            printf("Int: %d\n", node->value);
            break;
        case NODE_IDENT:
            printf("Ident: %s\n", node->name);
            break;
        case NODE_ASSIGN:
            printf("Assign: %s\n", node->name);
            print_ast(node->right, depth + 1);
            break;
        case NODE_ADD:
            printf("Add:\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case NODE_DO_WHILE:
            printf("Do While:\n");
            printf("  Body:\n");
            print_ast(node->left, depth + 2);
            printf("  Condition:\n");
            print_ast(node->condition, depth + 2);
            break;
        case NODE_STATEMENTS:
            printf("Statements:\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        default:
            printf("Unknown node type: %d\n", node->type);
            break;
    }
}

void interpret(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_INT:

            break;
        case NODE_IDENT:

            break;
        case NODE_ASSIGN:

            break;
        case NODE_ADD:

            break;
        case NODE_DO_WHILE:
            do {
                interpret(node->left);  
            } while (interpret_condition(node->condition)); 
            break;
        case NODE_STATEMENTS:
            interpret(node->left);
            interpret(node->right);
            break;
        default:
            fprintf(stderr, "Unknown node type: %d\n", node->type);
            break;
    }
}

int interpret_condition(ASTNode *node) {

    if (node->type == NODE_INT) {
        return node->value != 0;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = malloc(length + 1);
    fread(source, 1, length, file);
    fclose(file);
    source[length] = '\0';


    tokenize(source);


    ASTNode *ast = parse_program();


    print_ast(ast, 0);


    interpret(ast);


    free(source);


    return 0;
}

