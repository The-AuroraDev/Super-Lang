#define _GNU_SOURCE
#include "super_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void *global_allocator = NULL;

void *super_alloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        super_panic("Out of memory");
    }
    return memset(ptr, 0, size);
}

void *super_realloc(void *ptr, size_t old_size, size_t new_size) {
    (void)old_size;
    void *new_ptr = realloc(ptr, new_size);
    if (!new_ptr) {
        super_panic("Out of memory");
    }
    return new_ptr;
}

void super_free(void *ptr) {
    free(ptr);
}

void super_panic(const char *msg) {
    fprintf(stderr, "PANIC: %s\n", msg);
    abort();
}

void super_print(const char *str) {
    fputs(str, stdout);
}

SuperString super_read_line(const char *prompt) {
    if (prompt) fputs(prompt, stdout);
    fflush(stdout);
    char *line = NULL;
    size_t cap = 0;
    ssize_t len = getline(&line, &cap, stdin);
    if (len <= 0) {
        return (SuperString){NULL, 0};
    }
    if (line[len - 1] == '\n') line[--len] = '\0';
    return (SuperString){line, len};
}

typedef struct {
    void *buffer;
    size_t elem_size;
    size_t cap;
    size_t head;
    size_t tail;
    size_t count;
    void *mutex;
    void *not_empty;
    void *not_full;
} Channel;

void *super_channel_make(size_t elem_size, size_t cap) {
    Channel *ch = super_alloc(sizeof(Channel));
    ch->buffer = super_alloc(elem_size * (cap > 0 ? cap : 1));
    ch->elem_size = elem_size;
    ch->cap = cap > 0 ? cap : 1;
    ch->head = 0;
    ch->tail = 0;
    ch->count = 0;
    return ch;
}

void super_channel_send(void *chan, const void *elem) {
    (void)chan;
    (void)elem;
}

void super_channel_recv(void *chan, void *elem) {
    (void)chan;
    (void)elem;
}

void super_channel_close(void *chan) {
    (void)chan;
}