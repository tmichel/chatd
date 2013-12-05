#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"
#include "command.h"
#include "parser.h"

#define MAX_DATA_SIZE 1024

static int
open_serv_sock(const int port, const int max_conn) {
    int serv_sock;

    /* create socket for ipv4 */
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not create socket");
        return 1;
    }

    // get address to bind to
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    // bind to address
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // listen on port
    if ((listen(serv_sock, max_conn)) < 0) {
        perror("Could not listen on socket");
        return 1;
    }

    return serv_sock;
}

int start_server(const int port, const int max_conn)
{
    int serv_sock = open_serv_sock(port, max_conn);

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

        // echo back.
        while((len = read(remote_sock, buf, sizeof(buf))) > 0) {

            command_t *cmd = new_command();
            parse(buf, cmd);
            printf("%d\n", cmd->code);
            if (cmd->code == CMD_EXIT) {
                printf("YAY\n");
            }
            free_command(cmd);

            write(STDOUT_FILENO, buf, len);
        }
        printf("Done.\n");
        close(remote_sock);
    }

    close(serv_sock);
    return 0;
}
