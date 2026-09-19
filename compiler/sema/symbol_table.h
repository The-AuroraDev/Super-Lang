#ifndef SUPER_SYMBOL_TABLE_H
#define SUPER_SYMBOL_TABLE_H

#include "arena/arena.h"
#include "ast/ast.h"

typedef struct Symbol Symbol;
typedef struct Scope Scope;

Symbol *symbol_new(Arena *arena, const char *name, AstNode *decl);
Scope *scope_new(Arena *arena, Scope *parent);
void scope_define(Scope *scope, Symbol *sym);
Symbol *scope_lookup(Scope *scope, const char *name);
Symbol *scope_lookup_local(Scope *scope, const char *name);

#endif