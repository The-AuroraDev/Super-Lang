#include "os.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

char *os_getwd(void) {
    char *buf = NULL;
    size_t size = 1024;
    while (1) {
        buf = realloc(buf, size);
        if (!buf) return NULL;
        if (getcwd(buf, size)) return buf;
        if (errno != ERANGE) { free(buf); return NULL; }
        size *= 2;
    }
}

int os_system(const char *cmd) {
    return system(cmd);
}

char **os_args(int *argc) {
    extern char **environ;
    (void)argc;
    return environ;
}

char *os_read_file(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long l = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(l + 1);
    if (!buf) { fclose(f); return NULL; }
    fread(buf, 1, l, f);
    buf[l] = '\0';
    fclose(f);
    if (len) *len = l;
    return buf;
}

int os_write_file(const char *path, const char *data, size_t len) {
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, len, f);
    fclose(f);
    return written == len ? 0 : -1;
}

int os_mkdir(const char *path, int mode) {
    return mkdir(path, mode);
}