#include "super/lexer/lexer.h"
#include "super/arena/arena.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static void test_lex(const char *source, TokenKind *expected, size_t expected_count) {
    Arena *arena = arena_new(0);
    Lexer *lexer = lexer_new(arena, source, strlen(source));

    for (size_t i = 0; i < expected_count; i++) {
        Token token = lexer_next(lexer);
        assert(token.kind == expected[i]);
    }

    Token eof = lexer_next(lexer);
    assert(eof.kind == TOKEN_EOF);

    arena_free(arena);
}

int main(void) {
    TokenKind test1[] = {TOKEN_INT, TOKEN_IDENT, TOKEN_ASSIGN, TOKEN_INT_LIT, TOKEN_SEMICOLON};
    test_lex("int a = 1;", test1, 5);

    TokenKind test2[] = {TOKEN_FN, TOKEN_IDENT, TOKEN_LPAREN, TOKEN_INT, TOKEN_IDENT, TOKEN_COMMA,
                         TOKEN_IDENT, TOKEN_RPAREN, TOKEN_ARROW, TOKEN_INT, TOKEN_LBRACE, TOKEN_RBRACE};
    test_lex("fn a(int a, b) -> int {}", test2, 12);

    TokenKind test3[] = {TOKEN_PUB, TOKEN_FN, TOKEN_IDENT, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_ARROW,
                         TOKEN_NIL, TOKEN_LBRACE, TOKEN_OUT, TOKEN_LPAREN, TOKEN_STRING_LIT, TOKEN_RPAREN,
                         TOKEN_RBRACE};
    test_lex("pub fn b() -> nil { out(\"H\") }", test3, 13);

    TokenKind test4[] = {TOKEN_IDENT, TOKEN_ASSIGN, TOKEN_LBRACE, TOKEN_STRING_LIT, TOKEN_COMMA,
                         TOKEN_STRING_LIT, TOKEN_COMMA, TOKEN_STRING_LIT, TOKEN_RBRACE,
                         TOKEN_LBRACKET, TOKEN_INT_LIT, TOKEN_RBRACKET, TOKEN_SEMICOLON};
    test_lex("arr = {\"1\", \"2\", \"3\"}[3];", test4, 13);

    TokenKind test5[] = {TOKEN_FOR, TOKEN_IDENT, TOKEN_ASSIGN, TOKEN_INT_LIT, TOKEN_SEMICOLON,
                         TOKEN_IDENT, TOKEN_LE, TOKEN_INT_LIT, TOKEN_SEMICOLON, TOKEN_IDENT, TOKEN_INC,
                         TOKEN_LBRACE, TOKEN_RBRACE};
    test_lex("for i=0; i<=10; i++ {}", test5, 14);

    TokenKind test6[] = {TOKEN_STAR, TOKEN_IDENT, TOKEN_ASSIGN, TOKEN_AMPERSAND, TOKEN_IDENT, TOKEN_SEMICOLON,
                         TOKEN_STAR, TOKEN_IDENT, TOKEN_ASSIGN, TOKEN_INT_LIT, TOKEN_SEMICOLON};
    test_lex("*a = &b; *a = 10;", test6, 11);

    TokenKind test7[] = {TOKEN_IF, TOKEN_TRUE, TOKEN_LBRACE, TOKEN_RBRACE};
    test_lex("if True {}", test7, 4);

    TokenKind test8[] = {TOKEN_IDENT, TOKEN_ASSIGN, TOKEN_IN, TOKEN_SEMICOLON};
    test_lex("a = in;", test8, 4);

    TokenKind test9[] = {TOKEN_SWITCH, TOKEN_INT_LIT, TOKEN_LBRACE, TOKEN_STRING_LIT, TOKEN_COLON,
                         TOKEN_OUT, TOKEN_LPAREN, TOKEN_STRING_LIT, TOKEN_RPAREN, TOKEN_RBRACE};
    test_lex("switch 4 { \"1\": out(\"1\") }", test9, 10);

    TokenKind test10[] = {TOKEN_STRUCT, TOKEN_IDENT, TOKEN_LBRACE, TOKEN_IDENT, TOKEN_INT, TOKEN_RBRACE};
    test_lex("struct a { a int }", test10, 6);

    TokenKind test11[] = {TOKEN_INTERFACE, TOKEN_IDENT, TOKEN_LBRACE, TOKEN_IDENT, TOKEN_LPAREN, TOKEN_RPAREN,
                          TOKEN_IDENT, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_RBRACE};
    test_lex("interface example { a() b() }", test11, 10);

    TokenKind test12[] = {TOKEN_INCLUDE, TOKEN_IDENT};
    test_lex("include fmt", test12, 2);

    TokenKind test13[] = {TOKEN_HASH, TOKEN_MAIN};
    test_lex("#main", test13, 2);

    TokenKind test14[] = {TOKEN_HASH, TOKEN_GC, TOKEN_ASSIGN, TOKEN_TRUE};
    test_lex("#GC = True", test14, 4);

    TokenKind test15[] = {TOKEN_IDENT, TOKEN_POW, TOKEN_IDENT};
    test_lex("a ** b", test15, 3);

    printf("All lexer tests passed!\n");
    return 0;
}