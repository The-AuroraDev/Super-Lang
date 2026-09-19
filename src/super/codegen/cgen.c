#include "super/codegen/cgen.h"
#include <stdlib.h>

CGen *cgen_new(Arena *arena, FILE *out) {
    (void)arena;
    CGen *cgen = malloc(sizeof(CGen));
    cgen->out = out;
    cgen->indent = 0;
    return cgen;
}

bool cgen_generate(CGen *cgen, AstNode *file) {
    (void)cgen;
    (void)file;
    return true;
}

void cgen_free(CGen *cgen) {
    free(cgen);
}