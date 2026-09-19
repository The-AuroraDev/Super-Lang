#include "sys.h"
#include <stdlib.h>
#include <stdio.h>

void sys_exit(int code) {
    exit(code);
}

char *sys_getenv(const char *key) {
    return getenv(key);
}

const char *sys_arch(void) {
#if defined(__x86_64__) || defined(_M_X64)
    return "amd64";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return "arm64";
#elif defined(__i386__) || defined(_M_IX86)
    return "386";
#else
    return "unknown";
#endif
}

const char *sys_os(void) {
#if defined(_WIN32)
    return "windows";
#elif defined(__APPLE__)
    return "darwin";
#elif defined(__linux__)
    return "linux";
#elif defined(__FreeBSD__)
    return "freebsd";
#elif defined(__OpenBSD__)
    return "openbsd";
#else
    return "unknown";
#endif
}