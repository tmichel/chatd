#ifndef _LOG_H
#define _LOG_H

#include <syslog.h>

void log_init(int);
void log_fini();
void log_sys(int, const char*, ...);

#endif