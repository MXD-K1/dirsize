#ifndef DIRSIZE_FILESYSTEM_H
#define DIRSIZE_FILESYSTEM_H

#include <stdbool.h>

typedef struct FS_Dir FS_Dir;

FS_Dir* create_fs_dir(void);
bool fs_search_open_dir(const char* path, FS_Dir* fs_dir);
void fs_close_dir(FS_Dir* dir);
void fs_next_entry(FS_Dir* dir, bool* at_end);
bool fs_entry_is_dir(const char* path, bool* is_dir);
bool fs_entry_is_file(const char* path, bool* is_file);
bool fs_entry_is_hidden(const char* path, bool* is_hidden);
bool fs_entry_is_system_dir(const char* path, bool* is_system);
void fs_get_entry_name(const FS_Dir* entry, char* name);
bool fs_get_file_size(const char* file_path, size_t* size);

#endif //DIRSIZE_FILESYSTEM_H
