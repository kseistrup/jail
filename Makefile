# OH, HAI! :)

VERSION   := 1.8

CC        ?= gcc
CFLAGS    ?= -Wall -s -march=native -mtune=native -O2 -pipe -fstack-protector-strong

PREFIX    ?= /usr/local
SBINDIR   ?= $(DESTDIR)$(PREFIX)/sbin/
MANDIR    ?= $(DESTDIR)$(PREFIX)/share/man
CONFDIR   ?= $(DESTDIR)/etc/

SCDOC8    := $(wildcard man/*.8.md)
MAN8      := $(SCDOC8:.8.md=.8)
ZMAN8     := $(MAN8:.8=.8.gz)

SCDOC5    := $(wildcard man/*.5.md)
MAN5      := $(SCDOC5:.5.md=.5)
ZMAN5     := $(MAN5:.5=.5.gz)

SCDOCS    := $(SCDOC8) $(SCDOC5)
MANPAGES  := $(MAN8) $(MAN5)
ZMANPAGES := $(ZMAN8) $(ZMAN5)

SRC       := $(wildcard src/*.[ch])
BIN       := bin/icmplog bin/tcplog

COMMON    := src/global.h src/common.h src/common.c

ICMPCONF  := /etc/icmplog.conf
TCPCONF   := /etc/tcplog.conf
CONFIGS   := $(wildcard etc/*.conf)

CONFSTR   := -DICMPLOG_CONF='"$(ICMPCONF)"' -DTCPLOG_CONF='"$(TCPCONF)"'
VERSTR    := -DVERSION='"$(VERSION)"'

INSTALL   ?= install
STRIP     ?= strip
SCDOC     ?= scdoc
GZIP      ?= gzip

.PHONY: all
all: jail $(MANPAGES)

jail: $(BIN)
	$(STRIP) $(BIN)

install: $(BIN) $(CONFIGS) $(ZMANPAGES)
	@test -d $(SBINDIR) || mkdir -p $(SBINDIR)
	$(INSTALL) -s $(BIN) $(SBINDIR)
	@test -d $(CONFDIR) || mkdir -p $(CONFDIR)
	$(INSTALL) -m 0644 $(CONFIGS) $(CONFDIR)
	@test -d $(MANDIR)/man8 || mkdir $(MANDIR)/man8
	$(INSTALL) -m 0644 $(ZMAN8) $(MANDIR)/man8/
	@test -d $(MANDIR)/man5 || mkdir $(MANDIR)/man5
	$(INSTALL) -m 0644 $(ZMAN5) $(MANDIR)/man5/

src/common.o: $(COMMON)
	$(CC) $(CFLAGS) -c -o src/common.o src/common.c

bin/%: src/%.c src/%.h src/common.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(CONFSTR) $(VERSTR) -o $@ $< src/common.o

man/%.8: man/%.8.md
	$(SCDOC) <$< >$@

man/%.8.gz: man/%.8
	$(GZIP) -c $< >$@

man/%.5: man/%.5.md
	$(SCDOC) <$< >$@

man/%.5.gz: man/%.5
	$(GZIP) -c $< >$@

.PHONY: clean
clean:
	rm -f $(BIN)
	rm -f $(ZMANPAGES)
	rm -f src/*.o
	rm -f *~ */*~

# EOF
