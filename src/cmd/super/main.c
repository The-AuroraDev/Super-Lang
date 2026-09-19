#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifndef VERSION
#define VERSION "0.0.1"
#endif

int main(int argc, char **argv) {
    setlocale(LC_ALL, "");
    
    CliConfig *config = cli_parse(argc, argv);
    int ret = cli_execute(config);
    cli_free(config);
    return ret;
}