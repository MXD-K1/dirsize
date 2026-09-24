#include <stdlib.h>
#include <string.h>

#include "path.h"

#ifdef _WIN32
char PATH_SEP = '\\';
#else
char PATH_SEP = '/';
#endif

char* normalize_path(const char* path) {
    const int len = strlen(path);
    char* new_path = malloc(len + 1);
    if (new_path == NULL) exit(1);

    for (int i = 0; i < len; i++) {
        if (path[i] == '\\' || path[i] == '/') {
            new_path[i] = PATH_SEP;
            continue;
        }
        new_path[i] = path[i];
    }
    new_path[len] = '\0';

    return new_path;
}

char* join_path(const char* path_1, const char* path_2) {
    const size_t len_1 = strlen(path_1);
    const size_t len_2 = strlen(path_2);
    char* joined_path = malloc(len_1 + len_2 + 2);

    strncpy(joined_path, path_1, len_1);
    joined_path[len_1] = PATH_SEP;
    strncpy(joined_path + len_1 + 1, path_2, len_2);
    joined_path[len_1 + len_2 + 1] = '\0';

    return joined_path;
}
