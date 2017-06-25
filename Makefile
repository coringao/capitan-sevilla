CC = g++
CFLAGS = -g3 -Iinclude -Isrc/include
LDFLAGS = -Llib -lalogg `allegro-config --libs` -lalfont -lpng -lz

PACKAGE = Capitan Sevilla - El Remake
VERSION = 1.0
EXE = capitan

SRCDIR = src
OBJDIR = $(SRCDIR)/obj
DESTDIR = /usr
GAMEDIR = $(DESTDIR)/games
STARTUP = $(GAMEDIR)/$(EXE)
INSTDIR = $(DESTDIR)/share/games/$(EXE)

DATADIR = data lang
EXTRADIR = extra
ICNDIR = $(DESTDIR)/share/pixmaps
APPDIR = $(DESTDIR)/share/applications
ICON = capitan.png
DESKTOP = capitan.desktop

MODULES = \
	$(SRCDIR)/actor.cpp		$(SRCDIR)/actormanager.cpp \
	$(SRCDIR)/background.cpp	$(SRCDIR)/creditos.cpp     \
	$(SRCDIR)/configuracion.cpp	$(SRCDIR)/filehandling.cpp \
	$(SRCDIR)/gestordemundos.cpp	$(SRCDIR)/guagua.cpp       \
	$(SRCDIR)/hab.cpp		$(SRCDIR)/hardware.cpp     \
	$(SRCDIR)/loadpng.cpp		$(SRCDIR)/logger.cpp       \
	$(SRCDIR)/main.cpp		$(SRCDIR)/marcadores.cpp   \
	$(SRCDIR)/messages.cpp		$(SRCDIR)/menu.cpp         \
	$(SRCDIR)/menuplayer.cpp	$(SRCDIR)/mundo.cpp        \
	$(SRCDIR)/panel.cpp		$(SRCDIR)/particula.cpp    \
	$(SRCDIR)/partida.cpp		$(SRCDIR)/presentacion.cpp \
	$(SRCDIR)/sistparticulas.cpp	$(SRCDIR)/sonido.cpp       \
	$(SRCDIR)/sprite.cpp		$(SRCDIR)/sprmanager.cpp

OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(patsubst %.cpp, %.o, $(MODULES))))

CP = cp -r
RM = rm -rf
MD = mkdir -p
ECHO = echo
CHMOD = chmod
STRIP = strip

all: $(EXE)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJECTS)
	$(CC) $^ -o $(EXE) $(LDFLAGS)
	$(STRIP) $@

clean:
	$(RM) $(OBJDIR)/*.o
	$(RM) $(EXE)

install: all
	@$(ECHO) " o Creating install directory $(INSTDIR)"
	@$(MD) "$(INSTDIR)"
	@$(ECHO) " o Installing game and data to $(INSTDIR)"
	@$(CP) $(EXE) $(DATADIR) "$(INSTDIR)"
	@$(ECHO) " o Creating startup script $(STARTUP)"
	@$(MD) "$(GAMEDIR)"
	@$(ECHO) "#!/bin/sh" >"$(STARTUP)"
	@$(ECHO) "pulseaudio -k" >>"$(STARTUP)"
	@$(ECHO) "cd \"$(INSTDIR)\"; ./$(EXE); cd -" >>"$(STARTUP)"
	@$(ECHO) "pulseaudio -D" >>"$(STARTUP)"
	@$(CHMOD) 755 "$(STARTUP)"
	@$(ECHO) " o Creating application menu entry"
	@$(MD) "$(ICNDIR)"
	@$(CP) "$(EXTRADIR)/$(ICON)" "$(ICNDIR)"
	@$(MD) "$(APPDIR)"
	@$(CP) "$(EXTRADIR)/$(DESKTOP)" "$(APPDIR)"
	@$(ECHO) ""

uninstall:
	@$(ECHO) " o Removing game and data from $(INSTDIR)"
	@$(RM) -r "$(INSTDIR)"
	@$(ECHO) " o Removing startup script $(STARTUP)"
	@$(RM) "$(STARTUP)"
	@$(ECHO) " o Removing application menu entry"
	@$(RM) "$(ICNDIR)/$(ICON)" "$(APPDIR)/$(DESKTOP)"

deps:
	mkdir -p dependencies/alfont/obj/linux/static
	mkdir -p dependencies/alfont/lib/linux
	mkdir -p dependencies/alogg/obj/linux/static
	mkdir -p dependencies/alogg/lib/linux
	mkdir -p src/obj
	mkdir -p lib/linux
	mkdir -p include
	cd dependencies/alfont && make install
	cd dependencies/alogg && make install

depsclean:
	cd dependencies/alfont && make clean
	cd dependencies/alogg && make clean
	$(RM) lib/*.a include/*.h
	$(RM) dependencies/alfont/obj
	$(RM) dependencies/alfont/lib
	$(RM) dependencies/alogg/obj
	$(RM) dependencies/alogg/lib
	$(RM) src/obj
	$(RM) include
	$(RM) lib

.PHONY: all clean install deps depsclean uninstall
