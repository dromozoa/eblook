ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = eblook
info_TEXINFOS = eblook.texi

eblook_SOURCES = eblook.c codeconv.c getopt.c getopt1.c bmp2ppm.c bmp_in.c \
	win32.c
if USE_DROMOZOA
eblook_SOURCES += dromozoa.cpp
endif
eblook_LDADD = @LIBOBJS@
noinst_HEADERS = getopt.h codeconv.h

EXTRA_DIST = README.org
EXTRA_DIST += README.md dromozoa.h
# MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.h.in stamp-h.in \
#	configure missing mkinstalldirs install-sh texinfo.tex
