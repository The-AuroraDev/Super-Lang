#ifndef SUPER_CLI_H
#define SUPER_CLI_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CLI_INIT,
    CLI_RUN,
    CLI_BUILD,
    CLI_TIDY,
    CLI_VERSION,
    CLI_UPDATE,
    CLI_DOCS,
    CLI_ERROR,
    CLI_MOD_TIDY,
    CLI_MOD_MAP,
    CLI_FILE,
    CLI_HELP,
    CLI_UNKNOWN
} CliCommand;

typedef struct {
    CliCommand command;
    char **args;
    int argc;
    char *input_file;
    char *output_file;
    bool makefile;
    char *proxy;
    char *package;
    char *function;
    char *error_code;
    bool verbose;
    bool help;
} CliConfig;

CliConfig *cli_parse(int argc, char **argv);
void cli_free(CliConfig *config);
int cli_execute(CliConfig *config);
void cli_print_help(void);
void cli_print_version(void);

#ifdef __cplusplus
}
#endif

#endif