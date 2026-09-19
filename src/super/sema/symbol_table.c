#include "super/sema/symbol_table.h"
#include "super/arena/arena.h"
#include <string.h>

struct Symbol {
    const char *name;
    AstNode *decl;
    Symbol *next;
};

struct Scope {
    Symbol *symbols;
    Scope *parent;
};

Symbol *symbol_new(Arena *arena, const char *name, AstNode *decl) {
    Symbol *sym = arena_alloc_t(arena, Symbol);
    sym->name = name;
    sym->decl = decl;
    sym->next = NULL;
    return sym;
}

Scope *scope_new(Arena *arena, Scope *parent) {
    Scope *scope = arena_alloc_t(arena, Scope);
    scope->symbols = NULL;
    scope->parent = parent;
    return scope;
}

void scope_define(Scope *scope, Symbol *sym) {
    sym->next = scope->symbols;
    scope->symbols = sym;
}

Symbol *scope_lookup(Scope *scope, const char *name) {
    for (Scope *s = scope; s; s = s->parent) {
        for (Symbol *sym = s->symbols; sym; sym = sym->next) {
            if (strcmp(sym->name, name) == 0) return sym;
        }
    }
    return NULL;
}

Symbol *scope_lookup_local(Scope *scope, const char *name) {
    for (Symbol *sym = scope->symbols; sym; sym = sym->next) {
        if (strcmp(sym->name, name) == 0) return sym;
    }
    return NULL;
}