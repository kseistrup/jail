/*
 * Copyright 2008-2020 Klaus Alexander Seistrup <klaus@seistrup.dk>
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
