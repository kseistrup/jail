/*
 * $Id: tcplog.h,v 1.7 2001/02/19 10:06:17 ams Exp $
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include "common.h"

#if defined(__GLIBC__) && (__GLIBC__ >= 2)
#include <netinet/tcp.h>
#else
#include <linux/tcp.h>
#endif

char *version = "tcplog " VERSION ", ams@wiw.org";
char *cfgfile = TCPLOG_CONF;

/* Default log level */
short int lvl = LOG_WARNING;

/* Default logging facility */
short int fac = LOG_DAEMON;

/* TCP packet */
typedef struct {
    struct iphdr ip;
    struct tcphdr tcp;
    char data[0];
} tcppkt;

/* Service list */
char *ports[1024];
