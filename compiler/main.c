#include "lexer/lexer.h"
#include "arena/arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);
    *out_len = len;
    return buf;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.super>\n", argv[0]);
        return 1;
    }

    size_t len;
    char *source = read_file(argv[1], &len);
    if (!source) {
        fprintf(stderr, "Failed to read file: %s\n", argv[1]);
        return 1;
    }

    Arena *arena = arena_new(0);
    Lexer *lexer = lexer_new(arena, source, len);

    Token token;
    do {
        token = lexer_next(lexer);
        printf("%4zu:%-4zu %s", token.line, token.column, token_kind_name(token.kind));
        if (token.length > 0) {
            printf(" \"%.*s\"", (int)token.length, token.start);
        }
        printf("\n");
    } while (token.kind != TOKEN_EOF && token.kind != TOKEN_ERROR);

    free(source);
    arena_free(arena);
    return token.kind == TOKEN_ERROR ? 1 : 0;
}