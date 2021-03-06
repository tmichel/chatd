#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "user.h"
#include "const.h"
#include "command.h"

user_t*
user_new() {
    user_t* user = (user_t*)malloc(sizeof(user_t));
    user->rooms = vec_new();
    user->username = NULL;
    user->password = NULL;
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
    free(user->password);
    free(user);
}

cr_t
user_message(user_t* const sender, user_t* const receiver, string message) {
    cr_t res = cr_empty();

    char *msg = (char*)calloc(sizeof(char), sizeof(char) * MAX_MESSAGE_SIZE);
    int len = snprintf(msg, MAX_MESSAGE_SIZE, MESSAGE_PRIV_FORMAT, sender->username, message.val);

    if (write(receiver->sock, msg, len) < 0) {
        // TODO write log
        res = cr_create(CMD_RES_WRT_ERR);
    } else {
        res = cr_ok();
    }

    free(msg);
    return res;
}

int
user_eq(any_t name, any_t user) {
    char *username = (char*)name;
    user_t *u = (user_t*)user;

    return strcmp(username, u->username) == 0;
}
