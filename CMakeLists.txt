cmake_minimum_required(VERSION 3.16)
project(superc VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(
    -Wall -Wextra -Wpedantic -Werror
    -Wno-unused-parameter
    -Wno-unused-variable
)

set(SOURCES
    compiler/lexer/lexer.c
    compiler/lexer/token.c
    compiler/lexer/keywords.c
    compiler/arena/arena.c
    compiler/ast/ast.c
    compiler/ast/ast_print.c
    compiler/parser/parser.c
    compiler/sema/sema.c
    compiler/sema/symbol_table.c
    compiler/sema/type_check.c
    compiler/sema/ownership.c
    compiler/codegen/cgen.c
    compiler/codegen/cgen_types.c
    compiler/codegen/cgen_expr.c
    compiler/codegen/cgen_stmt.c
    compiler/driver/driver.c
    compiler/driver/diagnostics.c
    compiler/runtime/runtime.c
    compiler/runtime/channel.c
    compiler/runtime/interface.c
)

add_library(superc_lib STATIC ${SOURCES})
target_include_directories(superc_lib PUBLIC compiler)

add_executable(superc compiler/main.c)
target_link_libraries(superc superc_lib)

add_executable(lexer_tests tests/lexer_test.c)
target_link_libraries(lexer_tests superc_lib)

add_executable(parser_tests tests/parser_test.c)
target_link_libraries(parser_tests superc_lib)

install(TARGETS superc DESTINATION bin)
install(FILES compiler/runtime/super_runtime.h DESTINATION include/super)