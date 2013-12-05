#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"
#include "command.h"
#include "parser.h"
#include "util.h"

#define MAX_DATA_SIZE 1024
#define MAX_RESPONSE_SIZE 1024

/**
 * Handle incoming message from a client.
 * Returns 1 if connection needs to be closed.
 */
static int handle_message(char *buf, size_t len, command_result_t *res);
static int open_serv_sock(const int port, const int max_conn);
static void send_response(int sock, command_result_t res);

int start_server(const int port, const int max_conn)
{
    int serv_sock = open_serv_sock(port, max_conn);

    if (serv_sock < 0) {
        return 1;
    }

    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);

    printf("Server started on %d port, waiting for connections...\n", port);

    char buf[MAX_DATA_SIZE];

    while(1) {
        int remote_sock = accept(serv_sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
        if (remote_sock < 0) {
            perror("Could not accept socket");
            continue;
        }

        int len = 0;
        int quit = 0;

        while(!quit && (len = read(remote_sock, buf, sizeof(buf))) > 0) {
            command_result_t res;
            quit = handle_message(buf, len, &res);
            send_response(remote_sock, res);
            empty(buf, len);
        }
        printf("Client exited.\n");
        close(remote_sock);
    }

    close(serv_sock);
    return 0;
}

static void
send_response(int sock, command_result_t res) {
    char *response = calloc(sizeof(char), sizeof(char) * MAX_RESPONSE_SIZE);
    int len = sprintf(response, "%d %s\n", res.code, res.msg);

    if (write(sock, response, len) < 0) {
        // TODO: log error
    }
    free(response);
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
handle_message(char *buf, size_t len, command_result_t *res) {
    int quit = 0;

    command_t *cmd = new_command();
    parse(buf, len, cmd);

    // execute command
    command_result_t result = command_execute(NULL, cmd);
    res->code = result.code;
    res->msg = result.msg;

    if (cmd->code == CMD_EXIT) {
        quit = 1;
    }

    free_command(cmd);

    return quit;
}
