PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin
APPDIR ?= $(PREFIX)/share/simdock
ICONDIR ?= $(PREFIX)/share/pixmaps
SRCDIR ?= src/

SHELL ?= /bin/sh
CC = g++
CCFLAGS ?= -march=native -O2 -flto
DEPS = $(shell pkg-config --cflags glib-2.0 gconf-2.0 gtk+-2.0 libwnck-1.0 xcb-ewmh)
DEPS += $(shell wx-config --cflags )
LDFLAGS = $(shell pkg-config --libs glib-2.0 gconf-2.0 gtk+-2.0 libwnck-1.0 xcb-ewmh)
LDFLAGS +=  $(shell wx-config --libs)
SRCS = $(SRCDIR)background.o $(SRCDIR)main_arguments.o $(SRCDIR)save_launchers.o $(SRCDIR)tasks.o $(SRCDIR)gconf_settings.o $(SRCDIR)settingsDialog.o $(SRCDIR)xstuff.o $(SRCDIR)interrupts.o $(SRCDIR)main_settings.o $(SRCDIR)sim_gconf.o $(SRCDIR)launcher_dialog.o $(SRCDIR)myFrame.o $(SRCDIR)simImage.o $(SRCDIR)main.o

.PHONY: install clean uninstall

all: simdock

simdock: $(SRCS)
	$(CC) $(CCFLAGS) $(SRCS) $(LDFLAGS) -I$(SRCDIR) -o simdock
	 
%.o: %.cc %.h
	$(CC) $(CCFLAGS) $< $(DEPS) -c -o $@

%.o: %.cc
	$(CC) $(CCFLAGS) $< $(DEPS) -c -o $@

install: 
	@install -d "$(DESTDIR)$(BINDIR)" "$(DESTDIR)$(APPDIR)" "$(DESTDIR)$(ICONDIR)"
	@install -m 0755 -v simdock "$(DESTDIR)$(BINDIR)/simdock"
	@install -m 0644 -v gfx/* "$(DESTDIR)$(APPDIR)/"
	@install -m 0644 -v gfx/simdock.png "$(DESTDIR)$(ICONDIR)/"

clean:
	-rm -v $(SRCS) simdock

uninstall:
	@rm -vf "$(DESTDIR)$(BINDIR)/simdock" "$(DESTDIR)$(APPDIR)/*" "$(DESTDIR)$(ICONDIR)/simdock.png"
