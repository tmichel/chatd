#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"
#include "command.h"
#include "parser.h"
#include "string.h"

#define MAX_DATA_SIZE 1024
#define MAX_RESPONSE_SIZE 1024

/**
 * Handle incoming message from a client.
 * Returns 1 if connection needs to be closed.
 */
static void handle_conn(int client_sock);
static int handle_message(string in, user_t * user, /* out */ cr_t *res);
static int open_serv_sock(const int port, const int max_conn);
static void send_response(int sock, cr_t res);
static void send_welcome(int sock);

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
        int client_sock = accept(serv_sock, (struct sockaddr*)&remote_addr, &remote_addr_len);

        if (client_sock < 0) {
            perror("Could not accept socket");
            continue;
        }

        // TODO: log client info
        handle_conn(client_sock);
    }

    close(serv_sock);
    return 0;
}

static void
send_response(int sock, cr_t res) {
    char response[MAX_RESPONSE_SIZE] = {0};
    int len = sprintf(response, "%d %s\n", res.code, res.msg);

    if (write(sock, response, len) < 0) {
        // TODO: log error
    }
}

static int
open_serv_sock(const int port, const int max_conn) {
    int serv_sock;

    /* create socket for ipv4 */
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not create socket");
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
        perror("Could not listen on socket");
        return -1;
    }

    return serv_sock;
}

static int
handle_message(string in, user_t *user, cr_t *res) {
    int quit = 0;

    command_t cmd = parse(in);;

    // execute command
    cr_t result = command_execute(cmd, user);
    res->code = result.code;
    res->msg = result.msg;
    res->user = result.user;

    if (cmd.code == CMD_EXIT) {
        quit = 1;
    }

    command_destroy(cmd);

    return quit;
}

static void
handle_conn(int client_sock) {
    send_welcome(client_sock);

    char buf[MAX_DATA_SIZE] = {0};
    int len = 0;
    int quit = 0;
    user_t *user;

    while(!quit && (len = read(client_sock, buf, sizeof(buf))) > 0) {
        cr_t res = cr_init();
        string in = str_newn(buf, len);
        quit = handle_message(in, user, &res);

        if (res.user != NULL) {
            user = res.user;
            user->sock = client_sock;
        }

        send_response(client_sock, res);
        free(res.msg);
        memset(buf, 0, len);
    }
    printf("Client exited.\n");
    close(client_sock);
}

static void
send_welcome(int sock) {
    char msg[60] = {0};
    sprintf(msg, "Welcome!\n"
                 "To register your nick send 'REG nick [password]'\n");

    if (write(sock, msg, strlen(msg)) < 0) {
        // TODO: log error
    }
}
