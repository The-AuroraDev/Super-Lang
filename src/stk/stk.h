#ifndef SUPER_STK_H
#define SUPER_STK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void **data;
    size_t len;
    size_t cap;
} Stk;

Stk *stk_new(size_t initial_cap);
void stk_free(Stk *s);
void stk_push(Stk *s, void *elem);
void *stk_pop(Stk *s);
void *stk_top(Stk *s);
size_t stk_size(Stk *s);
int stk_empty(Stk *s);

#ifdef __cplusplus
}
#endif

#endif