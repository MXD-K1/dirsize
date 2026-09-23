#ifndef DIRSIZE_DIR_UTILS_H
#define DIRSIZE_DIR_UTILS_H

#include <stdint.h>
#include <stdbool.h>

/* Temp */
#define SI_MODE     0x01
#define SHOW_HIDDEN 0x02

extern uint8_t flags;

typedef struct dir {
    char* path;
    char** files;      /* Array of files inside the directory. */
    struct dir** dirs; /* Array of directories inside the directory. */

    // dir attributes
    bool is_hidden;     /* Is the directory hidden? */
    bool is_system;     /* Is this a system directory? */

    size_t size;       /* directory contents size in bytes. */

    // internal trackers
    size_t f_size;
    size_t d_size;
    size_t f_count;
    size_t d_count;
} Dir;

char* normalize_path(char* path);
Dir* create_dir(char* path);
void free_dir(Dir* dir);
Dir* traverse_tree(char* path);
void calc_size(Dir* root);
void print_info(Dir* root);

#endif //DIRSIZE_DIR_UTILS_H
