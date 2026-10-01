#ifndef DIRSIZE_DIR_UTILS_H
#define DIRSIZE_DIR_UTILS_H

#include <stdint.h>

/* Temp */
#define SI_MODE     0x01
#define SHOW_HIDDEN 0x02

typedef struct dir Dir;

Dir* create_dir(const char* path);
void free_dir(Dir* dir);
Dir* traverse_tree(char* path);
void calc_size(Dir* root);
void print_info(Dir* root, uint8_t flags, int max_depth);

#endif //DIRSIZE_DIR_UTILS_H
