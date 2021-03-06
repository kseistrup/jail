/*
 * Copyright 2008-2020 Klaus Alexander Seistrup <klaus@seistrup.dk>
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>

#include "global.h"

#ifndef NOFILE
#define NOFILE 1024
#endif

/* decide if a host is to be ignored or not */
int
excepted(unsigned long int addr, char *name)
{
    char *ip;
    struct ex *l = &root;
    struct in_addr i;

    i.s_addr = addr;
    ip = inet_ntoa(i);

    while (l->link != NULL)
      {
	  if (strspn(l->host, ".0123456789") == strlen(l->host))
	    {
		/* numeric fragment */
		if (strstr(ip, l->host) != NULL)
		  {
		      return 1;
		  }
	    }
	  else
	    {
		/* hostname fragment */
		if (strstr(name, l->host) != NULL)
		  {
		      return 1;
		  }
	    }
	  l = l->link;
      }
    return (0);
}

/* gethostbyaddr(3) wrapper */
char *
gethost(unsigned long int addr)
{
    extern int resolve_names;
    struct hostent *h;
    struct in_addr i;

    i.s_addr = addr;
    /* decide whether to call gethostbyaddr(3) or not */
    if (resolve_names)
      {
	  h = gethostbyaddr((char *) &i, sizeof(i), AF_INET);
	  if (h == NULL)
	    {
		return (inet_ntoa(i));
	    }
	  else
	    {
		return (h->h_name);
	    }
      }
    else
      {
	  return (inet_ntoa(i));
      }
}

/* get ip address */
char *
getip(unsigned long int addr)
{
    struct in_addr i;

    i.s_addr = addr;

    return inet_ntoa(i);
}

/* convert keyword to log level */
short int
loglevel(char *str)
{
    extern int lvl;

    if (str == NULL)
      {
	  return (lvl);
      }

    /* translate valid keywords */
    if (strcasecmp(str, "IGNORE") == 0)
      {
	  return (IGNORE);
      }
    if (strcasecmp(str, "ERR") == 0)
      {
	  return (LOG_ERR);
      }
    if (strcasecmp(str, "WARNING") == 0)
      {
	  return (LOG_WARNING);
      }
    if (strcasecmp(str, "NOTICE") == 0)
      {
	  return (LOG_NOTICE);
      }
    if (strcasecmp(str, "INFO") == 0)
      {
	  return (LOG_INFO);
      }
    if (strcasecmp(str, "DEBUG") == 0)
      {
	  return (LOG_DEBUG);
      }

    return (lvl);
}

/* convert keyword to syslog facility */
short int
facility(char *str)
{
    extern int fac;

    if (str == NULL)
      {
	  return (fac);
      }

    /* translate valid keywords */
    if (strcasecmp(str, "USER") == 0)
      {
	  return (LOG_USER);
      }
    if (strcasecmp(str, "DAEMON") == 0)
      {
	  return (LOG_DAEMON);
      }
    if (strcasecmp(str, "LOCAL0") == 0)
      {
	  return (LOG_LOCAL0);
      }
    if (strcasecmp(str, "LOCAL1") == 0)
      {
	  return (LOG_LOCAL1);
      }
    if (strcasecmp(str, "LOCAL2") == 0)
      {
	  return (LOG_LOCAL2);
      }
    if (strcasecmp(str, "LOCAL3") == 0)
      {
	  return (LOG_LOCAL3);
      }
    if (strcasecmp(str, "LOCAL4") == 0)
      {
	  return (LOG_LOCAL4);
      }
    if (strcasecmp(str, "LOCAL5") == 0)
      {
	  return (LOG_LOCAL5);
      }
    if (strcasecmp(str, "LOCAL6") == 0)
      {
	  return (LOG_LOCAL6);
      }
    if (strcasecmp(str, "LOCAL7") == 0)
      {
	  return (LOG_LOCAL7);
      }

    return (fac);
}
