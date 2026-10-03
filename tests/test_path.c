#include <assert.h>

#include "helpers.h"
#include "../include/path.h"

#ifdef _WIN32
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

void test_join_path(void) {
    assert(str_equals(join_path("x", "y"), "x" PATH_SEP "y"));
    assert(str_equals(join_path("", "y"), PATH_SEP "y"));
    assert(str_equals(join_path("x", ""), "x" PATH_SEP));
    assert(str_equals(join_path("x" PATH_SEP, "y"), "x" PATH_SEP PATH_SEP "y"));
    assert(str_equals(join_path("x", PATH_SEP "y"), "x" PATH_SEP PATH_SEP "y"));
    assert(str_equals(join_path("x", "y" PATH_SEP), "x" PATH_SEP "y" PATH_SEP));
    assert(str_equals(join_path("", ""), PATH_SEP));
}

void test_normalize_path(void) {
    assert(str_equals(normalize_path(""), ""));
    assert(str_equals(normalize_path("x\\y/z"), "x" PATH_SEP "y" PATH_SEP "z"));
}

int main() {
    test_join_path();
    test_normalize_path();
    mark_test_end();
}
