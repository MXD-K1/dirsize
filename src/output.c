#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "output.h"

const char* units_1024[] = {"KiB", "MiB", "GiB", "TiB"};
const char* units_1000[] = {"KB", "MB", "GB", "TB"};

void print_info(Dir* root, const uint8_t flags, const int max_depth) {
    if (root == NULL) return;
    if (!(flags & SHOW_HIDDEN) && is_dir_hidden(root)) return;

    const int unit_size = flags & SI_MODE ? 1000: 1024;

    int level = 0;
    size_t size = get_dir_size(root);
    while (size >= unit_size) {
        size = size / unit_size;
        level++;
    }

    if (level > 4) {
        fprintf(stderr, "Add more units...\n");
        free_dir(root);
        exit(2);
    }

    char str_size[20];
    if (level == 0) {
        const int size_in_unit = (int) size;
        snprintf(str_size, 20, "%d B", size_in_unit);
    } else {
        const char* unit = flags & SI_MODE ? units_1000[level - 1] : units_1024[level - 1];
        const double size_in_unit = (double) get_dir_size(root) / pow(unit_size, level);
        snprintf(str_size, 20, "%.2f %s", size_in_unit, unit);
    }

    printf("%-35s - %10s\n", get_dir_path(root), str_size);
    for (int i = 0; i < get_dir_dir_count(root); i++) {
        if (max_depth > 0) {
            print_info(get_dir_dirs(root)[i], flags, max_depth - 1);
        }
    }
}
