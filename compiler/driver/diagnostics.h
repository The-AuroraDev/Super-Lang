#ifndef SUPER_DIAGNOSTICS_H
#define SUPER_DIAGNOSTICS_H

#include "lexer/token.h"

typedef enum {
    DIAG_ERROR,
    DIAG_WARNING,
    DIAG_NOTE,
    DIAG_HELP,
} DiagLevel;

void diag_report(DiagLevel level, Token token, const char *message);
void diag_report_at(DiagLevel level, const char *file, size_t line, size_t col, const char *message);

#endif