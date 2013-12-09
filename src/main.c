#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "server.h"
#include "mem.h"
#include "log.h"
#include "db.h"

/* TODO: make these settable */
#define PORT 3222
#define MAX_CONN 100
#define DEFAULT_DBNAME "chatd_log.db"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    mem_init();
    db_init(DEFAULT_DBNAME);
    // TODO: determine if running as daemon
    log_init(0);

    int res = start_server(PORT, MAX_CONN);
    if (res != 0) {
        perror("Could not start server.");
    }

    log_fini();
    db_fini();

    return 0;
}