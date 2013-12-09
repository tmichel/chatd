#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>

#include "server.h"
#include "command.h"
#include "parser.h"
#include "string.h"
#include "const.h"
#include "user.h"
#include "room.h"
#include "mem.h"
#include "log.h"

#define MAX_DATA_SIZE 1024
#define MAX_RESPONSE_SIZE 100

char *RES_MSG[20] = {
    /* 100 */ "OK",
    /* 101 */ "ERR",
    /* 102 */ "NO SUCH USER",
    /* 103 */ "USER MUTED",
    /* 104 */ "NO_SUCH ROOM",
    /* 105 */ "WRITE ERR",
    /* 106 */ "PARSE ERR",
    /* 107 */ "NO SUCH COMMAND",
    /* 108 */ "USER NAME NOT AVAILABLE",
    /* 109 */ "NOT ALLOWED",
};

/**
 * Handle incoming message from a client.
 * Returns 1 if connection needs to be closed.
 */
static void* handle_conn(void* client_sock);
static int handle_message(string in, user_t * user, /* out */ cr_t *res);
static int open_serv_sock(const int port, const int max_conn);
static void send_response(int sock, cr_t res);
static void send_welcome(int sock);
static void user_quit(user_t*);

int start_server(const int port, const int max_conn)
{
    int serv_sock = open_serv_sock(port, max_conn);

    if (serv_sock < 0) {
        return 1;
    }

    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);

    printf("Server started on %d port, waiting for connections...\n", port);

    while(1) {
        char ip[NI_MAXHOST];
        char srv[NI_MAXSERV];
        int client_sock = accept(serv_sock, (struct sockaddr*)&remote_addr, &remote_addr_len);

        getnameinfo((struct sockaddr*)&remote_addr, remote_addr_len, ip, sizeof(ip), srv, sizeof(srv), NI_NUMERICHOST | NI_NUMERICSERV);

        if (client_sock < 0) {
            log_sys(LOG_ERR, "Could not accept connection from %s %s", ip, srv);
            continue;
        }

        log_sys(LOG_INFO, "Connection from %s:%s", ip, srv);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_conn, &client_sock);
    }

    close(serv_sock);
    return 0;
}

static void
send_response(int sock, cr_t res) {
    char *desc;
    if (res.code == CMD_RES_NOT_INIT) {
        log_sys(LOG_ERR, "Command result was not initialized.");
        // something went terribly wrong, got back unitialized command result
        desc = RES_MSG[CMD_RES_ERR - CMD_RES_OK];
    } else {
        desc = RES_MSG[res.code - CMD_RES_OK];
    }

    char reply[MAX_RESPONSE_SIZE];
    snprintf(reply, MAX_RESPONSE_SIZE, "%d %s\n", res.code, desc);

    if (write(sock, reply, strlen(reply)) < 0) {
        log_sys(LOG_ERR, "Could not send message to user.");
    }
}

static int
open_serv_sock(const int port, const int max_conn) {
    int serv_sock;

    /* create socket for ipv4 */
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_sys(LOG_ERR, "Could not create socket");
        return -1;
    }

    // get address to bind to
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    int reuse = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // bind to address
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // listen on port
    if ((listen(serv_sock, max_conn)) < 0) {
        log_sys(LOG_ERR, "Could not listen on socket");
        return -1;
    }

    return serv_sock;
}

static int
handle_message(string in, user_t *user, cr_t *res) {
    int quit = 0;

    command_t cmd = parse(in);;

    if (cmd.code == CMD_EXIT) {
        res->code = CMD_RES_OK;
        quit = 1;
    } else {
        // execute command
        cr_t result = command_execute(cmd, user);
        res->code = result.code;
        res->user = result.user;
    }
    command_destroy(cmd);

    return quit;
}

static void*
handle_conn(void *arg) {
    int client_sock = *((int*)arg);
    send_welcome(client_sock);

    char buf[MAX_DATA_SIZE] = {0};
    int len = 0;
    int quit = 0;
    user_t *user = NULL;

    while(!quit && (len = read(client_sock, buf, sizeof(buf))) > 0) {
        cr_t res = cr_empty();
        string in = str_newn(buf, len);
        quit = handle_message(in, user, &res);

        if (res.user != NULL) {
            user = res.user;
            user->sock = client_sock;
        }

        send_response(client_sock, res);
        memset(buf, 0, len);
    }

    user_quit(user);
    user_free(user);
    log_sys(LOG_INFO, "Client exited.");
    close(client_sock);

    return NULL;
}

static void
send_welcome(int sock) {
    char msg[60] = {0};
    sprintf(msg, "Welcome!\n"
                 "To register your nick send 'REG nick [password]'\n");

    if (write(sock, msg, strlen(msg)) < 0) {
        log_sys(LOG_ERR, "Could not send message.");
    }
}

static void
user_quit(user_t *user) {
    // remove user from every room she's in.
    room_t *r;
    for (int i = 0; i < vec_size(user->rooms); ++i) {
        vec_get(user->rooms, i, (any_t*)&r);

        // if error occured during removing user (broadcasting message)
        room_remove_user(r, user);

        if (vec_is_empty(r->users)) {
            mem_remove_room(r);
        }
    }
    mem_remove_user(user);
}
