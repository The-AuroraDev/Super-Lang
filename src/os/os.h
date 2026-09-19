#ifndef SUPER_OS_H
#define SUPER_OS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char *osGetwd(void);
int osSystem(const char *cmd);
char **osArgs(int *argc);
char *osReadFile(const char *path, size_t *len);
int osWriteFile(const char *path, const char *data, size_t len);
int osMkdir(const char *path, int mode);

#ifdef __cplusplus
}
#endif

#endif