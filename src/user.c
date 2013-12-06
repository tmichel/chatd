#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "util.h"

#define TOKEN_LENGTH 5

user_t*
new_user() {
    user_t* user = (user_t*)malloc(sizeof(user_t));
    user->token = NULL;
    user->username = NULL;
    user->sock = -1;

    // NOTE: +1 for the ending \0
    user->token = calloc(sizeof(char), sizeof(char) * (TOKEN_LENGTH + 1));
    rand_str(user->token, TOKEN_LENGTH);

    return user;
}

user_t*
new_user_with_name(const char *username) {
    user_t *user = new_user();

    int len = strlen(username);
    user->username = (char*)calloc(sizeof(char), sizeof(char) * len);
    strcpy(user->username, username);

    return user;
}

void
free_user(user_t *user) {
    free(user->token);
    free(user->username);
    free(user);
}