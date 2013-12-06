#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "server.h"
#include "command.h"
#include "parser.h"

/* TODO: make these settable */
#define PORT 3222
#define MAX_CONN 100

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    int res = start_server(PORT, MAX_CONN);

    if (res != 0) {
        perror("Could not start server.");
    }

    return 0;
}