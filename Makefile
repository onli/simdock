PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin
APPDIR ?= $(PREFIX)/share/simdock
ICONDIR ?= $(PREFIX)/share/pixmaps
SRCDIR ?= src/

SHELL ?= /bin/sh
CC = g++
CCFLAGS ?= -march=native -Ofast -flto
DEPS = $(shell pkg-config --libs --cflags glib-2.0 gconf-2.0 gtk+-2.0 libwnck-1.0 xcb-ewmh)
DEPS += $(shell wx-config --cflags --libs)
SRCS = $(SRCDIR)background.o $(SRCDIR)main_arguments.o $(SRCDIR)save_launchers.o $(SRCDIR)tasks.o $(SRCDIR)gconf_settings.o $(SRCDIR)settingsDialog.o $(SRCDIR)xstuff.o $(SRCDIR)interrupts.o $(SRCDIR)main_settings.o $(SRCDIR)sim_gconf.o $(SRCDIR)launcher_dialog.o $(SRCDIR)myFrame.o $(SRCDIR)simImage.o $(SRCDIR)main.o

.PHONY: install clean uninstall

all: simdock

simdock: $(SRCS)
	$(CC) $(CCFLAGS) $(SRCS) $(DEPS) $(LDLIBS) -I$(SRCDIR) -o simdock
	 
%.o: %.cc
	$(CC) $(CCFLAGS) $< $(DEPS) $(LDLIBS) -c -o $@

install: 
	@install -d "$(DESTDIR)$(BINDIR)" "$(DESTDIR)$(APPDIR)" "$(DESTDIR)$(ICONDIR)"
	@install -m 0755 -v simdock "$(DESTDIR)$(BINDIR)/simdock"
	@install -m 0644 -v gfx/* "$(DESTDIR)$(APPDIR)/"
	@install -m 0644 -v gfx/simdock.png "$(DESTDIR)$(ICONDIR)/"

clean:
	@rm -v $(SRCS) simdock

uninstall:
	@rm -vf "$(DESTDIR)$(BINDIR)/simdock" "$(DESTDIR)$(APPDIR)/*" "$(DESTDIR)$(ICONDIR)/simdock.png"
