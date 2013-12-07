#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "util.h"

user_t*
new_user() {
    user_t* user = (user_t*)malloc(sizeof(user_t));
    user->username = NULL;
    user->sock = -1;

    return user;
}

user_t*
new_user_with_name(const char *username) {
    user_t *user = new_user();

    int len = strlen(username);
    user->username = (char*)calloc(sizeof(char), sizeof(char) * (len+1));
    strcpy(user->username, username);

    return user;
}

void
free_user(user_t *user) {
    free(user->username);
    free(user);
}
