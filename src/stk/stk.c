#include "stk.h"
#include <stdlib.h>
#include <string.h>

Stk *stkNew(size_t initialCap) {
    if (initialCap == 0) initialCap = 8;
    Stk *s = malloc(sizeof(Stk));
    if (!s) return NULL;
    s->data = malloc(initialCap * sizeof(void *));
    if (!s->data) { free(s); return NULL; }
    s->len = 0;
    s->cap = initialCap;
    return s;
}

void stkFree(Stk *s) {
    if (s) {
        free(s->data);
        free(s);
    }
}

static int stkGrow(Stk *s) {
    size_t newCap = s->cap * 2;
    void **newData = realloc(s->data, newCap * sizeof(void *));
    if (!newData) return -1;
    s->data = newData;
    s->cap = newCap;
    return 0;
}

void stkPush(Stk *s, void *elem) {
    if (s->len >= s->cap) {
        if (stkGrow(s) != 0) return;
    }
    s->data[s->len++] = elem;
}

void *stkPop(Stk *s) {
    if (s->len == 0) return NULL;
    return s->data[--s->len];
}

void *stkTop(Stk *s) {
    if (s->len == 0) return NULL;
    return s->data[s->len - 1];
}

size_t stkSize(Stk *s) {
    return s->len;
}

int stkEmpty(Stk *s) {
    return s->len == 0;
}