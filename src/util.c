#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "util.h"

void trim(const char *str, char *dest) {
    int len = strlen(str);
    int lindex = 0;
    int rindex = len - 1;

    while(isspace(str[lindex])) {
        ++lindex;
    }

    while (rindex > lindex && isspace(str[rindex])) {
        --rindex;
    }

    strncpy(dest, str + lindex, rindex - lindex + 1);
}

void empty(char *str, int len) {
    if (len == U_NOT_SPECIFIED) {
        len = strlen(str);
    }

    for (int i = 0; i < len; i++) {
        str[i] = 0;
    }
}

void rand_str(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}