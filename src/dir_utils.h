#ifndef DIRSIZE_DIR_UTILS_H
#define DIRSIZE_DIR_UTILS_H

/* Temp */
#define SI_MODE     0x01
#define SHOW_HIDDEN 0x02

typedef struct dir Dir;

Dir* create_dir(const char* path);
void free_dir(Dir* dir);
Dir* traverse_tree(char* path);
void calc_size(Dir* root);

size_t get_dir_size(const Dir* dir);
char* get_dir_path(const Dir* dir);
size_t get_dir_file_count(const Dir* dir);
size_t get_dir_dir_count(const Dir* dir);
char** get_dir_files(const Dir* dir);
Dir** get_dir_dirs(const Dir* dir);
bool is_dir_hidden(const Dir* dir);

#endif //DIRSIZE_DIR_UTILS_H
