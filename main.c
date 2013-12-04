#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/* TODO: make these settable */
#define PORT 3222
#define MAX_CONN 100
#define MAX_DATA_SIZE 1024

int main(int argc, char const *argv[])
{
    int serv_sock, remote_sock;
    struct sockaddr_in serv_addr, remote_addr;
    socklen_t remote_addr_len;

    char buf[MAX_DATA_SIZE];
    int len;

    /* create socket for ipv4 */
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not create socket");
        return 1;
    }

    /* get address to bind to */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    /* bind to address */
    bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    /* listen on port */
    if ((listen(serv_sock, MAX_CONN)) < 0) {
        perror("Could not listen on socket");
        return 1;
    }

    remote_addr_len = sizeof(remote_addr);

    printf("Server started, waiting for connections...\n");

    while(1) {
        remote_sock = accept(serv_sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
        if (remote_sock < 0) {
            perror("Could not accept socket");
            continue;
        }

        /* echo back */
        while((len = read(remote_sock, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, len);
            write(remote_sock, buf, len);
        }
        printf("Done.\n");
        close(remote_sock);
    }

    close(serv_sock);
    return 0;
}