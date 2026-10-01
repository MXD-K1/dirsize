#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli.h"
#include "dirsize.h"

Options parse_args(const int argc, char* argv[]) {
    Options opts;
    opts.max_depth = 128;
    opts.si_mode = false;
    opts.include_hidden = false;

    if (argc < 2) {
        fprintf(stderr, "Usage: ./dirsize <dir> [options]\n");
        fprintf(stderr, "For help run ./dirsize -h");
        exit(1);
    }

    opts.path = argv[1];
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            exit(0);
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("dirsize " VERSION "\n");
            exit(0);
        }

        if (strcmp(argv[i], "--si") == 0) {
            opts.si_mode = true;
        } else if (strcmp(argv[i], "-d") == 0
            || strcmp(argv[i], "--depth")) {
            if (i + 1 > argc) {
                fprintf(stderr, "Flag depth is not set to a value.\n");
                exit(1);
            }
            char *end;
            opts.max_depth = (int) strtol(argv[i + 1], &end, 10);
            if (*end != '\0') {
                fprintf(stderr, "Flag depth is not set to an integer value.\n");
                exit(1);
            }
            i++; /* skip the next arg */
        } else if (strcmp(argv[i], "--include-hidden") == 0) {
            opts.include_hidden = true;
        } else {
            fprintf(stderr, "Unrecognized option: %s\n", argv[i]);
            exit(1);
        }
    }

    return opts;
}

struct help_field {
    char* short_opt;
    char* long_opt;
    char* desc;
};

void print_help(void) {
    const struct help_field fields[] = {
        { NULL, "--si", "use powers of 1000 not 1024 for unit size." },
        { "-d", "--depth", "print the directories that don't exceed depth from the path." },
        { NULL, "--include-hidden", "print hidden directories." },
        { "-h", "--help", "print this message." },
        { "-v", "--version", "print program version." },
    };
    const int len = sizeof(fields) / sizeof(struct help_field);

    printf("Usage: ./dirsize <dir> [options]\n");
    printf("Options:\n");
    for (int i = 0; i < len; i++) {
        const struct help_field* field = &fields[i];
        printf(
            "\t%2s%2s %-18s %s\n",
            field->short_opt ? field->short_opt: "",
            field->short_opt && field->long_opt ? ", " : "",
            field->long_opt ? field->long_opt: "",
            field->desc
        );
    }
    printf("\n\nCopyright (c) 2026 Mohammed Al-Shugaa (a.k.a MXD-K1)\n");
    printf("If you found a bug or have a suggestion, email me at hmdoonwork71@gmail.com.\n");
}
