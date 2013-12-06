#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "util.h"

#define TOKEN_LENGTH 5

/**
 * Compatibility layer for freeing users.
 */
static void free_user_comp(void *);

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
    user->username = (char*)calloc(sizeof(char), sizeof(char) * (len+1));
    strcpy(user->username, username);

    return user;
}

void
free_user(user_t *user) {
    free(user->token);
    free(user->username);
    free(user);
}

room_t*
new_room(const char *name) {
    room_t* room = (room_t*)malloc(sizeof(room_t));

    // copy name
    int len = strlen(name);
    room->name = (char*)calloc(sizeof(char), sizeof(char) * (len + 1));
    strcpy(room->name, name);

    // init users, admin
    room->users = new_vec();
    room->admins = new_vec();

    return room;
}

void
free_room(room_t *room) {
    free(room->name);
    free_vec(room->users, free_user_comp);
    free_vec(room->admins, free_user_comp);
    free(room);
}

static void
free_user_comp(void* val) {
    free_user((user_t*)val);
}
