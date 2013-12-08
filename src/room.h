#ifndef _ROOM_H
#define _ROOM_H

#include "types.h"

room_t *room_new(const char *name);

void room_free(room_t *room);

void room_add_user(room_t *room, user_t * const user, int is_admin);

void room_remove_user(room_t *room, user_t * const user);

cr_t room_send_msg(room_t * const, user_t * const, string msg);

#endif
