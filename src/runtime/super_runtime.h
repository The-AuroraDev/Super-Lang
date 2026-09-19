#ifndef SUPER_RUNTIME_H
#define SUPER_RUNTIME_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *data;
    size_t len;
    size_t cap;
} SuperSlice;

typedef struct {
    char *data;
    size_t len;
} SuperString;

void *super_alloc(size_t size);
void *super_realloc(void *ptr, size_t old_size, size_t new_size);
void super_free(void *ptr);

void super_panic(const char *msg);
void super_print(const char *str);
SuperString super_read_line(const char *prompt);

void *super_channel_make(size_t elem_size, size_t cap);
void super_channel_send(void *chan, const void *elem);
void super_channel_recv(void *chan, void *elem);
void super_channel_close(void *chan);

#ifdef __cplusplus
}
#endif

#endif