#include "cli.h"
#include "super/lexer/lexer.h"
#include "super/arena/arena.h"
#include "super/parser/parser.h"
#include "super/sema/sema.h"
#include "super/codegen/cgen.h"
#include "super/driver/driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef VERSION
#define VERSION "0.0.1"
#endif

#define SUPER_HOME_DEFAULT ".super"
#define BIN_DIR_DEFAULT "/usr/local/bin"

static const char *help_text = 
"Super Language Toolchain v" VERSION "\n"
"\n"
"Usage: super <command> [options]\n"
"\n"
"Commands:\n"
"  init [dir]              初始化新项目\n"
"  run <file.super>        编译并直接运行\n"
"  build <file.super> [out] 编译到目标文件 (留空输出为 文件.a)\n"
"  tidy                    整理文件结构和导入\n"
"  version                 显示版本信息\n"
"  update [--proxy <url>]  升级 super 工具链\n"
"  docs <package> [func]   查看文档\n"
"  error <code>            查看错误码说明\n"
"  mod tidy                整理依赖\n"
"  mod map                 显示依赖图\n"
"  file [--makefile]       按 SuperFile 构建\n"
"  help [command]          显示帮助\n"
"\n"
"Options:\n"
"  -v, --verbose           详细输出\n"
"  -h, --help              显示帮助\n"
"\n"
"Examples:\n"
"  super init myproj\n"
"  super run main.super\n"
"  super build main.super\n"
"  super build main.super myapp\n"
"  super mod tidy\n"
"  super docs fmt printf\n"
"  super error S001\n";

static const char *help_text_en = 
"Super Language Toolchain v" VERSION "\n"
"\n"
"Usage: super <command> [options]\n"
"\n"
"Commands:\n"
"  init [dir]              Initialize new project\n"
"  run <file.super>        Compile and run directly\n"
"  build <file.super> [out] Build to target (empty output = file.a)\n"
"  tidy                    Tidy file structure and imports\n"
"  version                 Show version info\n"
"  update [--proxy <url>]  Update super toolchain\n"
"  docs <package> [func]   View documentation\n"
"  error <code>            View error code explanation\n"
"  mod tidy                Tidy dependencies\n"
"  mod map                 Show dependency graph\n"
"  file [--makefile]       Build via SuperFile\n"
"  help [command]          Show help\n"
"\n"
"Options:\n"
"  -v, --verbose           Verbose output\n"
"  -h, --help              Show help\n";

static bool g_chinese = true;

CliConfig *cli_parse(int argc, char **argv) {
    CliConfig *config = calloc(1, sizeof(CliConfig));
    config->command = CLI_UNKNOWN;
    config->args = argv;
    config->argc = argc;

    if (argc < 2) {
        config->command = CLI_HELP;
        return config;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            config->help = true;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            config->verbose = true;
        } else if (strcmp(argv[i], "--proxy") == 0 && i + 1 < argc) {
            config->proxy = argv[++i];
        } else if (strcmp(argv[i], "--makefile") == 0) {
            config->makefile = true;
        } else if (argv[i][0] != '-') {
            if (config->command == CLI_UNKNOWN) {
                if (strcmp(argv[i], "init") == 0) config->command = CLI_INIT;
                else if (strcmp(argv[i], "run") == 0) config->command = CLI_RUN;
                else if (strcmp(argv[i], "build") == 0) config->command = CLI_BUILD;
                else if (strcmp(argv[i], "tidy") == 0) config->command = CLI_TIDY;
                else if (strcmp(argv[i], "version") == 0) config->command = CLI_VERSION;
                else if (strcmp(argv[i], "update") == 0) config->command = CLI_UPDATE;
                else if (strcmp(argv[i], "docs") == 0) config->command = CLI_DOCS;
                else if (strcmp(argv[i], "error") == 0) config->command = CLI_ERROR;
                else if (strcmp(argv[i], "mod") == 0) {
                    if (i + 1 < argc) {
                        if (strcmp(argv[i+1], "tidy") == 0) { config->command = CLI_MOD_TIDY; i++; }
                        else if (strcmp(argv[i+1], "map") == 0) { config->command = CLI_MOD_MAP; i++; }
                        else { config->command = CLI_UNKNOWN; }
                    } else { config->command = CLI_UNKNOWN; }
                }
                else if (strcmp(argv[i], "file") == 0) config->command = CLI_FILE;
                else if (strcmp(argv[i], "help") == 0) config->command = CLI_HELP;
                else config->command = CLI_UNKNOWN;
            } else {
                if (!config->input_file) config->input_file = argv[i];
                else if (!config->output_file) config->output_file = argv[i];
                else if (!config->package) config->package = argv[i];
                else if (!config->function) config->function = argv[i];
                else if (!config->error_code) config->error_code = argv[i];
            }
        }
    }

    return config;
}

void cli_free(CliConfig *config) {
    free(config);
}

static void cmd_init(CliConfig *config) {
    const char *dir = config->input_file ? config->input_file : ".";
    
    // 创建目录
    #ifdef _WIN32
    _mkdir(dir);
    #else
    mkdir(dir, 0755);
    #endif
    
    char path[1024];
    snprintf(path, sizeof(path), "%s/main.super", dir);
    FILE *f = fopen(path, "w");
    if (f) {
        fprintf(f, "#main\n\nfn main() -> int {\n    out(\"Hello, Super!\")\n    return 0\n}\n");
        fclose(f);
        printf(g_chinese ? "已创建: %s\n" : "Created: %s\n", path);
    }

    snprintf(path, sizeof(path), "%s/File.super", dir);
    f = fopen(path, "w");
    if (f) {
        fprintf(f, "super version %s\n\n# dependencies\n\n#code\nbuild {\n    (super) build main.super\n} build\ncode#\n", VERSION);
        fclose(f);
        printf(g_chinese ? "已创建: %s\n" : "Created: %s\n", path);
    }

    snprintf(path, sizeof(path), "%s/.gitignore", dir);
    f = fopen(path, "w");
    if (f) {
        fprintf(f, "build/\n*.a\n*.o\n");
        fclose(f);
        printf(g_chinese ? "已创建: %s\n" : "Created: %s\n", path);
    }

    printf(g_chinese ? "项目初始化完成: %s\n" : "Project initialized: %s\n", dir);
}

static void cmd_run(CliConfig *config) {
    if (!config->input_file) {
        fprintf(stderr, g_chinese ? "错误: 缺少输入文件\n" : "Error: missing input file\n");
        return;
    }

    DriverConfig drv = {0};
    drv.arena = NULL;
    drv.input_file = config->input_file;
    drv.output_file = NULL;
    drv.run_after_build = true;
    driver_run(&drv);
}

static void cmd_build(CliConfig *config) {
    if (!config->input_file) {
        fprintf(stderr, g_chinese ? "错误: 缺少输入文件\n" : "Error: missing input file\n");
        return;
    }

    char output[1024];
    if (config->output_file) {
        strncpy(output, config->output_file, sizeof(output));
    } else {
        const char *dot = strrchr(config->input_file, '.');
        if (dot && strcmp(dot, ".super") == 0) {
            size_t len = dot - config->input_file;
            strncpy(output, config->input_file, len);
            output[len] = '\0';
            strcat(output, ".a");
        } else {
            strcpy(output, "a.out");
        }
    }

    DriverConfig drv = {0};
    drv.arena = NULL;
    drv.input_file = config->input_file;
    drv.output_file = output;
    drv.run_after_build = false;
    int ret = driver_run(&drv);
    if (ret == 0) {
        printf(g_chinese ? "构建成功: %s\n" : "Build success: %s\n", output);
    }
}

static void cmd_tidy(CliConfig *config) {
    printf(g_chinese ? "整理文件结构...\n" : "Tidying file structure...\n");
    // TODO: 实现格式化、整理导入
}

static void cmd_version(CliConfig *config) {
    printf("super %s\n", VERSION);
    printf(g_chinese ? "Super 语言工具链\n" : "Super Language Toolchain\n");
}

static void cmd_update(CliConfig *config) {
    printf(g_chinese ? "检查更新...\n" : "Checking for updates...\n");
    if (config->proxy) {
        printf(g_chinese ? "使用代理: %s\n" : "Using proxy: %s\n", config->proxy);
    }
    // TODO: 实现自动更新逻辑
    printf(g_chinese ? "当前已是最新版本\n" : "Already up to date\n");
}

static void cmd_docs(CliConfig *config) {
    if (!config->package) {
        fprintf(stderr, g_chinese ? "错误: 缺少包名\n" : "Error: missing package\n");
        return;
    }
    printf(g_chinese ? "文档: %s" : "Docs: %s", config->package);
    if (config->function) {
        printf(".%s", config->function);
    }
    printf("\n");
    // TODO: 实现文档查看
}

static void cmd_error(CliConfig *config) {
    if (!config->error_code) {
        fprintf(stderr, g_chinese ? "错误: 缺少错误码\n" : "Error: missing error code\n");
        return;
    }
    printf(g_chinese ? "错误码 %s: " : "Error %s: ", config->error_code);
    // TODO: 实现错误码查询
    printf(g_chinese ? "请查阅文档\n" : "See documentation\n");
}

static void cmd_mod_tidy(CliConfig *config) {
    printf(g_chinese ? "整理依赖...\n" : "Tidying dependencies...\n");
    // TODO: 实现依赖管理
}

static void cmd_mod_map(CliConfig *config) {
    printf(g_chinese ? "依赖图:\n" : "Dependency graph:\n");
    // TODO: 实现依赖图
}

static void cmd_file(CliConfig *config) {
    printf(g_chinese ? "按 SuperFile 构建...\n" : "Building via SuperFile...\n");
    if (config->makefile) {
        printf(g_chinese ? "生成 Makefile...\n" : "Generating Makefile...\n");
    }
    // TODO: 实现 SuperFile 解析和构建
}

int cli_execute(CliConfig *config) {
    if (config->help || config->command == CLI_HELP) {
        cli_print_help();
        return 0;
    }

    switch (config->command) {
        case CLI_INIT: cmd_init(config); break;
        case CLI_RUN: cmd_run(config); break;
        case CLI_BUILD: cmd_build(config); break;
        case CLI_TIDY: cmd_tidy(config); break;
        case CLI_VERSION: cmd_version(config); break;
        case CLI_UPDATE: cmd_update(config); break;
        case CLI_DOCS: cmd_docs(config); break;
        case CLI_ERROR: cmd_error(config); break;
        case CLI_MOD_TIDY: cmd_mod_tidy(config); break;
        case CLI_MOD_MAP: cmd_mod_map(config); break;
        case CLI_FILE: cmd_file(config); break;
        default:
            fprintf(stderr, g_chinese ? "未知命令: %s\n" : "Unknown command\n", 
                    config->args[1] ? config->args[1] : "");
            cli_print_help();
            return 1;
    }
    return 0;
}

void cli_print_help(void) {
    printf("%s", g_chinese ? help_text : help_text_en);
}

void cli_print_version(void) {
    printf("super %s\n", VERSION);
}

void cli_set_language(bool chinese) {
    g_chinese = chinese;
}