/*
 * $Id: common.h,v 1.7 2001/02/19 10:06:17 ams Exp $
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include <ctype.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>
#include <sys/types.h>

#include "global.h"

/* Maximum line length in configuration file */
#define MAXLINE 80

#define MTU 4352

/* Command line parameters */
static struct option par[] = { {"file", 1, 0, 0},
{"no-resolve", 0, 0, 0},
{"version", 0, 0, 0},
{0, 0, 0, 0}
};

/* Convert IP addresses to hostnames? */
short int resolve_names = 1;

/* Functions defined in common.c */
extern int excepted(unsigned long int addr, char *str);
extern char *gethost(unsigned long int addr);
extern char *getip(unsigned long int addr);
extern short int loglevel(char *str);
extern short int facility(char *str);
