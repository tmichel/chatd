#ifndef _MEM_H
#define _MEM_H

#include "types.h"

typedef int mem_res;

/**
 * Init memory store.
 */
void mem_init();

mem_res mem_store_user(user_t *user);

mem_res mem_remove_user(user_t* const user);

mem_res mem_lookup_user(const char *username, user_t **user);

mem_res mem_store_room(room_t *room);

mem_res mem_remove_room(room_t* const room);

mem_res mem_lookup_room(const char *room_name, room_t **roomu);

#endif