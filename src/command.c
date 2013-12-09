#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "command.h"
#include "mem.h"
#include "user.h"
#include "room.h"

struct room_req {
    command_t cmd;
    user_t *user;
    room_t *room;
    tok_t tok;
    cr_t *res;
};

static cr_t user_reg(command_t cmd);
static cr_t user_join(command_t cmd, user_t* const user);
static void user_leave(struct room_req);
static void user_talk(struct room_req);
static void user_grant(struct room_req);
static cr_t user_exit(command_t cmd, user_t* const user);
static cr_t user_msg(command_t cmd, user_t* const user);
static cr_t exec_room(command_t cmd, user_t* const user, void (*f)(struct room_req));

command_t
command_new(cc_t code, string args) {
    command_t cmd = {code, args};
    return cmd;
}

void
command_destroy(command_t cmd) {
    str_destroy(cmd.args);
}

cr_t
cr_empty() {
    return cr_create(CMD_RES_NOT_INIT);
}

cr_t
cr_create(cc_t code) {
    cr_t res = {code, NULL};
    return res;
}

cr_t
cr_ok() {
    return cr_create(CMD_RES_OK);
}

cr_t
cr_parse_err() {
    return cr_create(CMD_RES_PARSE_ERR);
}

cr_t
command_execute(command_t cmd, user_t * const user) {
    if (cmd.code == CMD_PARSE_ERROR) {
        return cr_parse_err();
    }

    // everything needs a user except REG
    if (cmd.code != CMD_REG && user == NULL) {
        return cr_create(CMD_RES_NO_USR);
    }

    switch (cmd.code) {
    case CMD_REG:
        return user_reg(cmd);
    case CMD_JOIN:
        return user_join(cmd, user);
    case CMD_TALK:
        return exec_room(cmd, user, user_talk);
    case CMD_LEAVE:
        return exec_room(cmd, user, user_leave);
    case CMD_EXIT:
        return user_exit(cmd, user);
    case CMD_MSG:
        return user_msg(cmd, user);
    case CMD_GRANT:
        return exec_room(cmd, user, user_grant);
    }

    return cr_create(CMD_RES_NO_CMD);
}

/* User registration. */
static cr_t
user_reg(command_t cmd) {
    tok_t tok = str_tok_init(COMMAND_DELIM, cmd.args);

    string username = str_tok(&tok, SEP_EXCL);

    // if we could not find a username
    if (str_is_nil(username)) {
        str_destroy(username);
        return cr_parse_err();
    }

    string password = str_tok(&tok, SEP_EXCL);

    user_t *user = NULL;
    mem_res lookup_res = mem_lookup_user(username.val, &user);

    if (lookup_res == MEM_OK) {
        str_destroy(username);
        str_destroy(password);
        return cr_create(CMD_RES_USR_DUP);
    }

    if (lookup_res == MEM_NOTFOUND) {
        // password was specified. login or register
        if (!str_is_nil(password) && !str_is_empty(password)) {
            // TODO: lookup from persistent store and authenticate
            // user = ...

            // atm we dont have a persistent store, so create a new user
            printf("Registering with password.");
            user = user_new_with_name(username.val);
        } else {
            user = user_new_with_name(username.val);
        }
        mem_store_user(user);
    }

    // cleanup and return
    str_destroy(username);
    str_destroy(password);
    cr_t res = cr_ok();
    res.user = user; // newly created user
    return res;
}

/* Get a room.
params:
    user: the user who will join the room
    cmd: the actual command*/
static cr_t
user_join(command_t cmd, user_t * const user) {
    cr_t res = cr_create(CMD_RES_ERR);
    tok_t tok = str_tok_init(COMMAND_DELIM, cmd.args);
    string room_name = str_tok(&tok, SEP_EXCL);

    // error: no rome name
    if (str_is_nil(room_name)) {
        str_destroy(room_name);
        return cr_parse_err();
    }

    room_t *room = NULL;
    mem_res l_res = mem_lookup_room(room_name.val, &room);
    switch (l_res) {
    case MEM_OK:
        res = room_add_user(room, user, 0);
        break;
    case MEM_NOTFOUND:
        room = room_new(room_name.val);
        res = room_add_user(room, user, 1);
        mem_store_room(room);
        break;
    }

    str_destroy(room_name);
    return res;
}

static void
user_talk(struct room_req req) {
    string message = str_tok_rest(req.tok);
    *req.res = room_send_msg(req.room, req.user, message);
    str_destroy(message);
}

static cr_t
user_exit(command_t cmd, user_t * const user) {
    cr_t res = cr_ok();

    // remove user from every room she's in.
    room_t *r;
    for (int i = 0; i < vec_size(user->rooms); ++i) {
        vec_get(user->rooms, i, (any_t*)&r);

        // if error occured during removing user (broadcasting message)
        cr_t tmp = room_remove_user(r, user);
        if (tmp.code != CMD_RES_OK) {
            res = tmp;
        }

        if (vec_is_empty(r->users)) {
            mem_remove_room(r);
        }
    }
    mem_remove_user(user);

    return res;
}

static void
user_leave(struct room_req req) {
    *req.res = room_remove_user(req.room, req.user);

    if (vec_is_empty(req.room->users)) {
        mem_remove_room(req.room);
    }
}

static cr_t
user_msg(command_t cmd, user_t * const sender) {
    cr_t res = cr_empty();
    tok_t tok = str_tok_init(COMMAND_DELIM, cmd.args);
    user_t *receiver = NULL;

    string uname = str_tok(&tok, SEP_EXCL);
    if (str_is_nil(uname)) {
        res = cr_parse_err();
    } else if (mem_lookup_user(uname.val, &receiver) == MEM_OK) {
        string rest = str_tok_rest(tok);
        res = user_message(sender, receiver, rest);
        str_destroy(rest);
    } else {
        res = cr_create(CMD_RES_NO_USR);
    }

    str_destroy(uname);
    return res;
}

static void
user_grant(struct room_req req) {
    string username = str_tok_rest(req.tok);
    *req.res = room_admin(req.room, req.user, username);
    str_destroy(username);
}

static cr_t
exec_room(command_t cmd, user_t* const user, void (*exec)(struct room_req)) {
    cr_t res = cr_empty();
    tok_t tok = str_tok_init(COMMAND_DELIM, cmd.args);
    room_t *room = NULL;

    string rname = str_tok(&tok, SEP_EXCL);
    if (str_is_nil(rname)) {
        res = cr_parse_err();
    } else if (mem_lookup_room(rname.val, &room) == MEM_OK) {
        struct room_req req = {cmd, user, room, tok, &res};
        exec(req);
    } else {
        res = cr_create(CMD_RES_NO_ROOM);
    }

    str_destroy(rname);
    return res;
}
