#ifndef SUPER_FMT_H
#define SUPER_FMT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void fmt_print(const char *str);
void fmt_printf(const char *format, ...);
int fmt_scanln(char *buf, size_t size);
char *fmt_sprintf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif