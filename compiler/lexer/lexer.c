#include "lexer.h"
#include "keywords.h"
#include "arena/arena.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

Lexer *lexer_new(Arena *arena, const char *source, size_t length) {
    Lexer *lexer = arena_alloc_t(arena, Lexer);
    lexer->source = source;
    lexer->length = length;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->line_start = 0;
    lexer->arena = arena;
    return lexer;
}

static inline char lexer_peek(Lexer *lexer) {
    if (lexer->pos >= lexer->length) return '\0';
    return lexer->source[lexer->pos];
}

static inline char lexer_peek_next(Lexer *lexer) {
    if (lexer->pos + 1 >= lexer->length) return '\0';
    return lexer->source[lexer->pos + 1];
}

static inline char lexer_advance(Lexer *lexer) {
    if (lexer->pos >= lexer->length) return '\0';
    char c = lexer->source[lexer->pos++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
        lexer->line_start = lexer->pos;
    } else {
        lexer->column++;
    }
    return c;
}

static inline void lexer_skip_whitespace(Lexer *lexer) {
    while (1) {
        char c = lexer_peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            lexer_advance(lexer);
        } else if (c == '/' && lexer_peek_next(lexer) == '/') {
            while (lexer_peek(lexer) != '\n' && lexer_peek(lexer) != '\0') {
                lexer_advance(lexer);
            }
        } else if (c == '/' && lexer_peek_next(lexer) == '*') {
            lexer_advance(lexer);
            lexer_advance(lexer);
            while (1) {
                char c = lexer_peek(lexer);
                if (c == '\0') break;
                if (c == '*' && lexer_peek_next(lexer) == '/') {
                    lexer_advance(lexer);
                    lexer_advance(lexer);
                    break;
                }
                lexer_advance(lexer);
            }
        } else {
            break;
        }
    }
}

static Token make_token(Lexer *lexer, TokenKind kind, const char *start, size_t length) {
    Token token;
    token.kind = kind;
    token.start = start;
    token.length = length;
    token.line = lexer->line;
    token.column = lexer->column - length;
    token.offset = start - lexer->source;
    return token;
}

static Token scan_number(Lexer *lexer) {
    const char *start = lexer->source + lexer->pos - 1;
    bool is_float = false;

    while (isdigit(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }

    if (lexer_peek(lexer) == '.' && isdigit(lexer_peek_next(lexer))) {
        is_float = true;
        lexer_advance(lexer);
        while (isdigit(lexer_peek(lexer))) {
            lexer_advance(lexer);
        }
    }

    if (lexer_peek(lexer) == 'e' || lexer_peek(lexer) == 'E') {
        is_float = true;
        lexer_advance(lexer);
        if (lexer_peek(lexer) == '+' || lexer_peek(lexer) == '-') {
            lexer_advance(lexer);
        }
        while (isdigit(lexer_peek(lexer))) {
            lexer_advance(lexer);
        }
    }

    size_t length = lexer->pos - (start - lexer->source);
    return make_token(lexer, is_float ? TOKEN_FLOAT_LIT : TOKEN_INT_LIT, start, length);
}

static Token scan_string(Lexer *lexer) {
    const char *start = lexer->source + lexer->pos - 1;
    while (1) {
        char c = lexer_peek(lexer);
        if (c == '"' || c == '\0' || c == '\n') break;
        if (c == '\\') {
            lexer_advance(lexer);
            lexer_advance(lexer);
        } else {
            lexer_advance(lexer);
        }
    }
    if (lexer_peek(lexer) == '"') {
        lexer_advance(lexer);
    }
    size_t length = lexer->pos - (start - lexer->source);
    return make_token(lexer, TOKEN_STRING_LIT, start, length);
}

static Token scan_ident(Lexer *lexer) {
    const char *start = lexer->source + lexer->pos - 1;
    while (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_') {
        lexer_advance(lexer);
    }
    size_t length = lexer->pos - (start - lexer->source);
    TokenKind kind = keyword_lookup(start, length);
    return make_token(lexer, kind, start, length);
}

Token lexer_next(Lexer *lexer) {
    lexer_skip_whitespace(lexer);

    if (lexer->pos >= lexer->length) {
        return make_token(lexer, TOKEN_EOF, lexer->source + lexer->pos, 0);
    }

    size_t start_pos = lexer->pos;
    char c = lexer_advance(lexer);

    switch (c) {
        case '.':
            if (lexer_peek(lexer) == '.' && lexer_peek_next(lexer) == '.') {
                lexer_advance(lexer);
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_ELLIPSIS, lexer->source + start_pos, 3);
            }
            return make_token(lexer, TOKEN_DOT, lexer->source + start_pos, 1);
        case ',': return make_token(lexer, TOKEN_COMMA, lexer->source + start_pos, 1);
        case ';': return make_token(lexer, TOKEN_SEMICOLON, lexer->source + start_pos, 1);
        case ':':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_ASSIGN, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_COLON, lexer->source + start_pos, 1);
        case '(':
            return make_token(lexer, TOKEN_LPAREN, lexer->source + start_pos, 1);
        case ')':
            return make_token(lexer, TOKEN_RPAREN, lexer->source + start_pos, 1);
        case '{':
            return make_token(lexer, TOKEN_LBRACE, lexer->source + start_pos, 1);
        case '}':
            return make_token(lexer, TOKEN_RBRACE, lexer->source + start_pos, 1);
        case '[':
            return make_token(lexer, TOKEN_LBRACKET, lexer->source + start_pos, 1);
        case ']':
            return make_token(lexer, TOKEN_RBRACKET, lexer->source + start_pos, 1);
        case '?':
            return make_token(lexer, TOKEN_QUESTION, lexer->source + start_pos, 1);
        case '#':
            return make_token(lexer, TOKEN_HASH, lexer->source + start_pos, 1);
        case '&':
            if (lexer_peek(lexer) == '&') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_AND, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_AMPERSAND, lexer->source + start_pos, 1);
        case '|':
            if (lexer_peek(lexer) == '|') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_OR, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_BIT_OR, lexer->source + start_pos, 1);
        case '^':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_POW_ASSIGN, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_BIT_XOR, lexer->source + start_pos, 1);
        case '~':
            return make_token(lexer, TOKEN_BIT_NOT, lexer->source + start_pos, 1);
        case '!':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_NEQ, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_NOT, lexer->source + start_pos, 1);
        case '=':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_EQ, lexer->source + start_pos, 2);
            }
            if (lexer_peek(lexer) == '>') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_ARROW, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_ASSIGN, lexer->source + start_pos, 1);
        case '<':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_LE, lexer->source + start_pos, 2);
            }
            if (lexer_peek(lexer) == '<') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_LSHIFT, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_LT, lexer->source + start_pos, 1);
        case '>':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_GE, lexer->source + start_pos, 2);
            }
            if (lexer_peek(lexer) == '>') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_RSHIFT, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_GT, lexer->source + start_pos, 1);
        case '+':
            if (lexer_peek(lexer) == '+') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_INC, lexer->source + start_pos, 2);
            }
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_PLUS_ASSIGN, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_PLUS, lexer->source + start_pos, 1);
        case '-':
            if (lexer_peek(lexer) == '-') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_DEC, lexer->source + start_pos, 2);
            }
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_MINUS_ASSIGN, lexer->source + start_pos, 2);
            }
            if (lexer_peek(lexer) == '>') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_ARROW, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_MINUS, lexer->source + start_pos, 1);
        case '*':
            if (lexer_peek(lexer) == '*') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_POW, lexer->source + start_pos, 2);
            }
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_STAR_ASSIGN, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_STAR, lexer->source + start_pos, 1);
        case '/':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_SLASH_ASSIGN, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_SLASH, lexer->source + start_pos, 1);
        case '%':
            if (lexer_peek(lexer) == '=') {
                lexer_advance(lexer);
                return make_token(lexer, TOKEN_PERCENT_ASSIGN, lexer->source + start_pos, 2);
            }
            return make_token(lexer, TOKEN_PERCENT, lexer->source + start_pos, 1);
        case '"':
            return scan_string(lexer);
        default:
            if (isdigit(c)) {
                return scan_number(lexer);
            }
            if (isalpha(c) || c == '_') {
                return scan_ident(lexer);
            }
            return make_token(lexer, TOKEN_ERROR, lexer->source + start_pos, 1);
    }
}

void lexer_free(Lexer *lexer) {
    (void)lexer;
}