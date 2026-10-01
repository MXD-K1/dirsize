#include <string.h>

#include "dir_utils.h"
#include "output.h"
#include "path.h"
#include "cli.h"

int main(int argc, char *argv[]) {
    const Options options = parse_args(argc, argv);
    char* path = normalize_path(options.path);

    Dir* root = traverse_tree(path);
    if (root == NULL) {
        return 1;
    }

    calc_size(root);
    print_info(root, options);

    free_dir(root);
    return 0;
}
