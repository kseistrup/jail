/*
 * $Id: tcplog.c,v 1.7 2001/02/19 10:06:17 ams Exp $
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include <string.h>
#include <strings.h>
#include <errno.h>
#include "tcplog.h"

/* getservbyport(3) wrapper */
char *
service(int port)
{
    struct servent *serv;
    static char name[64];

    serv = getservbyport(htons(port), "tcp");
    if (serv == (struct servent *) NULL)
      {
	  (void) snprintf(name, 64, "port %d (unknown)", port);
      }
    else
      {
	  (void) snprintf(name, 64, "port %d (%s)", port, serv->s_name);
      }
    return (name);
}

/* parse configuration file */
void
configure(char *file)
{
    char buf[MAXLINE], *keyword, *name, *map;
    FILE *rc;
    int port = 0, level;
    struct ex *curr = &root, *next = (struct ex *) NULL;
    struct servent *serv;

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

	  /* service name/port */
	  if (isdigit(keyword[0]))
	    {
		/* port number */
		port = strtol(keyword, (char **) NULL, 0);
		level = loglevel(strtok((char *) NULL, " \t\n"));
	    }
	  else
	    {
		/* service name */
		serv = getservbyname(keyword, "tcp");
		if (serv != (struct servent *) NULL)
		  {
		      port = ntohs(serv->s_port);
		      level = loglevel(strtok((char *) NULL, " \t\n"));
		  }
		else
		  {
		      fprintf(stderr, "%s: unknown service\n", keyword);
		      continue;
		  }
	    }
	  if (port)
	    {
		/* add to magical two-level 1024-way port tree */
		if (ports[port >> 6] == (char *) NULL)
		  {
		      map = (char *) malloc(64);
		      if (map == (char *) NULL)
			{
			    fprintf(stderr, "malloc() failed.\n");
			    exit(-1);
			}
		      memset(map, lvl, 64);
		      ports[port >> 6] = map;
		  }
		ports[port >> 6][port & 0x3f] = level;
	    }
      }
}

int
main(int argc, char *argv[])
{
    char *rc = (char *) NULL, *host, *ip;
    int s = 0, c = 0, index = 0, level = lvl, p;
    tcppkt *pkt;

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

    /* need root to bind raw socket */

    if (geteuid() == 0)
      {
	  fprintf(stderr,
		  "%s: do not run as root, use capabilities(7) instead.\n",
		  argv[0]);
	  exit(-1);
      }

    configure(rc);

    //background();

    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
      {
	  perror("socket");
	  exit(1);
      }

    if ((pkt = (tcppkt *) malloc(MTU)) == (tcppkt *) NULL)
      {
	  fprintf(stderr, "malloc() failed\n");
	  exit(-1);
      }

    openlog("tcplog", 0, fac);
    syslog(LOG_INFO, "started");
    //drop_privs ("nobody");

    while (1)
      {
	  /* process packets */
	  if (read(s, pkt, MTU) == -1)
	    {
		perror("tcplog");
		exit(1);
	    }

	  /* only log connection requests */
	  if (pkt->tcp.syn && !pkt->tcp.ack)
	    {

		host = gethost(pkt->ip.saddr);

		if (excepted(pkt->ip.saddr, host))
		  {
		      continue;
		  }

		if (pkt->ip.ihl != 5)
		  {
		      /* pkt->ip.ihl < 5 should NEVER happen */
		      continue;
		  }

		p = ntohs(pkt->tcp.dest);

		/* find log level from magical tree */
		level = ports[p >> 6] == (char *) NULL ?
		    lvl : ports[p >> 6][p & 0x3f];

		if (ntohs(pkt->tcp.source) == 20 && p < 1024)
		  {
		      syslog(LOG_WARNING, "FTPbounce (%s) from %s",
			     service(p), host);
		      continue;
		  }

		if (level)
		  {
		      ip = getip(pkt->ip.saddr);
		      if (strcmp(host, ip) == 0)
			  syslog(level, "%s request from unknown[%s]",
				 service(p), host);
		      else
			  syslog(level, "%s request from %s[%s]", service(p),
				 host, ip);
		  }
	    }
      }
}
