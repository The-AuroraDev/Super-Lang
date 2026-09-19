#ifndef SUPER_FMT_H
#define SUPER_FMT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void fmtPrint(const char *str);
void fmtPrintf(const char *format, ...);
int fmtScanln(char *buf, size_t size);
char *fmtSprintf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif