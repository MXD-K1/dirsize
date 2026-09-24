#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "fs.h"
#include "path.h"
#include "dir_utils.h"

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

    dir->file_capacity = 64;
    dir->files = malloc(sizeof(char*) * dir->file_capacity);
    if (dir->files == NULL) {
        free(dir->path);
        free(dir);
        exit(1);
    }

    dir->dir_capacity = 64;
    dir->dirs = malloc(sizeof(Dir*) * dir->dir_capacity);
    if (dir->dirs == NULL) {
        free(dir->path);
        free(dir->files);
        free(dir);
        exit(1);
    }

    dir->size = 0;
    dir->file_count = 0;
    dir->dir_count = 0;

    dir->is_hidden = false;
    dir->is_system = false;

    return dir;
}

void free_dir(Dir* dir) {
    if (dir == NULL) return;

    for (int i = 0; i < dir->file_count; i++) {
        free(dir->files[i]);
    }

    for (int i = 0; i < dir->dir_count; i++) {
        free_dir(dir->dirs[i]);
    }

    free(dir->path);
    free(dir->dirs);
    free(dir->files);
    free(dir);
}

void append_file(Dir* dir, char* filename) {
    if (dir->file_count >= dir->file_capacity) {
        char** tmp = realloc(dir->files, sizeof(char*) * dir->file_capacity * 2);
        if (tmp == NULL) { /* TODO: handle that */ }
        dir->files = tmp;
        dir->file_capacity *= 2;
    }

    dir->files[dir->file_count++] = filename;
}

void append_dir(Dir* parent, Dir* child) {
    if (parent->dir_count >= parent->dir_capacity) {
        Dir** tmp = realloc(parent->dirs, sizeof(Dir*) * parent->dir_capacity * 2);
        if (tmp == NULL) { /* TODO: handle that */ }
        parent->dirs = tmp;
        parent->dir_capacity *= 2;
    }

    parent->dirs[parent->dir_count++] = child;
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
        exit(1);
    }

    do {
        if (strcmp(data.cFileName, ".") == 0
            || strcmp(data.cFileName, "..") == 0) {
            continue;
            }

        char* new_path = join_path(root->path, data.cFileName);
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            Dir* child = traverse_tree(new_path);
            if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) child->is_hidden = true;
            if (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) child->is_system = true;
            append_dir(root, child);
        } else {
            append_file(root, new_path);
        }

    } while (FindNextFile(handle, &data));

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
        exit(1);
    }

    struct dirent *dirent;
    while ((dirent = readdir(dir)) != NULL) {
        if (strcmp(dirent->d_name, ".") == 0
            || strcmp(dirent->d_name, "..") == 0) {
            continue;
        }

        struct stat path_stat;
        char* new_path = join_path(root->path, dirent->d_name);
        if (stat(new_path, &path_stat) == 0) {
            if (S_ISDIR(path_stat.st_mode)) {
                Dir* child = traverse_tree(new_path);
                if (dirent->d_name[0] == '.') child->is_hidden = true;
                append_dir(root, child);
            } else if (S_ISREG(path_stat.st_mode)) {
                append_file(root, new_path);
            }
        }
    }

    closedir(dir);
    return root;
}
#endif

void calc_size(Dir* root) {
    if (root == NULL) return;

    size_t size;
    for (int i = 0; i < root->file_count; i++) {
        fs_get_file_size(root->files[i], &size);
        root->size += size;
    }

    for (int i = 0; i < root->dir_count; i++) {
        calc_size(root->dirs[i]);
        root->size += root->dirs[i]->size;
    }
}

const char* units_1024[] = {"KiB", "MiB", "GiB", "TiB"};
const char* units_1000[] = {"KB", "MB", "GB", "TB"};

void print_info(Dir* root, const uint8_t flags, const int max_depth) {
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

    printf("%-35s - %10s\n", root->path, str_size);
    for (int i = 0; i < root->dir_count; i++) {
        if (max_depth > 0) {
            print_info(root->dirs[i], flags, max_depth - 1);
        }
    }
}
