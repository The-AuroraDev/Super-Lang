#ifndef SUPER_CGEN_H
#define SUPER_CGEN_H

#include <stdio.h>
#include "super/arena/arena.h"
#include "super/ast/ast.h"

typedef struct {
    Arena *arena;
    FILE *out;
    int indent;
} CGen;

CGen *cgen_new(Arena *arena, FILE *out);
bool cgen_generate(CGen *cgen, AstNode *file);
void cgen_free(CGen *cgen);

#endif