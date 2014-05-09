PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin
APPDIR ?= $(PREFIX)/share/simdock
ICONDIR ?= $(PREFIX)/share/pixmaps
SRCDIR ?= src/

SHELL ?= /bin/sh
CC = g++
CCFLAGS ?= -O2
LDLIBS ?= -lX11
DEPS = $(shell pkg-config --libs --cflags glib-2.0 gconf-2.0 gtk+-2.0 libwnck-1.0)
DEPS += $(shell wx-config --cflags --libs)

.PHONY: install uninstall

all:
	$(CC) $(CCFLAGS) $(SRCDIR)*.cc $(DEPS) $(LDLIBS) -o simdock

install: 
	@install -d "$(DESTDIR)$(BINDIR)" "$(DESTDIR)$(APPDIR)" "$(DESTDIR)$(ICONDIR)"
	@install -m 0755 -v simdock "$(DESTDIR)$(BINDIR)/simdock"
	@install -m 0644 -v gfx/* "$(DESTDIR)$(APPDIR)/"
	@install -m 0644 -v gfx/simdock.png "$(DESTDIR)$(ICONDIR)/"

uninstall:
	@rm -vf "$(DESTDIR)$(BINDIR)/simdock" "$(DESTDIR)$(APPDIR)/*" "$(DESTDIR)$(ICONDIR)/simdock.png"
