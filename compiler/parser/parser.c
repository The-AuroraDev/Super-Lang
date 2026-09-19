#include "parser.h"
#include "arena/arena.h"
#include <stdbool.h>
#include <stdio.h>

Parser *parser_new(Arena *arena, Lexer *lexer) {
    Parser *parser = arena_alloc_t(arena, Parser);
    parser->lexer = lexer;
    parser->arena = arena;
    parser->had_error = false;
    parser->panic_mode = false;
    parser->current = lexer_next(lexer);
    parser->previous = (Token){0};
    return parser;
}

static void parser_advance(Parser *parser) {
    parser->previous = parser->current;
    parser->current = lexer_next(parser->lexer);
}

static bool parser_check(Parser *parser, TokenKind kind) {
    return parser->current.kind == kind;
}

static bool parser_match(Parser *parser, TokenKind kind) {
    if (parser_check(parser, kind)) {
        parser_advance(parser);
        return true;
    }
    return false;
}

static void parser_error(Parser *parser, const char *msg) {
    if (parser->panic_mode) return;
    parser->panic_mode = true;
    parser->had_error = true;
    fprintf(stderr, "[%zu:%zu] Error: %s\n",
            parser->current.line, parser->current.column, msg);
}

static void parser_synchronize(Parser *parser) {
    parser->panic_mode = false;
    while (parser->current.kind != TOKEN_EOF) {
        if (parser->previous.kind == TOKEN_SEMICOLON) return;
        switch (parser->current.kind) {
            case TOKEN_FN:
            case TOKEN_STRUCT:
            case TOKEN_INTERFACE:
            case TOKEN_TYPE:
            case TOKEN_VAR:
            case TOKEN_CONST:
            case TOKEN_IF:
            case TOKEN_FOR:
            case TOKEN_RETURN:
            case TOKEN_SWITCH:
                return;
            default:
                break;
        }
        parser_advance(parser);
    }
}

static AstNode *parse_expression(Parser *parser);
static AstNode *parse_statement(Parser *parser);
static AstNode *parse_declaration(Parser *parser);

static AstNode *parse_primary(Parser *parser) {
    if (parser_match(parser, TOKEN_INT_LIT)) {
        return ast_new(parser->arena, AST_INT_LIT, parser->previous);
    }
    if (parser_match(parser, TOKEN_FLOAT_LIT)) {
        return ast_new(parser->arena, AST_FLOAT_LIT, parser->previous);
    }
    if (parser_match(parser, TOKEN_STRING_LIT)) {
        return ast_new(parser->arena, AST_STRING_LIT, parser->previous);
    }
    if (parser_match(parser, TOKEN_TRUE)) {
        return ast_new(parser->arena, AST_BOOL_LIT, parser->previous);
    }
    if (parser_match(parser, TOKEN_FALSE)) {
        return ast_new(parser->arena, AST_BOOL_LIT, parser->previous);
    }
    if (parser_match(parser, TOKEN_NIL)) {
        return ast_new(parser->arena, AST_NIL_LIT, parser->previous);
    }
    if (parser_match(parser, TOKEN_IDENT)) {
        return ast_new(parser->arena, AST_IDENT_EXPR, parser->previous);
    }
    if (parser_match(parser, TOKEN_LPAREN)) {
        AstNode *expr = parse_expression(parser);
        if (!parser_match(parser, TOKEN_RPAREN)) {
            parser_error(parser, "Expected ')' after expression");
        }
        return expr;
    }
    parser_error(parser, "Expected expression");
    return NULL;
}

static AstNode *parse_unary(Parser *parser) {
    if (parser_match(parser, TOKEN_MINUS) || parser_match(parser, TOKEN_NOT) ||
        parser_match(parser, TOKEN_STAR) || parser_match(parser, TOKEN_AMPERSAND)) {
        Token op = parser->previous;
        AstNode *operand = parse_unary(parser);
        AstNode *node = ast_new(parser->arena, AST_UNARY_EXPR, op);
        node->as.unary_expr.op = op;
        node->as.unary_expr.operand = operand;
        return node;
    }
    return parse_primary(parser);
}

static AstNode *parse_factor(Parser *parser) {
    AstNode *expr = parse_unary(parser);
    while (parser_match(parser, TOKEN_STAR) || parser_match(parser, TOKEN_SLASH) ||
           parser_match(parser, TOKEN_PERCENT) || parser_match(parser, TOKEN_POW)) {
        Token op = parser->previous;
        AstNode *right = parse_unary(parser);
        AstNode *node = ast_new(parser->arena, AST_BINARY_EXPR, op);
        node->as.binary_expr.left = expr;
        node->as.binary_expr.op = op;
        node->as.binary_expr.right = right;
        expr = node;
    }
    return expr;
}

static AstNode *parse_term(Parser *parser) {
    AstNode *expr = parse_factor(parser);
    while (parser_match(parser, TOKEN_PLUS) || parser_match(parser, TOKEN_MINUS)) {
        Token op = parser->previous;
        AstNode *right = parse_factor(parser);
        AstNode *node = ast_new(parser->arena, AST_BINARY_EXPR, op);
        node->as.binary_expr.left = expr;
        node->as.binary_expr.op = op;
        node->as.binary_expr.right = right;
        expr = node;
    }
    return expr;
}

static AstNode *parse_comparison(Parser *parser) {
    AstNode *expr = parse_term(parser);
    while (parser_match(parser, TOKEN_EQ) || parser_match(parser, TOKEN_NEQ) ||
           parser_match(parser, TOKEN_LT) || parser_match(parser, TOKEN_LE) ||
           parser_match(parser, TOKEN_GT) || parser_match(parser, TOKEN_GE)) {
        Token op = parser->previous;
        AstNode *right = parse_term(parser);
        AstNode *node = ast_new(parser->arena, AST_BINARY_EXPR, op);
        node->as.binary_expr.left = expr;
        node->as.binary_expr.op = op;
        node->as.binary_expr.right = right;
        expr = node;
    }
    return expr;
}

static AstNode *parse_expression(Parser *parser) {
    return parse_comparison(parser);
}

static AstNode *parse_block(Parser *parser) {
    AstNode **stmts = NULL;
    size_t stmt_count = 0;
    size_t stmt_capacity = 0;

    while (!parser_check(parser, TOKEN_RBRACE) && !parser_check(parser, TOKEN_EOF)) {
        AstNode *stmt = parse_declaration(parser);
        if (stmt) {
            if (stmt_count >= stmt_capacity) {
                stmt_capacity = stmt_capacity == 0 ? 8 : stmt_capacity * 2;
                stmts = arena_alloc(parser->arena, stmt_capacity * sizeof(AstNode *), alignof(AstNode *));
            }
            stmts[stmt_count++] = stmt;
        }
    }

    if (!parser_match(parser, TOKEN_RBRACE)) {
        parser_error(parser, "Expected '}' after block");
    }

    AstNode *node = ast_new(parser->arena, AST_BLOCK_STMT, parser->previous);
    node->as.block_stmt.stmts = stmts;
    node->as.block_stmt.stmt_count = stmt_count;
    return node;
}

static AstNode *parse_var_decl(Parser *parser) {
    Token name = parser->current;
    if (!parser_match(parser, TOKEN_IDENT)) {
        parser_error(parser, "Expected variable name");
        return NULL;
    }

    AstNode *type = NULL;
    if (parser_match(parser, TOKEN_INT) || parser_match(parser, TOKEN_FLOAT) ||
        parser_match(parser, TOKEN_STRING) || parser_match(parser, TOKEN_LONGINT) ||
        parser_match(parser, TOKEN_BOOL) || parser_match(parser, TOKEN_VEC)) {
        type = ast_new(parser->arena, AST_IDENT_EXPR, parser->previous);
    }

    if (!parser_match(parser, TOKEN_ASSIGN)) {
        parser_error(parser, "Expected '=' after variable name");
        return NULL;
    }

    AstNode *init = parse_expression(parser);
    if (!parser_match(parser, TOKEN_SEMICOLON)) {
        parser_error(parser, "Expected ';' after variable declaration");
    }

    AstNode *node = ast_new(parser->arena, AST_VAR_DECL, name);
    node->as.var_decl.name = name;
    node->as.var_decl.type = type;
    node->as.var_decl.init = init;
    return node;
}

static AstNode *parse_statement(Parser *parser) {
    if (parser_match(parser, TOKEN_IF)) {
        AstNode *condition = parse_expression(parser);
        AstNode *then_branch = parse_statement(parser);
        AstNode *else_branch = NULL;
        if (parser_match(parser, TOKEN_ELSE)) {
            else_branch = parse_statement(parser);
        }
        AstNode *node = ast_new(parser->arena, AST_IF_STMT, parser->previous);
        node->as.if_stmt.condition = condition;
        node->as.if_stmt.then_branch = then_branch;
        node->as.if_stmt.else_branch = else_branch;
        return node;
    }

    if (parser_match(parser, TOKEN_FOR)) {
        AstNode *init = NULL;
        AstNode *condition = NULL;
        AstNode *post = NULL;

        if (!parser_check(parser, TOKEN_SEMICOLON)) {
            init = parse_expression(parser);
        }
        parser_match(parser, TOKEN_SEMICOLON);

        if (!parser_check(parser, TOKEN_SEMICOLON)) {
            condition = parse_expression(parser);
        }
        parser_match(parser, TOKEN_SEMICOLON);

        if (!parser_check(parser, TOKEN_LBRACE)) {
            post = parse_expression(parser);
        }

        AstNode *body = parse_statement(parser);

        AstNode *node = ast_new(parser->arena, AST_FOR_STMT, parser->previous);
        node->as.for_stmt.init = init;
        node->as.for_stmt.condition = condition;
        node->as.for_stmt.post = post;
        node->as.for_stmt.body = body;
        return node;
    }

    if (parser_match(parser, TOKEN_RETURN)) {
        AstNode *value = NULL;
        if (!parser_check(parser, TOKEN_SEMICOLON) && !parser_check(parser, TOKEN_RBRACE)) {
            value = parse_expression(parser);
        }
        parser_match(parser, TOKEN_SEMICOLON);
        AstNode *node = ast_new(parser->arena, AST_RETURN_STMT, parser->previous);
        node->as.return_stmt.value = value;
        return node;
    }

    if (parser_match(parser, TOKEN_BREAK)) {
        parser_match(parser, TOKEN_SEMICOLON);
        return ast_new(parser->arena, AST_BREAK_STMT, parser->previous);
    }

    if (parser_match(parser, TOKEN_CONTINUE)) {
        parser_match(parser, TOKEN_SEMICOLON);
        return ast_new(parser->arena, AST_CONTINUE_STMT, parser->previous);
    }

    if (parser_check(parser, TOKEN_LBRACE)) {
        parser_advance(parser);
        return parse_block(parser);
    }

    AstNode *expr = parse_expression(parser);
    if (parser_match(parser, TOKEN_ASSIGN)) {
        AstNode *right = parse_expression(parser);
        parser_match(parser, TOKEN_SEMICOLON);
        AstNode *node = ast_new(parser->arena, AST_ASSIGN_EXPR, parser->previous);
        node->as.assign_expr.left = expr;
        node->as.assign_expr.op = parser->previous;
        node->as.assign_expr.right = right;
        return node;
    }

    parser_match(parser, TOKEN_SEMICOLON);
    AstNode *node = ast_new(parser->arena, AST_EXPR_STMT, parser->previous);
    node->as.expr_stmt.expr = expr;
    return node;
}

static AstNode *parse_declaration(Parser *parser) {
    if (parser_match(parser, TOKEN_VAR) || parser_match(parser, TOKEN_CONST) || parser_match(parser, TOKEN_MUT)) {
        return parse_var_decl(parser);
    }
    return parse_statement(parser);
}

AstNode *parser_parse(Parser *parser) {
    AstNode **decls = NULL;
    size_t decl_count = 0;
    size_t decl_capacity = 0;

    while (!parser_check(parser, TOKEN_EOF)) {
        if (parser->had_error) {
            parser_synchronize(parser);
        }
        AstNode *decl = parse_declaration(parser);
        if (decl) {
            if (decl_count >= decl_capacity) {
                decl_capacity = decl_capacity == 0 ? 8 : decl_capacity * 2;
                decls = arena_alloc(parser->arena, decl_capacity * sizeof(AstNode *), alignof(AstNode *));
            }
            decls[decl_count++] = decl;
        }
    }

    AstNode *file = ast_new(parser->arena, AST_FILE, (Token){0});
    file->as.file.decls = decls;
    file->as.file.decl_count = decl_count;
    return file;
}

void parser_free(Parser *parser) {
    (void)parser;
}