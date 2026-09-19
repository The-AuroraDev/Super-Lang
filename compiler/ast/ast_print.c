#include "ast.h"
#include <stdio.h>

void ast_print(AstNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%s\n", "TODO");
}