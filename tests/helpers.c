#include <stdio.h>
#include <string.h>

#include "helpers.h"

bool str_equals(const char* str1, const char* str2) {
    return strcmp(str1, str2) == 0;
}

void mark_test_end(void) {
    printf("All Tests Passed\n");
}
