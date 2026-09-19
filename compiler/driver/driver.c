#include "driver.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "sema/sema.h"
#include "codegen/cgen.h"
#include "arena/arena.h"
#include <stdio.h>
#include <stdlib.h>

int driver_run(DriverConfig *config) {
    FILE *f = fopen(config->input_file, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open input file: %s\n", config->input_file);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *source = malloc(len + 1);
    fread(source, 1, len, f);
    source[len] = '\0';
    fclose(f);

    Arena *arena = arena_new(0);
    Lexer *lexer = lexer_new(arena, source, len);
    Parser *parser = parser_new(arena, lexer);
    AstNode *ast = parser_parse(parser);

    if (parser->had_error) {
        free(source);
        arena_free(arena);
        return 1;
    }

    Sema *sema = sema_new(arena);
    if (!sema_analyze(sema, ast)) {
        free(source);
        arena_free(arena);
        return 1;
    }

    FILE *out = stdout;
    if (config->output_file) {
        out = fopen(config->output_file, "w");
        if (!out) {
            fprintf(stderr, "Failed to open output file: %s\n", config->output_file);
            free(source);
            arena_free(arena);
            return 1;
        }
    }

    CGen *cgen = cgen_new(arena, out);
    cgen_generate(cgen, ast);
    cgen_free(cgen);

    if (out != stdout) fclose(out);
    free(source);
    arena_free(arena);
    return 0;
}