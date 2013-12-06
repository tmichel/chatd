#ifndef _MEM_H
#define _MEM_H

#include "user.h"
#include "room.h"

#define MEM_OK        0
#define MEM_NOTFOUND  1
#define MEM_FULL      2

typedef int mem_res;

/**
 * Init memory store.
 */
mem_res mem_init();

mem_res mem_store_user(user_t *user);

mem_res mem_lookup_user(const char *username, user_t **user);

mem_res mem_store_room(room_t *room);

mem_res mem_lookup_room(const char *room_name, room_t **roomu);

#endif