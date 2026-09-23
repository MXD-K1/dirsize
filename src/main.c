#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

uint8_t flags = 0x00;

#define SI_MODE     0x01
#define SHOW_HIDDEN 0x02

typedef struct dir {
    char* path;
    char** files;      /* Array of files inside the directory. */
    struct dir** dirs; /* Array of directories inside the directory. */

    // dir data
    int is_hidden;     /* Is the directory hidden? */
    int is_system;     /* Is this a system directory? */

    size_t size;       /* directory contents size in bytes. */

    // internal trackers
    size_t f_size;
    size_t d_size;
    size_t f_count;
    size_t d_count;
} Dir;

char* normalize_path(char* path);
Dir* traverse_tree(char* path);
Dir* create_dir(char* path);
void free_dir(Dir* dir);
void calc_size(Dir* root);
void print_info(Dir* root);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            printf("%s\n", argv[i]);
            if (strcmp(argv[i], "--si") == 0) {
                flags |= SI_MODE;
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

const char* units_1024[] = {"KiB", "MiB", "GiB", "TiB"};
const char* units_1000[] = {"KB", "MB", "GB", "TB"};

void print_info(Dir* root) {
    if (root == NULL) return;
    if (!(flags & SHOW_HIDDEN) && (root->is_hidden || root->is_system)) return;

    const int unit_size = flags & SI_MODE ? 1000: 1024;

    int level = 0;
    size_t size = root->size;
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
        const double size_in_unit = (double) root->size / pow(unit_size, level);
        snprintf(str_size, 20, "%.2f %s", size_in_unit, unit);
    }

    printf("%s - %s\n", root->path, str_size);
    for (int i = 0; root->dirs[i] != NULL; i++) {
        print_info(root->dirs[i]);
    }
}

#ifdef _WIN32
char PATH_SEP = '\\';
#else
char PATH_SEP = '/';
#endif

char* normalize_path(char* path) {
    int len = (int) strlen(path);
    char* new_path = malloc(len + 1);
    if (new_path == NULL) {
        exit(1);
    }


    for (int i = 0; i < len; i++) {
        if (path[i] == '\\' || path[i] == '/') {
            new_path[i] = PATH_SEP;
        } else {
            new_path[i] = path[i];
        }
    }
    new_path[len] = '\0';

    return new_path;
}

Dir* create_dir(char* path) {
    Dir* dir = malloc(sizeof(Dir));
    if (dir == NULL) {
        exit(1);
    }

    const size_t len = strlen(path);
    dir->path = malloc(len + 1);
    if (dir->path == NULL) {
        free(dir);
        exit(1);
    }
    strcpy(dir->path, path);
    dir->path[len] = '\0';
    free(path);

    dir->f_size = 64;
    dir->files = malloc(sizeof(char*) * dir->f_size);
    if (dir->files == NULL) {
        free(dir->path);
        free(dir);
        exit(1);
    }

    dir->d_size = 64;
    dir->dirs = malloc(sizeof(Dir*) * dir->d_size);
    if (dir->dirs == NULL) {
        free(dir->path);
        free(dir->files);
        free(dir);
        exit(1);
    }

    dir->size = 0;
    dir->f_count = 0;
    dir->d_count = 0;

    dir->is_hidden = 0;
    dir->is_system = 0;

    return dir;
}

void free_dir(Dir* dir) {
    if (dir == NULL) return;

    for (int i = 0; dir->files[i] != NULL; i++) {
        free(dir->files[i]);
    }

    for (int i = 0; dir->dirs[i] != NULL; i++) {
        free_dir(dir->dirs[i]);
    }

    free(dir->path);
    free(dir->dirs);
    free(dir->files);
    free(dir);
}

void append_filename(Dir* dir, char* filename) {
    if (dir->f_count >= dir->f_size) {
        char** tmp = realloc(dir->files, sizeof(char*) * dir->f_size * 2);
        if (tmp == NULL) { /* TODO: handle that */ }
        dir->files = tmp;
        dir->f_size *= 2;
    }

    dir->files[dir->f_count++] = filename;
}

void append_dir(Dir* parent, Dir* child) {
    if (parent->d_count >= parent->d_size) {
        Dir** tmp = realloc(parent->dirs, sizeof(Dir*) * parent->d_size * 2);
        if (tmp == NULL) { /* TODO: handle that */ }
        parent->dirs = tmp;
        parent->d_size *= 2;
    }

    parent->dirs[parent->d_count++] = child;
}

#ifdef _WIN32
Dir* traverse_tree(char* path) {
    Dir* root = create_dir(path);

    char search_path[4096];
    WIN32_FIND_DATA data;
    HANDLE handle = INVALID_HANDLE_VALUE;

    // Create the search pattern by appending \* to the directory path
    snprintf(search_path, 4096, "%s\\*", root->path);
    handle = FindFirstFile(search_path, &data);

    if (handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Couldn't open directory '%s'\n", root->path);
        free_dir(root);
        return NULL;
    }

    do {
        if (strcmp(data.cFileName, ".") == 0
            || strcmp(data.cFileName, "..") == 0) {
            continue;
            }

        size_t len1 = strlen(root->path);
        size_t len2 = strlen(data.cFileName);
        char* new_path = malloc(len1 + len2 + 2);
        strncpy(new_path, root->path, len1);
        new_path[len1] = PATH_SEP;
        strncpy(new_path + len1 + 1, data.cFileName, len2);
        new_path[len1 + len2 + 1] = '\0';

        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            Dir* child = traverse_tree(new_path);
            if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) child->is_hidden = 1;
            if (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) child->is_system = 1;
            append_dir(root, child);
        } else {
            append_filename(root, new_path);
        }

    } while (FindNextFile(handle, &data));

    append_filename(root, NULL);
    append_dir(root, NULL);
    FindClose(handle);
    return root;
}
#else
Dir* traverse_tree(char* path) {
    Dir* root = create_dir(path);
    DIR *dir = opendir(root->path);
    if (dir == NULL) {
        fprintf(stderr, "Couldn't open directory '%s'\n", root->path);
        free_dir(root);
        return NULL;
    }

    struct dirent *dirent;
    while ((dirent = readdir(dir)) != NULL) {
        if (strcmp(dirent->d_name, ".") == 0
            || strcmp(dirent->d_name, "..") == 0) {
            continue;
        }

        struct stat path_stat;

        size_t len1 = strlen(root->path);
        size_t len2 = strlen(dirent->d_name);
        char* new_path = malloc(len1 + len2 + 2);
        strncpy(new_path, root->path, len1);
        new_path[len1] = PATH_SEP;
        strncpy(new_path + len1 + 1, dirent->d_name, len2);
        new_path[len1 + len2 + 1] = '\0';

        if (stat(new_path, &path_stat) == 0) {
            if (S_ISDIR(path_stat.st_mode)) {
                Dir* child = traverse_tree(new_path);
                if (dirent->d_name[0] == '.') child->is_hidden = 1;
                append_dir(root, child);
            } else if (S_ISREG(path_stat.st_mode)) {
                append_filename(root, new_path);
            }
        }
    }

    append_filename(root, NULL);
    append_dir(root, NULL);
    closedir(dir);
    return root;
}
#endif

#ifdef _WIN32
void calc_size(Dir* root) {
    if (root == NULL) return;

    HANDLE handle = INVALID_HANDLE_VALUE;
    for (int i = 0; root->files[i] != NULL; i++) {
        // get a handle to that file
        handle = CreateFile(
        TEXT(root->files[i]), GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
        );

        root->size += (size_t) GetFileSize(handle, NULL);
    }
    CloseHandle(handle);

    for (int i = 0; root->dirs[i] != NULL; i++) {
        calc_size(root->dirs[i]);
        root->size += root->dirs[i]->size;
    }
}
#else
void calc_size(Dir* root) {
    if (root == NULL) return;

    struct stat st;
    for (int i = 0; root->files[i] != NULL; i++) {
        if (stat(root->files[i], &st) == 0) {
            root->size += st.st_size;
        }
    }

    for (int i = 0; root->dirs[i] != NULL; i++) {
        calc_size(root->dirs[i]);
        root->size += root->dirs[i]->size;
    }
}
#endif
