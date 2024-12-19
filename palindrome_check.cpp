#include "palindrome_checker.h"
#include <cstring>

bool IsPalindrome(const char* palavra) {
    int i = 0;
    int j = strlen(palavra) - 1;

    while (i < j) {
        if (palavra[i] != palavra[j])
            return false;
        ++i;
        --j;
    }
    return true;
}
