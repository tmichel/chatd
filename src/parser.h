/*
    Command parser for the chat server.
*/
#ifndef _PARSER_H
#define _PARSER_H

#include "command.h"

#define COMMAND_DELIM " "

/*
 * Parse the raw input into a command_t struct.
 */
void parse(const char *input, size_t len, command_t *cmd);

#endif