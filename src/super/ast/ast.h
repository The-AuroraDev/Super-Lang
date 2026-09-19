#ifndef SUPER_AST_H
#define SUPER_AST_H

#include <stdbool.h>
#include "super/arena/arena.h"
#include "super/lexer/token.h"

typedef enum {
    AST_FILE,
    AST_PACKAGE_DECL,
    AST_IMPORT_DECL,
    AST_FN_DECL,
    AST_STRUCT_DECL,
    AST_INTERFACE_DECL,
    AST_TYPE_DECL,
    AST_VAR_DECL,
    AST_ASSIGN_EXPR,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_CALL_EXPR,
    AST_INDEX_EXPR,
    AST_DOT_EXPR,
    AST_IDENT_EXPR,
    AST_INT_LIT,
    AST_FLOAT_LIT,
    AST_STRING_LIT,
    AST_BOOL_LIT,
    AST_NIL_LIT,
    AST_ARRAY_LIT,
    AST_BLOCK_STMT,
    AST_IF_STMT,
    AST_FOR_STMT,
    AST_SWITCH_STMT,
    AST_RETURN_STMT,
    AST_BREAK_STMT,
    AST_CONTINUE_STMT,
    AST_EXPR_STMT,
} AstKind;

typedef struct AstNode AstNode;

struct AstNode {
    AstKind kind;
    Token token;
    union {
        struct {
            AstNode **decls;
            size_t decl_count;
        } file;
        struct {
            Token name;
        } package_decl;
        struct {
            Token path;
        } import_decl;
        struct {
            Token name;
            AstNode **params;
            size_t param_count;
            AstNode *return_type;
            AstNode *body;
            bool is_pub;
            bool is_method;
            AstNode *receiver;
        } fn_decl;
        struct {
            Token name;
            AstNode **fields;
            size_t field_count;
        } struct_decl;
        struct {
            Token name;
            AstNode **methods;
            size_t method_count;
        } interface_decl;
        struct {
            Token name;
            AstNode *underlying;
        } type_decl;
        struct {
            Token name;
            AstNode *type;
            AstNode *init;
            bool is_mut;
        } var_decl;
        struct {
            AstNode *left;
            Token op;
            AstNode *right;
        } assign_expr;
        struct {
            AstNode *left;
            Token op;
            AstNode *right;
        } binary_expr;
        struct {
            Token op;
            AstNode *operand;
        } unary_expr;
        struct {
            AstNode *callee;
            AstNode **args;
            size_t arg_count;
        } call_expr;
        struct {
            AstNode *object;
            AstNode *index;
        } index_expr;
        struct {
            AstNode *object;
            Token field;
        } dot_expr;
        struct {
            Token name;
        } ident_expr;
        struct {
            Token value;
        } int_lit;
        struct {
            Token value;
        } float_lit;
        struct {
            Token value;
        } string_lit;
        struct {
            Token value;
        } bool_lit;
        struct {
            AstNode **elements;
            size_t element_count;
            AstNode *size_expr;
        } array_lit;
        struct {
            AstNode **stmts;
            size_t stmt_count;
        } block_stmt;
        struct {
            AstNode *condition;
            AstNode *then_branch;
            AstNode *else_branch;
        } if_stmt;
        struct {
            AstNode *init;
            AstNode *condition;
            AstNode *post;
            AstNode *body;
        } for_stmt;
        struct {
            AstNode *expr;
            AstNode **cases;
            size_t case_count;
            AstNode *default_case;
        } switch_stmt;
        struct {
            AstNode *value;
        } return_stmt;
        struct {
            AstNode *expr;
        } expr_stmt;
    } as;
};

AstNode *ast_new(Arena *arena, AstKind kind, Token token);
void ast_print(AstNode *node, int indent);

#endif