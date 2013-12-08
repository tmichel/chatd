#ifndef _CONST_H
#define _CONST_H

#define SYSMSG_BUF_SIZE 255
#define MAX_MESSAGE_SIZE 2048
#define MESSAGE_FORMAT "#%s <%s> %s\n"
#define MESSAGE_ADMIN_FORMAT "#%s <@%s> %s\n"
#define MESSAGE_PRIV_FORMAT "<%s> %s\n"
#define COMMAND_DELIM ' '

#define MEM_OK        0
#define MEM_NOTFOUND  1

/*
 * Command codes
 * Detailed information can be found in the PROTOCOL.md file (in Hungarian).
 */

#define CMD_PARSE_ERROR -1

#define CMD_REG 1
#define CMD_REG_STR "REG"

#define CMD_EXIT 2
#define CMD_EXIT_STR "EXIT"

#define CMD_PWD 3
#define CMD_PWD_STR "PWD"

#define CMD_JOIN 4
#define CMD_JOIN_STR "JOIN"

#define CMD_TALK 5
#define CMD_TALK_STR "TALK"

#define CMD_LEAVE 6
#define CMD_LEAVE_STR "LEAVE"

#define CMD_MSG 7
#define CMD_MSG_STR "MSG"

#define CMD_RES_OK      100
#define CMD_RES_ERR     101
#define CMD_RES_NO_USR  102
#define CMD_RES_USR_BND 103 // banned
#define CMD_RES_NO_ROOM 104
#define CMD_RES_WRT_ERR 105

#endif