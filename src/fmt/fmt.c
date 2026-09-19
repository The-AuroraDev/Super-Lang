#include "fmt.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void fmt_print(const char *str) {
    fputs(str, stdout);
}

void fmt_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

int fmt_scanln(char *buf, size_t size) {
    if (fgets(buf, size, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
        return 0;
    }
    return -1;
}

char *fmt_sprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (len < 0) return NULL;
    
    char *buf = malloc(len + 1);
    if (!buf) return NULL;
    
    va_start(args, format);
    vsnprintf(buf, len + 1, format, args);
    va_end(args);
    
    return buf;
}