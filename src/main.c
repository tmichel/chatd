#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "server.h"
#include "mem.h"
#include "log.h"
#include "db.h"

/* TODO: make these settable */
#define PORT 3222
#define MAX_CONN 100
#define DEFAULT_DBNAME "chatd_log.db"

struct options {
    int port;
    int max_conn;
    int is_daemon;
    char *db_uri;
};

static struct options parse_opts(int argc, char *argv[]);
static void daemonize();
static void register_term_handler();
static void term(int);

int main(int argc, char *argv[])
{
    struct options opts = parse_opts(argc, argv);

    if (opts.is_daemon)
        daemonize();

    // register signals
    register_term_handler();

    srand(time(NULL));
    mem_init();
    db_init(opts.db_uri);
    log_init(opts.is_daemon);

    if (!opts.is_daemon) {
        printf("Server started on %d port, waiting for connections...\n", opts.port);
    }

    int res = start_server(opts.port, opts.max_conn);
    if (res != 0) {
        log_sys(LOG_CRIT, "Could not start server");
    }

    log_fini();
    db_fini();

    return res;
}

static struct options
parse_opts(int argc, char *argv[]) {
    int i = 1;
    char *arg = NULL;

    char pwd[1024];
    getcwd(pwd, 1024);
    int size = strlen(pwd) + strlen(DEFAULT_DBNAME) + 2;
    char *uri = (char*)calloc(sizeof(char), sizeof(char) * size);
    snprintf(uri, size, "%s/%s", pwd, DEFAULT_DBNAME);

    struct options opts;
    opts.is_daemon = 0;
    opts.port = PORT;
    opts.max_conn = MAX_CONN;
    opts.db_uri = uri;

    while(i < argc) {
        arg = argv[i];

        if (arg[0] == '-') {
            switch(arg[1]) {
                case 'p':
                    opts.port = atoi(argv[++i]);
                    break;
                case 'd':
                    opts.is_daemon = 1;
                    break;
                case 'm':
                    opts.max_conn = atoi(argv[++i]);
                    break;
                case'b':
                    // free the previously allocation "default" uri
                    free(opts.db_uri);
                    opts.db_uri = strdup(argv[++i]);
                case '?':
                case 'h':
                    printf("Usage: chatd [-p PORT] [-m MAXCONN] [-d] [-b DBPATH]\n");
                    printf("\t-m: the maximum length of the queue of pending connections. Default value is 100.\n");
                    printf("\t-p: the port to listen on. Default: 3222\n");
                    printf("\t-d: run as daemon.\n");
                    printf("\t-b: the path of the database. Should be an absolute path when used it -d\n");
                    printf("\t-?, h: prints this message\n");
                    exit(0);
                    break;
            }
        }

        ++i;
    }

    return opts;
}

static void
daemonize() {
    /* Our process ID and Session ID */
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
       we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

static void
term(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        log_fini();
        db_fini();
        exit(EXIT_SUCCESS);
    }
}

static void
register_term_handler() {
    struct sigaction *act = (struct sigaction*)malloc(sizeof(struct sigaction));
    memset(act, 0, sizeof(struct sigaction));
    act->sa_handler = term;

    sigaction(SIGINT, act, NULL);
    sigaction(SIGTERM, act, NULL);
}
