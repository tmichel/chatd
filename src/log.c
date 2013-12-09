#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LOG_SIZE 500

/* Determnines whether to use syslog or not. */
int on;

void
log_init(int d) {
    /* init syslog */
    openlog(NULL, LOG_CONS | LOG_PID, LOG_USER);
    on = d;
}

void
log_fini() {
    closelog();
}

void
log_sys(int level, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buf[MAX_LOG_SIZE] = {0};
    vsnprintf(buf, MAX_LOG_SIZE, format, args);

    if (on) {
        /* write syslog */
        syslog(level, "%s", buf);
    } else {
        write(STDOUT_FILENO, buf, MAX_LOG_SIZE);
        write(STDOUT_FILENO, "\n", 1);
    }

    va_end(args);
}