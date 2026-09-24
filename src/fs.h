#ifndef DIRSIZE_FILESYSTEM_H
#define DIRSIZE_FILESYSTEM_H

#include <stdbool.h>

bool fs_entry_is_dir(const char* path, bool* is_dir);
bool fs_entry_is_file(const char* path, bool* is_file);
bool fs_entry_is_hidden(const char* path, bool* is_hidden);
bool fs_entry_is_system_dir(const char* path, bool* is_system);
bool fs_get_file_size(const char* file_path, size_t* size);

/* TODO:
 * fs_open_dir()
 * fs_next_entry()
 * fs_close_dir()
*/

#endif //DIRSIZE_FILESYSTEM_H
