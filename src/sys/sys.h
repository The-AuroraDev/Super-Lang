#ifndef SUPER_SYS_H
#define SUPER_SYS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void sysExit(int code);
char *sysGetenv(const char *key);
const char *sysArch(void);
const char *sysOS(void);

#ifdef __cplusplus
}
#endif

#endif