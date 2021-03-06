/*
 * Copyright 2008-2020 Klaus Alexander Seistrup <klaus@seistrup.dk>
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include "common.h"

#if defined(__GLIBC__) && (__GLIBC__ >= 2)
#include <netinet/tcp.h>
#else
#include <linux/tcp.h>
#endif

char *version = "tcplog/" VERSION;
char *cfgfile = TCPLOG_CONF;

/* Default log level */
short int lvl = LOG_WARNING;

/* Default logging facility */
short int fac = LOG_DAEMON;

/* TCP packet */
typedef struct
{
    struct iphdr ip;
    struct tcphdr tcp;
    char data[0];
} tcppkt;

/* Service list */
char *ports[1024];
