/*
 * $Id: icmplog.c,v 1.7 2001/02/19 10:06:17 ams Exp $
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include <string.h>
#include <strings.h>
#include <errno.h>
#include "icmplog.h"

#define ICMP_FILTER 1

/* return packet description */
char *
icmps(int type)
{
    static char msg[15];

    if (types[type] == (char *) NULL)
      {
	  snprintf(msg, 15, "ICMP type %d", type);
	  msg[14] = '\0';
	  return (msg);
      }
    else
      {
	  return (types[type]);
      }
}

/* parse configuration file */
void
configure(char *file)
{
    char buf[MAXLINE], *keyword, *name;
    FILE *rc;
    int type, level, i, j = 0;
    struct ex *curr = &root, *next = (struct ex *) NULL;

    /* use default configuration if none is specified */
    if (file == (char *) NULL)
      {
	  file = cfgfile;
      }

    if ((rc = fopen(file, "r")) == (FILE *) NULL)
      {
	  perror(file);
	  return;
      }

    while (fgets(buf, MAXLINE - 1, rc))
      {
	  /* ignore comments and blank lines */
	  if (buf[0] == '#' || buf[0] == '\n')
	    {
		continue;
	    }

	  keyword = strtok(buf, " \t");

	  /* default log level */
	  if (strcasecmp(keyword, "level") == 0)
	    {
		lvl = loglevel(strtok((char *) NULL, " \t\n"));
		continue;
	    }

	  /* logging facility */
	  if (strcasecmp(keyword, "facility") == 0)
	    {
		fac = facility(strtok((char *) NULL, " \t\n"));
		continue;
	    }

	  /* list of hosts to ignore */
	  if (strcasecmp(keyword, "except") == 0)
	    {
		while ((name =
			strtok((char *) NULL, " \t\n,")) != (char *) NULL)
		  {
		      /* add to list */
		      curr->host = (char *) malloc(strlen(name) + 1);
		      if (curr->host == (char *) NULL)
			{
			    fprintf(stderr, "malloc() failed.\n");
			    exit(-1);
			}
		      strcpy(curr->host, name);
		      next = (struct ex *) malloc(sizeof(struct ex));
		      if (next == (struct ex *) NULL)
			{
			    fprintf(stderr, "malloc() failed.\n");
			    exit(-1);
			}
		      curr->link = next;
		      curr = next;
		      curr->link = (struct ex *) NULL;
		  }
		continue;
	    }

	  /* numeric ICMP type */
	  if (isdigit(keyword[0]))
	    {
		type = strtol(keyword, (char **) NULL, 0);

		if (type < 0 || type > 255)
		  {
		      fprintf(stderr, "%d: invalid ICMP type.\n", type);
		      continue;
		  }

		level = loglevel(strtok((char *) NULL, " \t\n"));
		levels[type] = level;
	    }
	  /* type name */
	  else
	    {
		/* this is still just waiting for salvation */
		for (i = 0; i < 11; ++i)
		  {
		      if (strcasecmp(keyword, names[i].name) == 0)
			{
			    level = loglevel(strtok((char *) NULL, " \t\n"));
			    levels[names[i].type] = level;
			    j = 1;
			    break;
			}
		  }
		if (j == 0)
		  {
		      fprintf(stderr, "%s: unknown packet type.\n", keyword);
		  }
	    }
      }
}

int
main(int argc, char *argv[])
{
    char *rc = (char *) NULL, *host, *ip;
    int s = 0, c = 0, index = 0, type;
    icmppkt *pkt;
    unsigned int filter = 0;

    /* initialise logging level table */
    for (index = 0; index < 256; ++index)
      {
	  levels[index] = lvl;
      }

    /* process command line parameters */
    while (1)
      {
	  c = getopt_long(argc, argv, "f:nlv", par, &index);
	  if (c == -1)
	    {
		break;
	    }
	  switch (c)
	    {
	    case 0:
		switch (index)
		  {
		  case 0:
		      rc = optarg;
		      break;
		  case 1:
		      resolve_names = 0;
		      break;
		  case 2:
		      printf("%s\n", version);
		      exit(0);
		      break;
		  }
		break;
	    case 'f':
		rc = optarg;
		break;
	    case 'n':
		resolve_names = 0;
		break;
	    case 'v':
		printf("%s\n", version);
		exit(0);
		break;
	    default:
		printf("options: --file, --no-resolve, --version\n");
		exit(0);
	    }
      }

    /* we no longer need root to bind raw socket */
    if (geteuid() == 0)
      {
	  fprintf(stderr,
		  "%s: do not run as root, use capabilities instead.\n",
		  argv[0]);
	  exit(-1);
      }

    /* parse configuration */
    configure(rc);

    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
      {
	  perror("socket");
	  exit(1);
      }

    /* 2.1.x kernels mask some ICMP types by default */
    setsockopt(s, SOL_RAW, ICMP_FILTER, &filter, sizeof(filter));

    if ((pkt = (icmppkt *) malloc(MTU)) == (icmppkt *) NULL)
      {
	  fprintf(stderr, "malloc() failed\n");
	  exit(-1);
      }

    openlog("icmplog", 0, fac);
    syslog(LOG_INFO, "started");

    while (1)
      {
	  /* process packets */
	  if (read(s, pkt, MTU) == -1)
	    {
		perror("icmplog");
		exit(1);
	    }

	  host = gethost(pkt->ip.saddr);
	  type = pkt->icmp.type;

	  if (excepted(pkt->ip.saddr, host))
	    {
		continue;
	    }

	  if (pkt->ip.ihl != 5)
	    {
		/* pkt->ip.ihl < 5 should NEVER happen */
		continue;
	    }

	  if (levels[type])
	    {
		ip = getip(pkt->ip.saddr);
		if (strcmp(host, ip) == 0)
		    syslog(levels[type], "%s from unknown[%s]", icmps(type),
			   host);
		else
		    syslog(levels[type], "%s from %s[%s]", icmps(type), host,
			   ip);
	    }
      }
}
