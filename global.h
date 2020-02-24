/*
 * $Id: global.h,v 1.7 2001/02/19 10:06:17 ams Exp $
 * Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#include <unistd.h>

#define IGNORE 0

/* Linked list of hosts to ignore */
struct ex
{
    char *host;
    struct ex *link;
} root;
