# Makefile for AllegroOGG using Ogg Vorbis (Xiph.org Foundation)

CC=gcc

TARGETFLAGS=-Wall -O2 -fomit-frame-pointer -finline-functions -ffast-math -Wno-pointer-sign -Wno-unused-variable -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-unused-function
OBJDIR=obj/linux/static
LIBDEST=lib/linux/libalogg.a

# setting object files, paths and compiler flags
vpath %.c decoder decoder/libvorbis/lib decoder/libogg/src
vpath %.c src
vpath %.o $(OBJDIR)

CFLAGS=$(TARGETFLAGS) -Idecoder -Iinclude -Idecoder/libvorbis/include -Idecoder/libogg/include
OBJECTS=analysis.o bitrate.o bitwise.o block.o codebook.o envelope.o floor0.o floor1.o framing.o info.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o sharedbook.o smallft.o synthesis.o vorbisfile.o window.o alogg.o
OBJECTS2=$(addprefix $(OBJDIR)/,$(OBJECTS))


# making of the library
$(LIBDEST): $(OBJECTS)
	ar rs $(LIBDEST) $(OBJECTS2)

# compiling of the library
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(OBJDIR)/$@

clean:
	rm -f $(OBJECTS2) $(LIBDEST)

install: $(LIBDEST)
	cp $(LIBDEST) ../../lib/
	cp include/*.h ../../include/
