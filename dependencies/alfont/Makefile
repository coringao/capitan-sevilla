# Makefile for AllegroFont using FreeType 2

CC=gcc

TARGETFLAGS=-Wall -O2 -fomit-frame-pointer -finline-functions -ffast-math -Wno-pointer-sign -Wno-unused-variable -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-unused-function
OBJDIR=obj/linux/static
LIBDEST=lib/linux/libalfont.a

# setting object files, paths and compiler flags
vpath %.c freetype/src/autohint
vpath %.c freetype/src/base
vpath %.c freetype/src/cache
vpath %.c freetype/src/cff
vpath %.c freetype/src/cid
vpath %.c freetype/src/pcf
vpath %.c freetype/src/psaux
vpath %.c freetype/src/psnames
vpath %.c freetype/src/pshinter
vpath %.c freetype/src/raster
vpath %.c freetype/src/sfnt
vpath %.c freetype/src/smooth
vpath %.c freetype/src/truetype
vpath %.c freetype/src/type1
vpath %.c freetype/src/type42
vpath %.c freetype/src/gzip
vpath %.c freetype/src/bdf
vpath %.c freetype/src/pfr
vpath %.c freetype/src/winfonts
vpath %.c src
vpath %.o $(OBJDIR)

CFLAGS=$(TARGETFLAGS) -Ifreetype/include -Iinclude

OBJECTS=alfont.o ftsystem.o ftdebug.o ftinit.o ftbase.o ftglyph.o ftmm.o autohint.o ftcache.o cff.o type1cid.o pcf.o psaux.o pshinter.o psmodule.o raster.o sfnt.o smooth.o truetype.o type1.o winfnt.o type42.o ftgzip.o pfr.o bdf.o
OBJECTS2=$(addprefix $(OBJDIR)/,$(OBJECTS))


$(LIBDEST): $(OBJECTS)
	ar rs $(LIBDEST) $(OBJECTS2)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(OBJDIR)/$@

clean:
	rm -f $(OBJECTS2) $(LIBDEST)

install: $(LIBDEST)
	cp $(LIBDEST) ../../lib/
	cp include/*.h ../../include/

