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

Stk *stkNew(size_t initialCap);
void stkFree(Stk *s);
void stkPush(Stk *s, void *elem);
void *stkPop(Stk *s);
void *stkTop(Stk *s);
size_t stkSize(Stk *s);
int stkEmpty(Stk *s);

#ifdef __cplusplus
}
#endif

#endif