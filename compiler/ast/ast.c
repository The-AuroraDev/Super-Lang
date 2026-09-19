#include "ast.h"
#include "arena/arena.h"
#include <stdio.h>

AstNode *ast_new(Arena *arena, AstKind kind, Token token) {
    AstNode *node = arena_alloc_t(arena, AstNode);
    node->kind = kind;
    node->token = token;
    return node;
}

static const char *ast_kind_name(AstKind kind) {
    switch (kind) {
        case AST_FILE: return "File";
        case AST_PACKAGE_DECL: return "PackageDecl";
        case AST_IMPORT_DECL: return "ImportDecl";
        case AST_FN_DECL: return "FnDecl";
        case AST_STRUCT_DECL: return "StructDecl";
        case AST_INTERFACE_DECL: return "InterfaceDecl";
        case AST_TYPE_DECL: return "TypeDecl";
        case AST_VAR_DECL: return "VarDecl";
        case AST_ASSIGN_EXPR: return "AssignExpr";
        case AST_BINARY_EXPR: return "BinaryExpr";
        case AST_UNARY_EXPR: return "UnaryExpr";
        case AST_CALL_EXPR: return "CallExpr";
        case AST_INDEX_EXPR: return "IndexExpr";
        case AST_DOT_EXPR: return "DotExpr";
        case AST_IDENT_EXPR: return "IdentExpr";
        case AST_INT_LIT: return "IntLit";
        case AST_FLOAT_LIT: return "FloatLit";
        case AST_STRING_LIT: return "StringLit";
        case AST_BOOL_LIT: return "BoolLit";
        case AST_NIL_LIT: return "NilLit";
        case AST_ARRAY_LIT: return "ArrayLit";
        case AST_BLOCK_STMT: return "BlockStmt";
        case AST_IF_STMT: return "IfStmt";
        case AST_FOR_STMT: return "ForStmt";
        case AST_SWITCH_STMT: return "SwitchStmt";
        case AST_RETURN_STMT: return "ReturnStmt";
        case AST_BREAK_STMT: return "BreakStmt";
        case AST_CONTINUE_STMT: return "ContinueStmt";
        case AST_EXPR_STMT: return "ExprStmt";
    }
    return "Unknown";
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void ast_print(AstNode *node, int indent) {
    if (!node) return;
    print_indent(indent);
    printf("%s", ast_kind_name(node->kind));
    if (node->token.length > 0) {
        printf(" \"%.*s\"", (int)node->token.length, node->token.start);
    }
    printf("\n");

    switch (node->kind) {
        case AST_FILE:
            for (size_t i = 0; i < node->as.file.decl_count; i++) {
                ast_print(node->as.file.decls[i], indent + 1);
            }
            break;
        case AST_FN_DECL:
            for (size_t i = 0; i < node->as.fn_decl.param_count; i++) {
                ast_print(node->as.fn_decl.params[i], indent + 1);
            }
            if (node->as.fn_decl.return_type) ast_print(node->as.fn_decl.return_type, indent + 1);
            if (node->as.fn_decl.body) ast_print(node->as.fn_decl.body, indent + 1);
            if (node->as.fn_decl.receiver) ast_print(node->as.fn_decl.receiver, indent + 1);
            break;
        case AST_BLOCK_STMT:
            for (size_t i = 0; i < node->as.block_stmt.stmt_count; i++) {
                ast_print(node->as.block_stmt.stmts[i], indent + 1);
            }
            break;
        case AST_IF_STMT:
            ast_print(node->as.if_stmt.condition, indent + 1);
            ast_print(node->as.if_stmt.then_branch, indent + 1);
            if (node->as.if_stmt.else_branch) ast_print(node->as.if_stmt.else_branch, indent + 1);
            break;
        case AST_FOR_STMT:
            if (node->as.for_stmt.init) ast_print(node->as.for_stmt.init, indent + 1);
            if (node->as.for_stmt.condition) ast_print(node->as.for_stmt.condition, indent + 1);
            if (node->as.for_stmt.post) ast_print(node->as.for_stmt.post, indent + 1);
            ast_print(node->as.for_stmt.body, indent + 1);
            break;
        case AST_BINARY_EXPR:
        case AST_ASSIGN_EXPR:
            ast_print(node->as.binary_expr.left, indent + 1);
            ast_print(node->as.binary_expr.right, indent + 1);
            break;
        case AST_UNARY_EXPR:
            ast_print(node->as.unary_expr.operand, indent + 1);
            break;
        case AST_CALL_EXPR:
            ast_print(node->as.call_expr.callee, indent + 1);
            for (size_t i = 0; i < node->as.call_expr.arg_count; i++) {
                ast_print(node->as.call_expr.args[i], indent + 1);
            }
            break;
        case AST_ARRAY_LIT:
            for (size_t i = 0; i < node->as.array_lit.element_count; i++) {
                ast_print(node->as.array_lit.elements[i], indent + 1);
            }
            if (node->as.array_lit.size_expr) ast_print(node->as.array_lit.size_expr, indent + 1);
            break;
        default:
            break;
    }
}