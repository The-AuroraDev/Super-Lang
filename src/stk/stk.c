#include "stk.h"
#include <stdlib.h>
#include <string.h>

Stk *stk_new(size_t initial_cap) {
    if (initial_cap == 0) initial_cap = 8;
    Stk *s = malloc(sizeof(Stk));
    if (!s) return NULL;
    s->data = malloc(initial_cap * sizeof(void *));
    if (!s->data) { free(s); return NULL; }
    s->len = 0;
    s->cap = initial_cap;
    return s;
}

void stk_free(Stk *s) {
    if (s) {
        free(s->data);
        free(s);
    }
}

static int stk_grow(Stk *s) {
    size_t new_cap = s->cap * 2;
    void **new_data = realloc(s->data, new_cap * sizeof(void *));
    if (!new_data) return -1;
    s->data = new_data;
    s->cap = new_cap;
    return 0;
}

void stk_push(Stk *s, void *elem) {
    if (s->len >= s->cap) {
        if (stk_grow(s) != 0) return;
    }
    s->data[s->len++] = elem;
}

void *stk_pop(Stk *s) {
    if (s->len == 0) return NULL;
    return s->data[--s->len];
}

void *stk_top(Stk *s) {
    if (s->len == 0) return NULL;
    return s->data[s->len - 1];
}

size_t stk_size(Stk *s) {
    return s->len;
}

int stk_empty(Stk *s) {
    return s->len == 0;
}