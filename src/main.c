#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dir_utils.h"

uint8_t flags = 0x00;
int max_depth = 128;

int main(int argc, char *argv[]) {
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--si") == 0) {
                flags |= SI_MODE;
            } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--depth") == 0) {
                if (i + 1 > argc) {
                    fprintf(stderr, "Flag depth is not set to a value.\n");
                    return 1;
                }
                char *end;
                max_depth = (int) strtol(argv[i + 1], &end, 10);
                if (*end != '\0') {
                    fprintf(stderr, "Flag depth is not set to an integer value.\n");
                    return 1;
                }
                i++; /* skip the next arg */
            } else if (strcmp(argv[i], "--include-hidden") == 0) {
                flags |= SHOW_HIDDEN;
            } else {
                fprintf(stderr, "Unrecognized option: %s\n", argv[i]);
                return 1;
            }
        }
    }
    else if (argc != 2) {
        fprintf(stderr, "Usage: ./dirsize <dir> [--si]\n");
        return 1;
    }

    char* path = argv[1];
    path = normalize_path(path);

    Dir* root = traverse_tree(path);
    calc_size(root);
    print_info(root);

    free_dir(root);

    return 0;
}
