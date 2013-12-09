#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "command.h"
#include "mem.h"
#include "user.h"
#include "room.h"
#include "db.h"

struct room_req {
    command_t cmd;
    user_t *user;
    room_t *room;
    tok_t tok;
    cr_t *res;
};

static cr_t cmd_reg(command_t cmd);
static cr_t cmd_join(command_t cmd, user_t* const user);
static void cmd_leave(struct room_req);
static void cmd_talk(struct room_req);
static void cmd_grant(struct room_req);
static void cmd_mute(struct room_req);
static void cmd_voice(struct room_req);
static void cmd_kick(struct room_req);
static cr_t cmd_msg(command_t cmd, user_t* const user);
static cr_t exec_room_cmd(command_t cmd, user_t* const user, void (*f)(struct room_req));
static user_t* login(string, string, cr_t*);

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
        return cmd_reg(cmd);
    case CMD_JOIN:
        return cmd_join(cmd, user);
    case CMD_TALK:
        return exec_room_cmd(cmd, user, cmd_talk);
    case CMD_LEAVE:
        return exec_room_cmd(cmd, user, cmd_leave);
    case CMD_MSG:
        return cmd_msg(cmd, user);
    case CMD_GRANT:
        return exec_room_cmd(cmd, user, cmd_grant);
    case CMD_MUTE:
        return exec_room_cmd(cmd, user, cmd_mute);
    case CMD_VOICE:
        return exec_room_cmd(cmd, user, cmd_voice);
    case CMD_KICK:
        return exec_room_cmd(cmd, user, cmd_kick);
    }

    return cr_create(CMD_RES_NO_CMD);
}

/* User registration. */
static cr_t
cmd_reg(command_t cmd) {
    cr_t res = cr_ok();
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
        user = login(username, password, &res);
        if (user != NULL) {
            mem_store_user(user);
        }
    }

    // cleanup and return
    str_destroy(username);
    str_destroy(password);
    res.user = user; // newly created user or NULL
    return res;
}

/* Get a room.
params:
    user: the user who will join the room
    cmd: the actual command*/
static cr_t
cmd_join(command_t cmd, user_t * const user) {
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
cmd_talk(struct room_req req) {
    string message = str_tok_rest(req.tok);
    *req.res = room_send_msg(req.room, req.user, message);
    str_destroy(message);
}

static void
cmd_leave(struct room_req req) {
    *req.res = room_remove_user(req.room, req.user);

    if (vec_is_empty(req.room->users)) {
        mem_remove_room(req.room);
    }
}

static cr_t
cmd_msg(command_t cmd, user_t * const sender) {
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
cmd_grant(struct room_req req) {
    string username = str_tok_rest(req.tok);
    *req.res = room_admin(req.room, req.user, username);
    str_destroy(username);
}

static cr_t
exec_room_cmd(command_t cmd, user_t* const user, void (*exec)(struct room_req)) {
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

static void
cmd_mute(struct room_req req) {
    string username = str_tok_rest(req.tok);
    *req.res = room_mute_user(req.room, req.user, username);
    str_destroy(username);
}

static void
cmd_voice(struct room_req req) {
    string username = str_tok_rest(req.tok);
    *req.res = room_voice_user(req.room, req.user, username);
    str_destroy(username);
}

static void
cmd_kick(struct room_req req) {
    string username = str_tok_rest(req.tok);
    *req.res = room_kick_user(req.room, req.user, username);
    str_destroy(username);
}

static user_t*
login(string username, string password, cr_t *res) {
    user_t* user = db_get_user(username);

    // password was specified. login or register
    if (!str_is_nil(password) && !str_is_empty(password)) {
        // no user found with that name. register user
        if (user == NULL) {
            user = user_new_with_name(username.val);
            user->password = strdup(password.val);
            if (db_store_user(user) == db_ok) {
               return user;
            } else {
                // there is a user with this name in the database
                // name must be unique
                user_free(user);
                res->code = CMD_RES_USR_DUP;
                return NULL;
            }
        }

        // there is a user, try to authenticate
        if (strcmp(user->password, password.val) != 0) {
            user_free(user);
            res->code = CMD_RES_INV_PWD;
            user = NULL;
        }
    } else if (user != NULL) {
        res->code = CMD_RES_USR_DUP;
        user_free(user);
        user = NULL;
    } else {
        user = user_new_with_name(username.val);
    }
    return user;
}
