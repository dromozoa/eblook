dnl *
dnl * Copyright (c) 2020 Kazuhiro Ito
dnl * 
dnl * This file is derived from m4/gettext.m4 in EB Library.
dnl * Below is original comment.
dnl * 
dnl * Copyright (c) 2004-2006  Motoyuki Kasahara
dnl *
dnl * Redistribution and use in source and binary forms, with or without
dnl * modification, are permitted provided that the following conditions
dnl * are met:
dnl * 1. Redistributions of source code must retain the above copyright
dnl *    notice, this list of conditions and the following disclaimer.
dnl * 2. Redistributions in binary form must reproduce the above copyright
dnl *    notice, this list of conditions and the following disclaimer in the
dnl *    documentation and/or other materials provided with the distribution.
dnl * 3. Neither the name of the project nor the names of its contributors
dnl *    may be used to endorse or promote products derived from this software
dnl *    without specific prior written permission.
dnl * 
dnl * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
dnl * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
dnl * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
dnl * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
dnl * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
dnl * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
dnl * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
dnl * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
dnl * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
dnl * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
dnl * SUCH DAMAGE.
dnl *

AC_DEFUN([eb_ICONV], [dnl
  INTLINCS=
  INTLDEPS=
  INTLLIBS=

  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PROG_LIBTOOL])

  dnl * 
  dnl * --enable-ebu option
  dnl * 
  AC_ARG_ENABLE(iconv,
    AC_HELP_STRING([--enable-iconv], [Build with iconv functionality [[auto]]]),
    ENABLE_ICONV=$enableval, ENABLE_ICONV=auto)

  if test $ENABLE_ICONV != no; then
    dnl * 
    dnl * --with-iconv-includes option
    dnl *
    AC_ARG_WITH(iconv-includes,
      AC_HELP_STRING([--with-iconv-includes=DIR],
        [iconv include files are in DIR]),
        [iconv_includes="-I${withval}"], [iconv_includes=''])

    dnl * 
    dnl * --with-iconv-libraries option
    dnl * 
    AC_ARG_WITH(iconv-libraries,
      AC_HELP_STRING([--with-iconv-libraries=DIR],
        [iconv library files are in DIR]),
      [iconv_libraries="-L${withval}"], [iconv_libraries=''])

    dnl *
    dnl * Check iconv(), iconv.h and -liconv.
    dnl *
    ICONVINCS=
    ICONVLIBS=
    save_CPPFLAGS=$CPPFLAGS
    save_LIBS=$LIBS
    CPPFLAGS="$save_CPPFLAGS $iconv_includes"
    LIBS="$save_LIBS $iconv_libraries"
    AC_CHECK_LIB(iconv, iconv_open,
      [ICONVLIBS="$iconv_libraries -liconv"; LIBS="$LIBS -liconv"],
      AC_CHECK_LIB(iconv, libiconv_open,
      [ICONVLIBS="$iconv_libraries -liconv"; LIBS="$LIBS -liconv"]))
    AC_CHECK_FUNCS(iconv_open libiconv_open locale_charset)
    AC_CHECK_HEADERS(iconv.h libcharset.h)
    if test $ac_cv_func_iconv_open != no; then
      ICONVINCS="$iconv_includes"
    fi
    CPPFLAGS=$save_CPPFLAGS
    LIBS=$save_LIBS
    AC_SUBST(ICONVINCS)
    AC_SUBST(ICONVLIBS)

    if test $ac_cv_func_iconv_open != no; then
      ENABLE_ICONV=yes
    elif test $ac_cv_func_libiconv_open != no; then
      ENABLE_ICONV=yes
    fi

    if test $ac_cv_func_iconv_open = no; then
      if test $ac_cv_func_libiconv_open = no; then
        if test $ENABLE_ICONV = yes; then
          AC_MSG_ERROR(ICONV functionality is requested but not available)
        else
          ENABLE_ICONV=no
        fi
      fi
    fi
   
    if test $ENABLE_ICONV = yes; then
      AC_DEFINE(ENABLE_ICONV, 1, [Define if iconv functionality is enabled])
    fi
  fi
])
