#ifndef _DB_H
#define _DB_H

#include "string.h"
#include "types.h"

enum db_result {
    db_ok,
    db_constraint_err,
    db_prepare_stmt_err,
    db_unknown
};

/* Initialize database. Returns 0 on success*/
int db_init(char*);

/* Finalize database */
void db_fini();

/* Retrieve user based on username */
user_t *db_get_user(string);

/*
 * Store a user in the database.
 */
enum db_result db_store_user(const user_t*);

/* Store a log message in the database */
enum db_result db_log_msg(const room_t*, const char *msg);

#endif
