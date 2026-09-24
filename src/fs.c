#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "fs.h"

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
