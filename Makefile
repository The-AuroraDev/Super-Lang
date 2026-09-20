# Super-Lang Makefile
# Inspired by Go's build system

VERSION = 0.0.1

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
LIB_DIR = $(BUILD_DIR)/lib

# Tools (can be overridden for cross-compilation)
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror \
          -Wno-unused-parameter -Wno-unused-variable \
          -I$(SRC_DIR) \
          -DVERSION=\"$(VERSION)\"
LDFLAGS ?= -lm
AR ?= ar
ARFLAGS = rcs

# For cross-compilation, can override:
# make CC=aarch64-linux-gnu-gcc CFLAGS="-static -std=c11 ..." LDFLAGS="-static -lm"

# Targets
SUPERC = $(BIN_DIR)/superc
SUPER = $(BIN_DIR)/super
SUPERC_LIB = $(LIB_DIR)/libsuperc.a

# Source files
LEXER_SRCS = $(SRC_DIR)/super/lexer/lexer.c \
             $(SRC_DIR)/super/lexer/token.c \
             $(SRC_DIR)/super/lexer/keywords.c

PARSER_SRCS = $(SRC_DIR)/super/parser/parser.c

AST_SRCS = $(SRC_DIR)/super/ast/ast.c \
           $(SRC_DIR)/super/ast/ast_print.c

SEMA_SRCS = $(SRC_DIR)/super/sema/sema.c \
            $(SRC_DIR)/super/sema/symbol_table.c \
            $(SRC_DIR)/super/sema/type_check.c \
            $(SRC_DIR)/super/sema/ownership.c

CODEGEN_SRCS = $(SRC_DIR)/super/codegen/cgen.c \
               $(SRC_DIR)/super/codegen/cgen_types.c \
               $(SRC_DIR)/super/codegen/cgen_expr.c \
               $(SRC_DIR)/super/codegen/cgen_stmt.c

DRIVER_SRCS = $(SRC_DIR)/super/driver/driver.c \
              $(SRC_DIR)/super/driver/diagnostics.c

RUNTIME_SRCS = $(SRC_DIR)/runtime/runtime.c \
               $(SRC_DIR)/runtime/channel.c \
               $(SRC_DIR)/runtime/interface.c

ARENA_SRCS = $(SRC_DIR)/super/arena/arena.c

CLI_SRCS = $(SRC_DIR)/cmd/super/cli.c

STDLIB_SRCS = $(SRC_DIR)/fmt/fmt.c \
              $(SRC_DIR)/os/os.c \
              $(SRC_DIR)/sys/sys.c \
              $(SRC_DIR)/stk/stk.c \
              $(SRC_DIR)/math/math.c

ALL_LIB_SRCS = $(LEXER_SRCS) $(PARSER_SRCS) $(AST_SRCS) $(SEMA_SRCS) \
               $(CODEGEN_SRCS) $(DRIVER_SRCS) $(RUNTIME_SRCS) $(ARENA_SRCS) \
               $(STDLIB_SRCS) $(CLI_SRCS)

ALL_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(ALL_LIB_SRCS))

SUPERC_OBJ = $(OBJ_DIR)/cmd/superc/main.o
SUPER_OBJ = $(OBJ_DIR)/cmd/super/main.o

# Test sources
TEST_SRCS = tests/lexer_test.c tests/parser_test.c
TEST_OBJS = $(patsubst tests/%.c,$(OBJ_DIR)/tests/%.o,$(TEST_SRCS))
TEST_BINS = $(patsubst tests/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

# Default target
.PHONY: all
all: $(SUPERC) $(SUPER)

# Build static library
$(SUPERC_LIB): $(ALL_OBJS)
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Build superc compiler
$(SUPERC): $(SUPERC_OBJ) $(SUPERC_LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Build super CLI tool
$(SUPER): $(SUPER_OBJ) $(SUPERC_LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Test objects
$(OBJ_DIR)/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Test binaries
$(BIN_DIR)/%: $(OBJ_DIR)/tests/%.o $(SUPERC_LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Phony targets
.PHONY: test clean install fmt vet stdlib

# Run tests
test: $(TEST_BINS)
	@echo "Running tests..."
	@for test in $(TEST_BINS); do \
		echo "  Running $$test..."; \
		$$test || exit 1; \
	done
	@echo "All tests passed!"

# Run lexer tests only
test-lexer: $(BIN_DIR)/lexer_test
	@$(BIN_DIR)/lexer_test

# Run parser tests only
test-parser: $(BIN_DIR)/parser_test
	@$(BIN_DIR)/parser_test

# Build standard library
stdlib: $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(STDLIB_SRCS))

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Install to system
install: $(SUPERC) $(SUPER)
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(SUPERC) $(DESTDIR)/usr/local/bin/superc
	install -m 755 $(SUPER) $(DESTDIR)/usr/local/bin/super
	install -d $(DESTDIR)/usr/local/include/super
	install -m 644 $(SRC_DIR)/runtime/super_runtime.h $(DESTDIR)/usr/local/include/super/

# Format code (requires clang-format)
fmt:
	@find $(SRC_DIR) tests -name '*.c' -o -name '*.h' | xargs clang-format -i

# Static analysis (requires cppcheck)
vet:
	@cppcheck --enable=all --std=c11 -I$(SRC_DIR) $(SRC_DIR)

# Generate compile_commands.json for LSP
compile-commands:
	@bear -- make clean all

# Show help
help:
	@echo "Super-Lang Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all           - Build superc compiler (default)"
	@echo "  test          - Run all tests"
	@echo "  test-lexer    - Run lexer tests only"
	@echo "  test-parser   - Run parser tests only"
	@echo "  stdlib        - Build standard library"
	@echo "  clean         - Remove build artifacts"
	@echo "  install       - Install to /usr/local"
	@echo "  fmt           - Format code with clang-format"
	@echo "  vet           - Static analysis with cppcheck"
	@echo "  compile-commands - Generate compile_commands.json"
	@echo "  help          - Show this help"

# Cross-compilation help
cross-help:
	@echo "Cross-compilation examples:"
	@echo "  Linux ARM64:  make CC=aarch64-linux-gnu-gcc CFLAGS='-static -std=c11 -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Isrc -DVERSION=\"$(VERSION)\"' LDFLAGS='-static -lm' clean all"
	@echo "  Windows:      make CC=x86_64-w64-mingw32-gcc clean all"

# Prevent make from deleting intermediate files
.SECONDARY: $(ALL_OBJS) $(SUPERC_OBJ) $(SUPER_OBJ) $(TEST_OBJS)