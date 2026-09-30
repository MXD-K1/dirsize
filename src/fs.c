#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

struct FS_Dir {
    char* name;
#ifdef _WIN32
    HANDLE handle;
#else
    DIR* handle;
#endif
};

#include "fs.h"

FS_Dir* create_fs_dir(void) {
    FS_Dir* fs_dir = malloc(sizeof(FS_Dir));
    if (fs_dir == NULL) {
        /* TODO: exist program */
    }
    *fs_dir = (FS_Dir){0};
    return fs_dir;
}

#ifdef _WIN32
bool fs_open_dir(const char* path, FS_Dir** fs_dir) {
    char search_path[4096];
    HANDLE handle = INVALID_HANDLE_VALUE;

    WIN32_FIND_DATA data;

    // Create the search pattern by appending \* to the directory path
    snprintf(search_path, 4096, "%s\\*", path);
    handle = FindFirstFile(search_path, &data);

    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    FS_Dir* temp_dir = create_fs_dir();

    temp_dir->name = data.cFileName;
    temp_dir->handle = handle;
    *fs_dir = temp_dir;

    return true;
}
#else
bool fs_open_dir(const char* path, FS_Dir** fs_dir) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return false;
    }

    struct dirent *dirent = readdir(dir);
    if (dirent == NULL) {
        /* TODO: exit program */
    }

    FS_Dir* temp_dir = create_fs_dir();

    temp_dir->name = dirent->d_name;
    temp_dir->handle = dir;
    *fs_dir = temp_dir;

    return true;
}
#endif

#ifdef _WIN32
void fs_next_entry(FS_Dir* dir, bool* at_end) {
    WIN32_FIND_DATA data;
    *at_end = FindNextFile(dir->handle, &data);
    dir->name = data.cFileName;
}
#else
void fs_next_entry(FS_Dir* dir, bool* at_end) {
    struct dirent* dirent = readdir(dir->handle);
    if (dirent == NULL) {
        *at_end = true;
        return;
    }

    dir->name = dirent->d_name;
    *at_end = false;
}
#endif

#ifdef _WIN32
void fs_close_dir(FS_Dir* dir) {
    FindClose(dir->handle);
    free(dir);
}
#else
void fs_close_dir(FS_Dir* dir) {
    closedir(dir->handle);
    free(dir);
}
#endif

#ifdef _WIN32
bool fs_entry_is_dir(const char* path, bool* is_dir) {
    const DWORD attrs = GetFileAttributes(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
        *is_dir = true;
    } else {
        *is_dir = false;
    }
    return true;
}
#else
bool fs_entry_is_dir(const char* path, bool* is_dir) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return false;
    }

    *is_dir = S_ISDIR(st.st_mode);
    return true;
}
#endif

#ifdef _WIN32
bool fs_entry_is_file(const char* path, bool* is_file) {
    const DWORD attrs = GetFileAttributes(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
        *is_file = false;
    } else {
        *is_file = true;
    }
    return true;
}
#else
bool fs_entry_is_file(const char* path, bool* is_file) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return false;
    }

    *is_file = S_ISREG(st.st_mode);
    return true;
}
#endif

#ifdef _WIN32
bool fs_entry_is_hidden(const char* path, bool* is_hidden) {
    const DWORD attrs = GetFileAttributes(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    *is_hidden = attrs & FILE_ATTRIBUTE_HIDDEN;

    return true;
}
#else
bool fs_entry_is_hidden(const char* path, bool* is_hidden) {
    *is_hidden = path[0] == '.';
    return true;
}
#endif

#ifdef _WIN32
bool fs_entry_is_system_dir(const char* path, bool* is_system) {
    const DWORD attrs = GetFileAttributes(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    *is_system = attrs & FILE_ATTRIBUTE_SYSTEM;
    return true;
}
#else
bool fs_entry_is_system_dir(const char* path, bool* is_system) {
    *is_system = false;
    return true;
}
#endif

char* fs_get_entry_name(const FS_Dir* entry) {
    return entry->name;
}

#ifdef _WIN32
bool fs_get_file_size(const char* file_path, size_t* size) {
    HANDLE handle = INVALID_HANDLE_VALUE;

    // get a handle to that file
    handle = CreateFile(
        TEXT(file_path), GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (handle == INVALID_HANDLE_VALUE) return false;

    *size = (size_t) GetFileSize(handle, NULL);

    CloseHandle(handle);
    return true;
}
#else
bool fs_get_file_size(const char* file_path, size_t* size) {
    struct stat st;
    if (stat(file_path, &st) != 0) return false;

    *size = st.st_size;
    return true;
}
#endif
