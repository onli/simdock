PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin
APPDIR ?= $(PREFIX)/share/simdock
ICONDIR ?= $(PREFIX)/share/pixmaps
SRCDIR ?= src/

SHELL ?= /bin/sh
CC = g++
CCFLAGS ?= -O2
DEPS = $(shell pkg-config --libs --cflags glib-2.0 gconf-2.0 gtk+-3.0 libwnck-3.0 xcb-ewmh librsvg-2.0)
ifneq (, $(shell which wx-config))
	DEPS += $(shell wx-config --cflags --libs)
else
	DEPS += $(shell wx-config-gtk3 --cflags --libs)
endif

.PHONY: install uninstall

all:
	$(CC) $(CCFLAGS) $(SRCDIR)*.cc $(DEPS) $(LDLIBS) -o simdock

install: 
	@install -d "$(DESTDIR)$(BINDIR)" "$(DESTDIR)$(APPDIR)" "$(DESTDIR)$(ICONDIR)" "$(DESTDIR)$(PREFIX)/share/applications/" "$(DESTDIR)$(PREFIX)/share/icons/hicolor/256x256/apps/" "$(DESTDIR)$(PREFIX)/share/icons/hicolor/scalable/apps/"
	@install -m 0755 -v simdock "$(DESTDIR)$(BINDIR)/simdock"
	@install -m 0644 -v gfx/* "$(DESTDIR)$(APPDIR)/"
	@install -m 0644 -v gfx/simdock.png "$(DESTDIR)$(ICONDIR)/"
	@install -m 0644 -v simdock.desktop "$(DESTDIR)$(PREFIX)/share/applications/"
	@install -m 0644 -v gfx/simdock.png "$(DESTDIR)$(PREFIX)/share/icons/hicolor/256x256/apps/"
	@install -m 0644 -v gfx/simdock.svg "$(DESTDIR)$(PREFIX)/share/icons/hicolor/scalable/apps/"

uninstall:
	@rm -vf "$(DESTDIR)$(BINDIR)/simdock" "$(DESTDIR)$(APPDIR)/*" "$(DESTDIR)$(ICONDIR)/simdock.png"