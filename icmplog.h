/*
 * $Id: icmplog.h,v 1.7 2001/02/19 10:06:17 ams Exp $
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include "common.h"

#include <netinet/ip_icmp.h>

#define MAX_ICMP 18

char *version = "icmplog " VERSION ", ams@wiw.org";
char *cfgfile = ICMPLOG_CONF;

/* Default log level */
short int lvl = LOG_WARNING;

/* Default logging facility */
short int fac = LOG_DAEMON;

/* ICMP packet */
typedef struct {
    struct iphdr ip;
    struct icmphdr icmp;
    char data[0];
} icmppkt;

/* Level at which each packet is logged */
short int levels[256];

typedef struct {
    char *name;
    int type;
} tname;

/* Configuration file type keywords */
tname names[] = { { "echoreply", 0 },
                  { "destunreach", 3 },
                  { "quench", 4 },
                  { "redirect", 5 },
                  { "echo", 8 },
                  { "timeout", 11 },
                  { "paramprob", 12 },
                  { "timereq", 13 },
                  { "timerep", 14 },
                  { "inforeq", 15 },
                  { "inforep", 16 }
                };


/* ICMP packet type names (RFC 792) */
char *types[256] = { "echo reply", NULL, NULL,
                     "destination unreachable",
                     "source quench",
                     "redirect", NULL, NULL,
                     "echo", NULL, NULL,
                     "time exceeded",
                     "parameter problem",
                     "timestamp request",
                     "timestamp reply",
                     "info request",
                     "info reply"
                   };
