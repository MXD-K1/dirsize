#include <stdio.h>
#include <string.h>

#include "dir_utils.h"
#include "output.h"
#include "path.h"
#include "cli.h"

int main(int argc, char *argv[]) {
    Options options = parse_args(argc, argv);

    char* path = argv[1];
    path = normalize_path(path);

    Dir* root = traverse_tree(path);
    if (root == NULL) {
        return 1;
    }

    calc_size(root);
    print_info(root, options);

    free_dir(root);

    return 0;
}
