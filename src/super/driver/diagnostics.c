#include "super/driver/diagnostics.h"
#include <stdio.h>

void diag_report(DiagLevel level, Token token, const char *message) {
    const char *prefix;
    switch (level) {
        case DIAG_ERROR: prefix = "error"; break;
        case DIAG_WARNING: prefix = "warning"; break;
        case DIAG_NOTE: prefix = "note"; break;
        case DIAG_HELP: prefix = "help"; break;
    }
    fprintf(stderr, "%s[%zu:%zu] %s: %s\n",
            prefix, token.line, token.column, prefix, message);
}

void diag_report_at(DiagLevel level, const char *file, size_t line, size_t col, const char *message) {
    (void)file;
    const char *prefix;
    switch (level) {
        case DIAG_ERROR: prefix = "error"; break;
        case DIAG_WARNING: prefix = "warning"; break;
        case DIAG_NOTE: prefix = "note"; break;
        case DIAG_HELP: prefix = "help"; break;
    }
    fprintf(stderr, "%s[%zu:%zu] %s: %s\n", prefix, line, col, prefix, message);
}