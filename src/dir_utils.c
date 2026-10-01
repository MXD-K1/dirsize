#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct dir {
    char* path;

    char** files;          /* Array of files inside the directory. */
    size_t file_count;     /* Internal tracker. */
    size_t file_capacity;  /* Internal tracker. */

    struct dir** dirs;     /* Array of directories inside the directory. */
    size_t dir_count;      /* Internal tracker. */
    size_t dir_capacity;   /* Internal tracker. */


    /* Directory attributes: */
    bool is_hidden;        /* Is the directory hidden? */
    bool is_system;        /* Is this a system directory? */

    size_t size;           /* directory contents size in bytes. */
};

#include "fs.h"
#include "path.h"
#include "dir_utils.h"

Dir* create_dir(const char* path) {
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

bool append_file(Dir* dir, char* filename) {
    if (dir->file_count >= dir->file_capacity) {
        char** tmp = realloc(dir->files, sizeof(char*) * dir->file_capacity * 2);
        if (tmp == NULL) {
            return false;
        }
        dir->files = tmp;
        dir->file_capacity *= 2;
    }

    dir->files[dir->file_count++] = filename;
    return true;
}

bool append_dir(Dir* parent, Dir* child) {
    if (parent->dir_count >= parent->dir_capacity) {
        Dir** tmp = realloc(parent->dirs, sizeof(Dir*) * parent->dir_capacity * 2);
        if (tmp == NULL) {
            return false;
        }
        parent->dirs = tmp;
        parent->dir_capacity *= 2;
    }

    parent->dirs[parent->dir_count++] = child;
    return true;
}

/* It expects the path to be dynamically allocated. */
Dir* traverse_tree(char* path) {
    Dir* root = create_dir(path);

    FS_Dir* fs_dir;
    if (!fs_open_dir(root->path, &fs_dir)) {
        fprintf(stderr, "Couldn't open directory '%s'\n", root->path);
        fs_close_dir(fs_dir);
        free_dir(root);
        return NULL;
    }

    bool at_end;
    do {
        const char* name = fs_get_entry_name(fs_dir);
        if (strcmp(name, ".") == 0
            || strcmp(name, "..") == 0) {
            fs_next_entry(fs_dir, &at_end);
            continue;
            }

        char* new_path = join_path(root->path, name);

        bool is_dir;
        if (!fs_entry_is_dir(new_path, &is_dir)) {
            fprintf(stderr, "Some error happened.\n");
            fs_close_dir(fs_dir);
            free_dir(root);
            free(new_path);
            return NULL;
        }

        if (is_dir) {
            Dir* child = traverse_tree(new_path);
            if (child == NULL) {
                fs_close_dir(fs_dir);
                free_dir(root);
                return NULL;
            }

            fs_entry_is_hidden(name, &child->is_hidden);
            fs_entry_is_system_dir(name, &child->is_system);
            if (!append_dir(root, child)) {
                fprintf(stderr, "Not enough storage.\n");
                fs_close_dir(fs_dir);
                free_dir(root);
                return NULL;
            }
        } else {
            if (!append_file(root, new_path)) {
                fprintf(stderr, "Not enough storage.\n");
                fs_close_dir(fs_dir);
                free_dir(root);
                return NULL;
            }
        }

        fs_next_entry(fs_dir, &at_end);
    } while (!at_end);

    fs_close_dir(fs_dir);
    free(path);

    return root;
}

void calc_size(Dir* root) {
    if (root == NULL) return;

    size_t size;
    for (int i = 0; i < root->file_count; i++) {
        if (fs_get_file_size(root->files[i], &size))  root->size += size;
        else fprintf(stderr, "Couldn't inspect '%s' size.\n", root->files[i]);
    }

    for (int i = 0; i < root->dir_count; i++) {
        calc_size(root->dirs[i]);
        root->size += root->dirs[i]->size;
    }
}

// ------------------------------------
size_t get_dir_size(const Dir* dir) {
    return dir->size;
}

char* get_dir_path(const Dir* dir) {
    return dir->path;
}

size_t get_dir_file_count(const Dir* dir) {
    return dir->file_count;
}

size_t get_dir_dir_count(const Dir* dir) {
    return dir->dir_count;
}

char** get_dir_files(const Dir* dir) {
    return dir->files;
}

Dir** get_dir_dirs(const Dir* dir) {
    return dir->dirs;
}

bool is_dir_hidden(const Dir* dir) {
    return dir->is_hidden || dir->is_system;
}
