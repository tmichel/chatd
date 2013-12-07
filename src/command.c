#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "command.h"
#include "mem.h"
#include "room.h"

struct room_req {
    command_t cmd;
    user_t *user;
    room_t *room;
    tok_t tok;
    cr_t *res;
};

static void make_result(cr_t *res, cc_t code, char *msg);
static void command_ok(cr_t *res, char *msg);
static void command_err(cr_t *res, char *msg);
static void command_parse_error(cr_t *res);
static cr_t user_reg(command_t cmd);
static cr_t user_join(command_t cmd, user_t* const user);
static void user_leave(struct room_req);
static void user_talk(struct room_req);
static cr_t user_change_pwd(user_t* const user, const command_t *cmd);
static cr_t user_exit(command_t cmd, user_t* const user);
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
cr_init() {
    cr_t res = {CMD_RES_ERR, NULL, NULL};
    return res;
}

cr_t
cr_create(cc_t code, char *msg) {
    cr_t res = cr_init();

    res.code = code;
    res.msg = strdup(msg);

    return res;
}

cr_t
cr_ok() {
    return cr_create(CMD_RES_OK, "OK");
}

cr_t
command_execute(command_t cmd, user_t * const user) {
    cr_t res = cr_init();

    if (cmd.code == CMD_PARSE_ERROR) {
        command_parse_error(&res);
        return res;
    }

    // everything needs a user except REG
    if (cmd.code != CMD_REG && user == NULL) {
        make_result(&res, CMD_RES_NO_USR, "No user found for connection.");
        return res;
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
    }

    command_parse_error(&res);
    return res;
}

/* User registration. */
static cr_t
user_reg(command_t cmd) {
    cr_t res = cr_init();
    tok_t tok = str_tok_init(COMMAND_DELIM, cmd.args);

    string username = str_tok(&tok, SEP_EXCL);

    // if we could not find a username
    if (str_is_nil(username)) {
        command_parse_error(&res);
        str_destroy(username);
        return res;
    }

    string password = str_tok(&tok, SEP_EXCL);

    user_t *user = NULL;
    mem_res lookup_res = mem_lookup_user(username.val, &user);

    if (lookup_res == MEM_OK) {
        command_err(&res, "ERROR: Cannot login twice.");
        str_destroy(username);
        str_destroy(password);
        return res;
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

    char msg[100];
    sprintf(msg, "Welcome <%s>. Join a room with 'JOIN room'", user->username);

    command_ok(&res, msg);
    res.user = user;
    str_destroy(username);
    str_destroy(password);
    return res;
}

/* Get a room.
params:
    user: the user who will join the room
    cmd: the actual command*/
static cr_t
user_join(command_t cmd, user_t * const user) {
    cr_t res = cr_init();
    tok_t tok = str_tok_init(COMMAND_DELIM, cmd.args);

    string room_name = str_tok(&tok, SEP_EXCL);

    if (str_is_nil(room_name)) {
        // error: no rome name
        command_parse_error(&res);
        str_destroy(room_name);
        return res;
    }

    room_t *room = NULL;
    mem_res l_res = mem_lookup_room(room_name.val, &room);
    switch (l_res) {
    case MEM_OK:
        room_add_user(room, user, 0);
        break;
    case MEM_NOTFOUND:
        room = room_new(room_name.val);
        room_add_user(room, user, 1);
        // TODO: error handling?
        mem_store_room(room);
        break;
    default:
        command_err(&res, "Unspecified error. Could not join the room.");
        str_destroy(room_name);
        return res;
    }

    char msg[100];
    sprintf(msg, "Joined room #%s, which currently has %d user(s).", room_name.val, vec_size(room->users));
    command_ok(&res, msg);
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
    // remove user from every room she's in.
    room_t *r;
    for (int i = 0; i < vec_size(user->rooms); ++i) {
        vec_get(user->rooms, i, (any_t*)&r);
        room_remove_user(r, user);

        if (vec_is_empty(r->users)) {
            mem_remove_room(r);
        }
    }
    mem_remove_user(user);

    return cr_ok();
}

static void
user_leave(struct room_req req) {
    room_remove_user(req.room, req.user);

    if (vec_is_empty(req.room->users)) {
        mem_remove_room(req.room);
    }
}

static cr_t
exec_room(command_t cmd, user_t* const user, void (*exec)(struct room_req)) {
    cr_t res = cr_init();
    tok_t tok = str_tok_init(COMMAND_DELIM, cmd.args);
    room_t *room = NULL;

    string rname = str_tok(&tok, SEP_EXCL);
    if (str_is_nil(rname)) {
        command_parse_error(&res);
    } else if (mem_lookup_room(rname.val, &room) == MEM_OK) {
        struct room_req req = {cmd, user, room, tok, &res};
        exec(req);
    } else {
        make_result(&res, CMD_RES_NO_ROOM, "No such room");
    }

    str_destroy(rname);
    return res;
}


static void
make_result(cr_t *res, cc_t code, char *msg) {
    res->code = code;
    res->msg = strdup(msg);
}

static void
command_ok(cr_t *res, char *msg) {
    make_result(res, CMD_RES_OK, msg);
}

static void
command_err(cr_t *res, char *msg) {
    make_result(res, CMD_RES_ERR, msg);
}

static void
command_parse_error(cr_t *res) {
    command_err(res, "ERROR: could not parse command.");
}
