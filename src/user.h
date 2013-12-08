#ifndef _USER_H
#define _USER_H

#include "types.h"

user_t* user_new();

user_t* user_new_with_name(const char *username);

void user_free(user_t *user);

#endif
