#ifndef DIRSIZE_CLI_H
#define DIRSIZE_CLI_H

#include <stdbool.h>

typedef struct {
    char* path;
    int max_depth;
    bool si_mode;
    bool include_hidden;
} Options;

Options parse_args(int argc, char* argv[]);
void print_help(void);

#endif //DIRSIZE_CLI_H
