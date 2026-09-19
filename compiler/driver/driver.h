#ifndef SUPER_DRIVER_H
#define SUPER_DRIVER_H

#include <stdbool.h>
#include "arena/arena.h"

typedef struct {
    Arena *arena;
    const char *input_file;
    const char *output_file;
    bool emit_llvm;
    bool emit_asm;
    bool run_after_build;
} DriverConfig;

int driver_run(DriverConfig *config);

#endif