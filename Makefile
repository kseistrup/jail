# Copyright 2008-2020 Klaus Alexander Seistrup <klaus@seistrup.dk>
# Copyright 1998-2001 Abhijit Menon-Sen <ams@wiw.org>
#

CC      = musl-gcc
#CC      = gcc
CFLAGS  = -Wall -s -march=native -mtune=native -O2 -pipe -fstack-protector-strong
VERSION = 1.8
INSTALL = /usr/bin/install
STRIP   = /usr/bin/strip
DESTDIR = /usr/local
CONF    = $(CONFDIR)/etc
SBIN    = $(DESTDIR)/sbin
MANDIR  = $(DESTDIR)/share/man
DOCS    = README CREDITS TODO License INSTALL
MAN8    = icmplog.8 tcplog.8
MAN5    = icmplog.conf.5 tcplog.conf.5
SCONF   = icmplog.conf tcplog.conf
COMMON  = global.h common.h common.c
SRC     = icmplog.h tcplog.h icmplog.c tcplog.c Makefile
BIN     = icmplog tcplog
ICMCONF = $(CONF)/icmplog.conf
TCPCONF = $(CONF)/tcplog.conf
CONFSTR = -DICMPLOG_CONF='"$(ICMCONF)"' -DTCPLOG_CONF='"$(TCPCONF)"'
VERSTR  = -DVERSION='"$(VERSION)"'

jail: $(BIN)
	$(STRIP) $(BIN)

common.o: $(COMMON)
	$(CC) $(CFLAGS) -c -o common.o common.c

%: %.c %.h common.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(CONFSTR) $(VERSTR) -o $@ $< common.o

install: $(BIN) $(MAN) $(SCONF)
	$(INSTALL) -s $(BIN) $(SBIN)
	-$(INSTALL) -m0644 $(MAN8) $(MANDIR)/man8
	-$(INSTALL) -m0644 $(MAN5) $(MANDIR)/man5
	-$(INSTALL) -m0644 $(SCONF) $(CONF)

dist: jail
	-mkdir jail-$(VERSION)
	-cp $(DOCS) $(MAN8) $(MAN5) $(COMMON) $(SCONF) $(SRC) $(BIN) jail-$(VERSION)
	tar czf jail-$(VERSION).tar.gz jail-$(VERSION)/*
	rm -rf jail-$(VERSION)

.PHONY: clean

clean:
	-rm -f $(BIN) *.o core *~ jail-$(VERSION).tar.gz
