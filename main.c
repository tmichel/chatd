#include <stdio.h>

#include "server.h"

/* TODO: make these settable */
#define PORT 3222
#define MAX_CONN 100

int main(int argc, char const *argv[])
{
    int res = start_server(PORT, MAX_CONN);

    if (res != 0) {
        perror("Could not start server.");
    }
    return 0;
}