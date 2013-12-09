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

#define CMD_REG   0
#define CMD_EXIT  1
#define CMD_PWD   2
#define CMD_JOIN  3
#define CMD_TALK  4
#define CMD_LEAVE 5
#define CMD_MSG   6
#define CMD_GRANT 7
#define CMD_MUTE  8
#define CMD_VOICE 9
#define CMD_KICK 10

#define CMD_RES_NOT_INIT      0
#define CMD_RES_OK          100
#define CMD_RES_ERR         101
#define CMD_RES_NO_USR      102
#define CMD_RES_USR_MUTE    103
#define CMD_RES_NO_ROOM     104
#define CMD_RES_WRT_ERR     105
#define CMD_RES_PARSE_ERR   106
#define CMD_RES_NO_CMD      107
#define CMD_RES_USR_DUP     108
#define CMD_RES_NOT_ALLOWED 109

#endif