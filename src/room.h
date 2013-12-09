#ifndef _ROOM_H
#define _ROOM_H

#include "types.h"

room_t *room_new(const char *name);

void room_free(room_t *room);

cr_t room_add_user(room_t *room, user_t * const user, int is_admin);

cr_t room_remove_user(room_t *room, user_t * const user);

cr_t room_send_msg(room_t * const, user_t * const, string msg);

cr_t room_admin(room_t* const, user_t* const, string);

cr_t room_mute_user(room_t* const, user_t* const, string);

cr_t room_voice_user(room_t* const, user_t* const, string);

cr_t room_kick_user(room_t* const, user_t* const, string);

#endif
