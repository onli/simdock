SUBDIRS = po src

bin_PROGRAMS = simdock
#simdock_LDADD = @LIBS@ #$(WX_LIBS)
simdock_LDFLAGS = $(LDFLAGS)
simdock_LDADD = $(LIBS)

simdock_SOURCES = src/background.cc src/save_launchers.cc src/sim_gconf.cc src/xstuff.cc src/interrupts.cc src/main_arguments.cc src/main_settings.cc src/settingsDialog.cc src/tasks.cc src/gconf_settings.cc src/launcher_dialog.cc src/main.cc src/simImage.cc src/myFrame.cc

simdockBGSdir = /usr/share/simdock
simdockBGS_DATA= src/bg.png src/bg2.png src/bg5.png src/mark.png src/question.png

simdockDESKTOPdir = /usr/share/applications
simdockDESKTOP_DATA= simdock.desktop

simdockICONdir = /usr/share/pixmaps
simdockICON_DATA= simdock.png
