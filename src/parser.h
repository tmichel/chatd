/*
    Command parser for the chat server.
*/
#ifndef _PARSER_H
#define _PARSER_H

#include "types.h"

/*
 * Parse the raw input into a command_t struct.
 */
command_t parse(string in);

#endif