#ifndef _E_STRING_H
#define _E_STRING_H

#define NIL str_nil()
#define SEP_EXCL 0
#define SEP_INCL 1

typedef struct
{
    const int len;
    const char * const val;
} string;

typedef struct
{
    const char sep;
    const string str;
    int start;
    int has_more;
} tok_t;

string str_new(const char *);

string str_newn(const char*, int);

void str_destroy(string);

string str_empty();

int str_is_empty(string);

string str_nil();

int str_is_nil(string);

/*
 * Substring. str_sub(str, start, count)
 * start is inclusive
 * end is exclusive
 */
string str_sub(string, int, int);

string str_trim(string);

int str_eq(string, string);

tok_t str_tok_init(char, string);

string str_tok(tok_t*,int);

#endif