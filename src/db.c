#include "db.h"
#include "log.h"
#include "user.h"

#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

sqlite3 *db;

int
db_init(char *uri) {
    int rc;

    rc = sqlite3_open(uri, &db);
    /* Error close and return. */
    if (rc) {
        log_sys(LOG_CRIT, "Could not open database: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    return 0;
}

void
db_fini() {
    sqlite3_close(db);
}

user_t *
db_get_user(string username) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, "SELECT username, password FROM users WHERE username = ?", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        log_sys(LOG_ERR, "Could not created prepared statement."),
        sqlite3_finalize(stmt);
        return NULL;
    }
    sqlite3_bind_text(stmt, 1, username.val, username.len, NULL);
    rc = sqlite3_step(stmt);

    user_t *user = NULL;
    if (rc == SQLITE_ROW) {
        /* USER(id, username, password) */
        const unsigned char *username = sqlite3_column_text(stmt, 0);
        const unsigned char *password = sqlite3_column_text(stmt, 1);
        user = user_new();
        user->username = strdup((char*)username);
        user->password = strdup((char*)password);
    }

    sqlite3_finalize(stmt);
    return user;
}

enum db_result
db_store_user(const user_t *user) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, "INSERT INTO users(username, password) values (?, ?)", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        log_sys(LOG_ERR, "Could not created prepared statement."),
        sqlite3_finalize(stmt);
        return db_prepare_stmt_err;
    }
    sqlite3_bind_text(stmt, 1, user->username, strlen(user->username), NULL);
    sqlite3_bind_text(stmt, 2, user->password, strlen(user->password), NULL);
    rc = sqlite3_step(stmt);

    enum db_result res = db_ok;

    if (rc != SQLITE_DONE) {
        log_sys(LOG_ERR, "Database error: %s", sqlite3_errmsg(db));
        if (rc == SQLITE_CONSTRAINT) {
            res = db_constraint_err;
        }
        else {
            res = db_unknown;
        }
    }

    sqlite3_finalize(stmt);
    return res;
}

enum db_result
db_log_msg(const room_t *room, const char *msg) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, "INSERT INTO chat_log(room, message) values (?, ?)", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        log_sys(LOG_ERR, "Could not created prepared statement."),
        sqlite3_finalize(stmt);
        return db_prepare_stmt_err;
    }
    sqlite3_bind_text(stmt, 1, room->name, strlen(room->name), NULL);
    sqlite3_bind_text(stmt, 2, msg, strlen(msg), NULL);
    rc = sqlite3_step(stmt);

    enum db_result res = db_ok;

    if (rc != SQLITE_DONE) {
        log_sys(LOG_ERR, "Database error: %s", sqlite3_errmsg(db));
        if (rc == SQLITE_CONSTRAINT) {
            res = db_constraint_err;
        }
        else {
            res = db_unknown;
        }
    }

    sqlite3_finalize(stmt);
    return res;
}
