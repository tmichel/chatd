#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "string.h"

string
str_newn(const char *str, int n) {
    char *cpy = (char*)calloc(sizeof(char), sizeof(char) * (n+1));
    strncpy(cpy, str, n);

    string s = { n, cpy };
    return s;
}

string
str_new(const char *str) {
    int n = strlen(str);
    return str_newn(str, n);
}

void
str_destroy(string str) {
    free((char*)str.val);
}

string
str_empty() {
    return str_new("");
}

int
str_is_empty(string str) {
    return str.len == 0 && strcmp("", str.val) == 0;
}

string
str_nil() {
    string s = {-1, NULL};
    return s;
}

int
str_is_nil(string str) {
    return str.len == -1 && str.val == NULL;
}

string
str_sub(string str, int start, int count) {
    if (str.len < count + start || start < 0) {
        return NIL;
    }

    if (count == 0) {
        return str_empty();
    }
    // return the rest
    if (count < 0) {
        count = str.len - start;
    }

    return str_newn(str.val + start, count);
}

int
str_eq(string s1, string s2) {
    if (s1.len != s2.len) {
        return 0;
    }

    if (str_is_nil(s1) && str_is_nil(s2)) {
        return 1;
    }

    return strcmp(s1.val, s2.val) == 0;
}

string
str_trim(string str) {
    int lindex = 0;
    int rindex = str.len - 1;

    while(isspace(str.val[lindex])) {
        ++lindex;
    }
    while (rindex > lindex && isspace(str.val[rindex])) {
        --rindex;
    }


    return str_sub(str, lindex, rindex - lindex + 1);
}


tok_t
str_tok_init(char sep, string str) {
    tok_t token = {sep, str, 0, 1};
    return token;
}

string
str_tok(tok_t *tok, int include_sep) {
    int i = tok->start;
    string str = tok->str;

    // start > len
    if (i > str.len || !tok->has_more) {
        return NIL;
    }

    // look for the next sep
    while (i <= str.len && tok->sep != str.val[i]) ++i;

    if (i > str.len) {
        tok->has_more = 0;
        return str_sub(str, tok->start, -1);
    }

    int count = include_sep ? i + 1 : i;
    string token = str_sub(str, tok->start, count);

    tok->start = i + 1;
    tok->has_more = 1;

    return token;
}

