#include <stdlib.h>
#include <string.h>
#include "user.h"

user_t*
user_new() {
    user_t* user = (user_t*)malloc(sizeof(user_t));
    user->rooms = vec_new();
    user->username = NULL;
    user->sock = -1;

    return user;
}

user_t*
user_new_with_name(const char *username) {
    user_t *user = user_new();
    user->username = strdup(username);

    return user;
}

void
user_free(user_t *user) {
    vec_free(user->rooms);
    free(user->username);
    free(user);
}
