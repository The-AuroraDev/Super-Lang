project('superc', 'c',
  version: '0.0.1',
  default_options: [
    'warning_level=3',
    'werror=true',
    'c_std=c11',
    'buildtype=debugoptimized',
  ])

# 编译器核心库
superc_lib = library('superc',
  sources: [
    'compiler/lexer/lexer.c',
    'compiler/lexer/token.c',
    'compiler/lexer/keywords.c',
    'compiler/arena/arena.c',
    'compiler/ast/ast.c',
    'compiler/ast/ast_print.c',
    'compiler/parser/parser.c',
    'compiler/sema/sema.c',
    'compiler/sema/symbol_table.c',
    'compiler/sema/type_check.c',
    'compiler/sema/ownership.c',
    'compiler/codegen/cgen.c',
    'compiler/codegen/cgen_types.c',
    'compiler/codegen/cgen_expr.c',
    'compiler/codegen/cgen_stmt.c',
    'compiler/driver/driver.c',
    'compiler/driver/diagnostics.c',
    'compiler/runtime/runtime.c',
    'compiler/runtime/channel.c',
    'compiler/runtime/interface.c',
  ],
  include_directories: include_directories('compiler'),
  install: false
)

# 主程序
executable('superc',
  sources: ['compiler/main.c'],
  link_with: superc_lib,
  install: true
)

# 测试
test('lexer_tests', executable('lexer_tests',
  sources: ['tests/lexer_test.c'],
  link_with: superc_lib
))

test('parser_tests', executable('parser_tests',
  sources: ['tests/parser_test.c'],
  link_with: superc_lib
))

# 安装运行时头文件
install_headers(
  'compiler/runtime/super_runtime.h',
  subdir: 'super'
)

# pkg-config
pkg = import('pkgconfig')
pkg.generate(
  name: 'superc',
  description: 'Super Language Compiler',
  version: meson.project_version(),
  libs: superc_lib,
  includes: include_directories('compiler')
)