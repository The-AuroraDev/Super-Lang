#ifndef SUPER_SYS_H
#define SUPER_SYS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void sys_exit(int code);
char *sys_getenv(const char *key);
const char *sys_arch(void);
const char *sys_os(void);

#ifdef __cplusplus
}
#endif

#endif