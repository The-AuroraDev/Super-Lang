#ifndef SUPER_OS_H
#define SUPER_OS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char *os_getwd(void);
int os_system(const char *cmd);
char **os_args(int *argc);
char *os_read_file(const char *path, size_t *len);
int os_write_file(const char *path, const char *data, size_t len);
int os_mkdir(const char *path, int mode);

#ifdef __cplusplus
}
#endif

#endif