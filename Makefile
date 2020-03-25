# Makefile.in generated by automake 1.16.1 from Makefile.am.
# Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2018 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



# Makefile.am -- Template for generating Makefile via Automake.
# Copyright (C) 2006-2018 PUC-Rio/Laboratorio TeleMidia
#
# This file is part of Ginga (Ginga-NCL).
#
# Ginga is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Ginga is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
# License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Ginga.  If not, see <https://www.gnu.org/licenses/>.

# Makefile.am.common -- Common declarations.
# Copyright (C) 2013-2016 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# Makefile.am.coverage -- Generates code coverage report.
# Copyright (C) 2013-2018 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# doc.mk -- Build documentation.
# Copyright (C) 2006-2018 PUC-Rio/Laboratorio TeleMidia
#
# This file is part of Ginga (Ginga-NCL).
#
# Ginga is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Ginga is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
# License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Ginga.  If not, see <https://www.gnu.org/licenses/>.

am__is_gnu_make = { \
  if test -z '$(MAKELEVEL)'; then \
    false; \
  elif test -n '$(MAKE_HOST)'; then \
    true; \
  elif test -n '$(MAKE_VERSION)' && test -n '$(CURDIR)'; then \
    true; \
  else \
    false; \
  fi; \
}
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/ginga
pkgincludedir = $(includedir)/ginga
pkglibdir = $(libdir)/ginga
pkglibexecdir = $(libexecdir)/ginga
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = x86_64-pc-linux-gnu
host_triplet = x86_64-pc-linux-gnu
am__append_1 = README
am__append_2 = README
subdir = .
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps =  \
	$(top_srcdir)/build-aux/ax_cxx_compile_stdcxx_11.m4 \
	$(top_srcdir)/build-aux/libtool.m4 \
	$(top_srcdir)/build-aux/ltoptions.m4 \
	$(top_srcdir)/build-aux/ltsugar.m4 \
	$(top_srcdir)/build-aux/ltversion.m4 \
	$(top_srcdir)/build-aux/lt~obsolete.m4 \
	$(top_srcdir)/build-aux/manywarnings.m4 \
	$(top_srcdir)/build-aux/perl.m4 \
	$(top_srcdir)/build-aux/util.m4 \
	$(top_srcdir)/build-aux/warnings.m4 $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
DIST_COMMON = $(srcdir)/Makefile.am $(top_srcdir)/configure \
	$(am__configure_deps) $(am__DIST_COMMON)
am__CONFIG_DISTCLEAN_FILES = config.status config.cache config.log \
 configure.lineno config.status.lineno
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/lib/config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
SOURCES =
DIST_SOURCES =
RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \
	ctags-recursive dvi-recursive html-recursive info-recursive \
	install-data-recursive install-dvi-recursive \
	install-exec-recursive install-html-recursive \
	install-info-recursive install-pdf-recursive \
	install-ps-recursive install-recursive installcheck-recursive \
	installdirs-recursive pdf-recursive ps-recursive \
	tags-recursive uninstall-recursive
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
am__recursive_targets = \
  $(RECURSIVE_TARGETS) \
  $(RECURSIVE_CLEAN_TARGETS) \
  $(am__extra_recursive_targets)
AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \
	cscope distdir distdir-am dist dist-all distcheck
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
# Read a list of newline-separated strings from the standard input,
# and print each of them once, without duplicates.  Input order is
# *not* preserved.
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
# Make sure the list of sources is unique.  This is necessary because,
# e.g., the same source file might be shared among _SOURCES variables
# for different programs/libraries.
am__define_uniq_tagged_files = \
  list='$(am__tagged_files)'; \
  unique=`for i in $$list; do \
    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
  done | $(am__uniquify_input)`
ETAGS = etags
CTAGS = ctags
CSCOPE = cscope
DIST_SUBDIRS = $(SUBDIRS)
am__DIST_COMMON = $(srcdir)/Makefile.in $(srcdir)/doc/doc.mk \
	$(top_srcdir)/build-aux/Makefile.am.common \
	$(top_srcdir)/build-aux/Makefile.am.coverage \
	$(top_srcdir)/build-aux/ar-lib $(top_srcdir)/build-aux/compile \
	$(top_srcdir)/build-aux/config.guess \
	$(top_srcdir)/build-aux/config.sub \
	$(top_srcdir)/build-aux/git-version-gen \
	$(top_srcdir)/build-aux/install-sh \
	$(top_srcdir)/build-aux/ltmain.sh \
	$(top_srcdir)/build-aux/manywarnings.m4 \
	$(top_srcdir)/build-aux/missing \
	$(top_srcdir)/build-aux/perl.m4 \
	$(top_srcdir)/build-aux/warnings.m4 AUTHORS COPYING ChangeLog \
	INSTALL NEWS README build-aux/ar-lib build-aux/compile \
	build-aux/config.guess build-aux/config.sub \
	build-aux/install-sh build-aux/ltmain.sh build-aux/missing
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)
am__remove_distdir = \
  if test -d "$(distdir)"; then \
    find "$(distdir)" -type d ! -perm -200 -exec chmod u+w {} ';' \
      && rm -rf "$(distdir)" \
      || { sleep 5 && rm -rf "$(distdir)"; }; \
  else :; fi
am__post_remove_distdir = $(am__remove_distdir)
am__relativize = \
  dir0=`pwd`; \
  sed_first='s,^\([^/]*\)/.*$$,\1,'; \
  sed_rest='s,^[^/]*/*,,'; \
  sed_last='s,^.*/\([^/]*\)$$,\1,'; \
  sed_butlast='s,/*[^/]*$$,,'; \
  while test -n "$$dir1"; do \
    first=`echo "$$dir1" | sed -e "$$sed_first"`; \
    if test "$$first" != "."; then \
      if test "$$first" = ".."; then \
        dir2=`echo "$$dir0" | sed -e "$$sed_last"`/"$$dir2"; \
        dir0=`echo "$$dir0" | sed -e "$$sed_butlast"`; \
      else \
        first2=`echo "$$dir2" | sed -e "$$sed_first"`; \
        if test "$$first2" = "$$first"; then \
          dir2=`echo "$$dir2" | sed -e "$$sed_rest"`; \
        else \
          dir2="../$$dir2"; \
        fi; \
        dir0="$$dir0"/"$$first"; \
      fi; \
    fi; \
    dir1=`echo "$$dir1" | sed -e "$$sed_rest"`; \
  done; \
  reldir="$$dir2"
DIST_ARCHIVES = $(distdir).tar.gz
GZIP_ENV = --best
DIST_TARGETS = dist-gzip
distuninstallcheck_listfiles = find . -type f -print
am__distuninstallcheck_listfiles = $(distuninstallcheck_listfiles) \
  | sed 's|^\./|$(prefix)/|' | grep -v '$(infodir)/dir$$'
distcleancheck_listfiles = find . -type f -print
ACLOCAL = ${SHELL} /home/marina/Documentos/git/mulsemediaformatter/build-aux/missing aclocal-1.16
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 0
AR = ar
AS = as
AUTOCONF = ${SHELL} /home/marina/Documentos/git/mulsemediaformatter/build-aux/missing autoconf
AUTOHEADER = ${SHELL} /home/marina/Documentos/git/mulsemediaformatter/build-aux/missing autoheader
AUTOMAKE = ${SHELL} /home/marina/Documentos/git/mulsemediaformatter/build-aux/missing automake-1.16
AWK = mawk
CAIRO_CFLAGS = -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16
CAIRO_LIBS = -lcairo
CAIRO_REQUIRED_VERSION = 1.10.2
CC = gcc
CCDEPMODE = depmode=gcc3
CEF_CFLAGS = 
CEF_LIBS = 
CEF_REQUIRED_VERSION = 3.0
CFLAGS = -g -O2
CPP = gcc -E
CPPFLAGS = 
CXX = g++
CXXCPP = g++ -E
CXXDEPMODE = depmode=gcc3
CXXFLAGS = -g -O2
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DLLTOOL = false
DSYMUTIL = 
DUMPBIN = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /bin/grep -E
EXEEXT = 
FGREP = /bin/grep -F
GENHTML = 
GINGA_ALL_CXXFLAGS =     -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16  -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/orc-0.4 -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -pthread -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/libdrm -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -pthread -I/usr/include/librsvg-2.0 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/libxml2 -I/usr/include/lua5.3   -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -pthread -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GINGA_ALL_LDFLAGS =  -lcairo  -lglib-2.0 -lgstvideo-1.0 -lgstaudio-1.0 -lgstapp-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lrsvg-2 -lm -lgio-2.0 -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0 -lcairo -lxml2 -llua5.3 -export-dynamic   -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lcairo -lsoup-2.4 -lgio-2.0 -lgobject-2.0 -lglib-2.0
GINGA_LIBTOOL_AGE = 0
GINGA_LIBTOOL_CURRENT = 0
GINGA_LIBTOOL_CURRENT_MINUS_AGE = 0
GINGA_LIBTOOL_REVISION = 0
GINGA_LIBTOOL_STRING = 0:0:0
GINGA_VERSION_MAJOR = 0
GINGA_VERSION_MICRO = 0
GINGA_VERSION_MINOR = 0
GINGA_VERSION_STRING = UNKNOWN
GLIB_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GLIB_LIBS = -lglib-2.0
GLIB_REQUIRED_VERSION = 2.32.4
GREP = /bin/grep
GSTREAMER_CFLAGS = -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/orc-0.4 -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GSTREAMER_LIBS = -lgstvideo-1.0 -lgstaudio-1.0 -lgstapp-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GSTREAMER_REQUIRED_VERSION = 1.2.0
GTK_CFLAGS = -pthread -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/libdrm -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GTK_LIBS = -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
GTK_REQUIRED_VERSION = 3.4.2
HAVE_CXX11 = 
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
LCOV = 
LD = /usr/bin/ld -m elf_x86_64
LDFLAGS = 
LIBM = -lm
LIBOBJS = 
LIBRSVG_CFLAGS = -pthread -I/usr/include/librsvg-2.0 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16
LIBRSVG_LIBS = -lrsvg-2 -lm -lgio-2.0 -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0 -lcairo
LIBRSVG_REQUIRED_VERSION = 2.40.0
LIBS = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBXML_CFLAGS = -I/usr/include/libxml2
LIBXML_LIBS = -lxml2
LIBXML_REQUIRED_VERSION = 2.9
LIB_EXE_MACHINE_FLAG = 
LIPO = 
LN_S = ln -s
LTLIBOBJS = 
LT_MODULE_LDFLAGS = -shared -avoid-version
LT_SYS_LIBRARY_PATH = 
LUA_CFLAGS = -I/usr/include/lua5.3
LUA_LIBS = -llua5.3 -export-dynamic
LUA_REQUIRED_VERSION = 3.0
LUA_VERSION = 5.3
M4 = m4
MAKEINFO = ${SHELL} /home/marina/Documentos/git/mulsemediaformatter/build-aux/missing makeinfo
MANIFEST_TOOL = :
MINGW_ROOT = 
MKDIR_P = /bin/mkdir -p
NCLUA_CFLAGS = 
NCLUA_LIBS = 
NCLUA_REQUIRED_VERSION = 1.0
NM = /usr/bin/nm -B
NMEDIT = 
OBJDUMP = objdump
OBJEXT = o
OPENGL_CFLAGS = 
OPENGL_LIBS = 
OPENGL_REQUIRED_VERSION = 17.0
OTOOL = 
OTOOL64 = 
PACKAGE = ginga
PACKAGE_BUGREPORT = bugs@telemidia.puc-rio.br
PACKAGE_NAME = Ginga
PACKAGE_STRING = Ginga UNKNOWN
PACKAGE_TARNAME = ginga
PACKAGE_URL = http://github.com/TeleMidia/ginga
PACKAGE_VERSION = UNKNOWN
PANGO_CFLAGS = -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16
PANGO_LIBS = -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lcairo
PANGO_REQUIRED_VERSION = 1.30.0
PATH_SEPARATOR = :
PERL = perl
PKG_CONFIG = /usr/bin/pkg-config
PKG_CONFIG_LIBDIR = 
PKG_CONFIG_PATH = 
RANLIB = ranlib
SED = /bin/sed
SET_MAKE = 
SHELL = /bin/bash
SHTOOL = 
SOEXT = .so
SOUP_CFLAGS = -pthread -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
SOUP_LIBS = -lsoup-2.4 -lgio-2.0 -lgobject-2.0 -lglib-2.0
SOUP_REQUIRED_VERSION = 2.42
STRIP = strip
VALGRIND = 
VERSION = UNKNOWN
WARN_CFLAGS = 
WERROR_CFLAGS = 
WINDRES = 
abs_builddir = /home/marina/Documentos/git/mulsemediaformatter
abs_srcdir = /home/marina/Documentos/git/mulsemediaformatter
abs_top_builddir = /home/marina/Documentos/git/mulsemediaformatter
abs_top_srcdir = /home/marina/Documentos/git/mulsemediaformatter
ac_ct_AR = ar
ac_ct_CC = gcc
ac_ct_CXX = g++
ac_ct_DUMPBIN = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = $${TAR-tar} chof - "$$tardir"
am__untar = $${TAR-tar} xf -
bindir = ${exec_prefix}/bin
build = x86_64-pc-linux-gnu
build_alias = 
build_cpu = x86_64
build_os = linux-gnu
build_vendor = pc
builddir = .
cef_dir = 
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = ${prefix}
gingadatadir = ${datarootdir}/ginga
gingasysconfdir = ${prefix}/etc/ginga
host = x86_64-pc-linux-gnu
host_alias = 
host_cpu = x86_64
host_os = linux-gnu
host_vendor = pc
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /home/marina/Documentos/git/mulsemediaformatter/build-aux/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
luadatadir = ${datadir}/lua/5.3
lualibdir = ${libdir}/lua/5.3
mandir = ${datarootdir}/man
mkdir_p = $(MKDIR_P)
ms_lib = 
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /usr/local
program_transform_name = s,x,x,
psdir = ${docdir}
runstatedir = ${localstatedir}/run
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target_alias = 
top_build_prefix = 
top_builddir = .
top_srcdir = .
CLEANDIRS = doc/html
CLEANFILES = *.core *.gcda *.gcno vgcore.* doc/Doxyfile \
	$(am__append_1)
DISTCLEANFILES = 
EXTRA_DIST = doc/Doxyfile.in doc/arch.png doc/arch.tex $(am__append_2)
MAINTAINERCLEANFILES = Makefile.in INSTALL ChangeLog Makefile.in \
	aclocal.m4 build-aux/ar-lib build-aux/compile \
	build-aux/config.guess build-aux/config.sub build-aux/depcomp \
	build-aux/install-sh build-aux/libtool.m4 build-aux/ltmain.sh \
	build-aux/ltoptions.m4 build-aux/ltsugar.m4 \
	build-aux/ltversion.m4 build-aux/lt~obsolete.m4 \
	build-aux/missing build-aux/test-driver configure $(NULL)
ACLOCAL_AMFLAGS = -I build-aux ${ACLOCAL_FLAGS}
SUBDIRS = lib src src-gui tests-ncl tests
LCOV_DIR = coverage
LCOV_INFO = coverage.info

# Setup code coverage.
COVERAGE_EXCLUDE =
lcov_capture_options = --capture --directory $(top_builddir)\
  --output-file $(LCOV_INFO) --test-name '$(PACKAGE_NAME)_PERF'\
  --no-checksum --compat-libtool --ignore-errors source

lcov_remove_options = --output-file $(LCOV_INFO)\
  --remove $(LCOV_INFO) $(COVERAGE_EXCLUDE)

lcov_genhtml_options = --title '$(PACKAGE_NAME) Code Coverage' --quiet\
  --prefix $(top_builddir) --output-directory $(LCOV_DIR) --legend\
  --frames --show-details $(LCOV_INFO) --ignore-errors source

COVERAGE_TARGETS = coverage-reset coverage-genhtml coverage coverage-clean
DOXYGEN = doxygen
edit = sed -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' \
	  -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' \
	  -e 's,@PERL\@,$(PERL),g' \
	  -e 's,@top_builddir\@,$(top_builddir),g' \
	  -e 's,@top_srcdir\@,$(top_srcdir),g'


# Generate README from README.md if we have Perl.
BUILT_SOURCES = README
perl_strip_md_id_or_path_re = [\w\.\-/:]
perl_strip_md = \
  BEGIN { $$n = 0; }\
  s, , ,g;\
  s,`($(perl_strip_md_id_or_path_re)+)`,$$1,g;\
  s,^!.*$$,,g;\
  s,\[[^\]]+]\(($(perl_strip_md_id_or_path_re)+)\),$$1,g;\
  $$n >= 2 and print;\
  $$n++;\
  $(NULL)

all: $(BUILT_SOURCES)
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

.SUFFIXES:
am--refresh: Makefile
	@:
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am $(top_srcdir)/build-aux/Makefile.am.common $(top_srcdir)/build-aux/Makefile.am.coverage $(srcdir)/doc/doc.mk $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      echo ' cd $(srcdir) && $(AUTOMAKE) --gnu'; \
	      $(am__cd) $(srcdir) && $(AUTOMAKE) --gnu \
		&& exit 0; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    echo ' $(SHELL) ./config.status'; \
	    $(SHELL) ./config.status;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $@ $(am__maybe_remake_depfiles)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $@ $(am__maybe_remake_depfiles);; \
	esac;
$(top_srcdir)/build-aux/Makefile.am.common $(top_srcdir)/build-aux/Makefile.am.coverage $(srcdir)/doc/doc.mk $(am__empty):

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck

$(top_srcdir)/configure:  $(am__configure_deps)
	$(am__cd) $(srcdir) && $(AUTOCONF)
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	$(am__cd) $(srcdir) && $(ACLOCAL) $(ACLOCAL_AMFLAGS)
$(am__aclocal_m4_deps):

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool config.lt

# This directory's subdirectories are mostly independent; you can cd
# into them and run 'make' without going through this Makefile.
# To change the values of 'make' variables: instead of editing Makefiles,
# (1) if the variable is set in 'config.status', edit 'config.status'
#     (which will cause the Makefiles to be regenerated when you run 'make');
# (2) otherwise, pass the desired values on the 'make' command line.
$(am__recursive_targets):
	@fail=; \
	if $(am__make_keepgoing); then \
	  failcom='fail=yes'; \
	else \
	  failcom='exit 1'; \
	fi; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	  || eval $$failcom; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

ID: $(am__tagged_files)
	$(am__define_uniq_tagged_files); mkid -fID $$unique
tags: tags-recursive
TAGS: tags

tags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	set x; \
	here=`pwd`; \
	if ($(ETAGS) --etags-include --version) >/dev/null 2>&1; then \
	  include_option=--etags-include; \
	  empty_fix=.; \
	else \
	  include_option=--include; \
	  empty_fix=; \
	fi; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test ! -f $$subdir/TAGS || \
	      set "$$@" "$$include_option=$$here/$$subdir/TAGS"; \
	  fi; \
	done; \
	$(am__define_uniq_tagged_files); \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: ctags-recursive

CTAGS: ctags
ctags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	$(am__define_uniq_tagged_files); \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"
cscope: cscope.files
	test ! -s cscope.files \
	  || $(CSCOPE) -b -q $(AM_CSCOPEFLAGS) $(CSCOPEFLAGS) -i cscope.files $(CSCOPE_ARGS)
clean-cscope:
	-rm -f cscope.files
cscope.files: clean-cscope cscopelist
cscopelist: cscopelist-recursive

cscopelist-am: $(am__tagged_files)
	list='$(am__tagged_files)'; \
	case "$(srcdir)" in \
	  [\\/]* | ?:[\\/]*) sdir="$(srcdir)" ;; \
	  *) sdir=$(subdir)/$(srcdir) ;; \
	esac; \
	for i in $$list; do \
	  if test -f "$$i"; then \
	    echo "$(subdir)/$$i"; \
	  else \
	    echo "$$sdir/$$i"; \
	  fi; \
	done >> $(top_builddir)/cscope.files

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags
	-rm -f cscope.out cscope.in.out cscope.po.out cscope.files

distdir: $(BUILT_SOURCES)
	$(MAKE) $(AM_MAKEFLAGS) distdir-am

distdir-am: $(DISTFILES)
	$(am__remove_distdir)
	test -d "$(distdir)" || mkdir "$(distdir)"
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
	@list='$(DIST_SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    $(am__make_dryrun) \
	      || test -d "$(distdir)/$$subdir" \
	      || $(MKDIR_P) "$(distdir)/$$subdir" \
	      || exit 1; \
	    dir1=$$subdir; dir2="$(distdir)/$$subdir"; \
	    $(am__relativize); \
	    new_distdir=$$reldir; \
	    dir1=$$subdir; dir2="$(top_distdir)"; \
	    $(am__relativize); \
	    new_top_distdir=$$reldir; \
	    echo " (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir="$$new_top_distdir" distdir="$$new_distdir" \\"; \
	    echo "     am__remove_distdir=: am__skip_length_check=: am__skip_mode_fix=: distdir)"; \
	    ($(am__cd) $$subdir && \
	      $(MAKE) $(AM_MAKEFLAGS) \
	        top_distdir="$$new_top_distdir" \
	        distdir="$$new_distdir" \
		am__remove_distdir=: \
		am__skip_length_check=: \
		am__skip_mode_fix=: \
	        distdir) \
	      || exit 1; \
	  fi; \
	done
	-test -n "$(am__skip_mode_fix)" \
	|| find "$(distdir)" -type d ! -perm -755 \
		-exec chmod u+rwx,go+rx {} \; -o \
	  ! -type d ! -perm -444 -links 1 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -400 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -444 -exec $(install_sh) -c -m a+r {} {} \; \
	|| chmod -R a+r "$(distdir)"
dist-gzip: distdir
	tardir=$(distdir) && $(am__tar) | eval GZIP= gzip $(GZIP_ENV) -c >$(distdir).tar.gz
	$(am__post_remove_distdir)

dist-bzip2: distdir
	tardir=$(distdir) && $(am__tar) | BZIP2=$${BZIP2--9} bzip2 -c >$(distdir).tar.bz2
	$(am__post_remove_distdir)

dist-lzip: distdir
	tardir=$(distdir) && $(am__tar) | lzip -c $${LZIP_OPT--9} >$(distdir).tar.lz
	$(am__post_remove_distdir)

dist-xz: distdir
	tardir=$(distdir) && $(am__tar) | XZ_OPT=$${XZ_OPT--e} xz -c >$(distdir).tar.xz
	$(am__post_remove_distdir)

dist-tarZ: distdir
	@echo WARNING: "Support for distribution archives compressed with" \
		       "legacy program 'compress' is deprecated." >&2
	@echo WARNING: "It will be removed altogether in Automake 2.0" >&2
	tardir=$(distdir) && $(am__tar) | compress -c >$(distdir).tar.Z
	$(am__post_remove_distdir)

dist-shar: distdir
	@echo WARNING: "Support for shar distribution archives is" \
	               "deprecated." >&2
	@echo WARNING: "It will be removed altogether in Automake 2.0" >&2
	shar $(distdir) | eval GZIP= gzip $(GZIP_ENV) -c >$(distdir).shar.gz
	$(am__post_remove_distdir)

dist-zip: distdir
	-rm -f $(distdir).zip
	zip -rq $(distdir).zip $(distdir)
	$(am__post_remove_distdir)

dist dist-all:
	$(MAKE) $(AM_MAKEFLAGS) $(DIST_TARGETS) am__post_remove_distdir='@:'
	$(am__post_remove_distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	case '$(DIST_ARCHIVES)' in \
	*.tar.gz*) \
	  eval GZIP= gzip $(GZIP_ENV) -dc $(distdir).tar.gz | $(am__untar) ;;\
	*.tar.bz2*) \
	  bzip2 -dc $(distdir).tar.bz2 | $(am__untar) ;;\
	*.tar.lz*) \
	  lzip -dc $(distdir).tar.lz | $(am__untar) ;;\
	*.tar.xz*) \
	  xz -dc $(distdir).tar.xz | $(am__untar) ;;\
	*.tar.Z*) \
	  uncompress -c $(distdir).tar.Z | $(am__untar) ;;\
	*.shar.gz*) \
	  eval GZIP= gzip $(GZIP_ENV) -dc $(distdir).shar.gz | unshar ;;\
	*.zip*) \
	  unzip $(distdir).zip ;;\
	esac
	chmod -R a-w $(distdir)
	chmod u+w $(distdir)
	mkdir $(distdir)/_build $(distdir)/_build/sub $(distdir)/_inst
	chmod a-w $(distdir)
	test -d $(distdir)/_build || exit 0; \
	dc_install_base=`$(am__cd) $(distdir)/_inst && pwd | sed -e 's,^[^:\\/]:[\\/],/,'` \
	  && dc_destdir="$${TMPDIR-/tmp}/am-dc-$$$$/" \
	  && am__cwd=`pwd` \
	  && $(am__cd) $(distdir)/_build/sub \
	  && ../../configure \
	    $(AM_DISTCHECK_CONFIGURE_FLAGS) \
	    $(DISTCHECK_CONFIGURE_FLAGS) \
	    --srcdir=../.. --prefix="$$dc_install_base" \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) uninstall \
	  && $(MAKE) $(AM_MAKEFLAGS) distuninstallcheck_dir="$$dc_install_base" \
	        distuninstallcheck \
	  && chmod -R a-w "$$dc_install_base" \
	  && ({ \
	       (cd ../.. && umask 077 && mkdir "$$dc_destdir") \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" install \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" uninstall \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" \
	            distuninstallcheck_dir="$$dc_destdir" distuninstallcheck; \
	      } || { rm -rf "$$dc_destdir"; exit 1; }) \
	  && rm -rf "$$dc_destdir" \
	  && $(MAKE) $(AM_MAKEFLAGS) dist \
	  && rm -rf $(DIST_ARCHIVES) \
	  && $(MAKE) $(AM_MAKEFLAGS) distcleancheck \
	  && cd "$$am__cwd" \
	  || exit 1
	$(am__post_remove_distdir)
	@(echo "$(distdir) archives ready for distribution: "; \
	  list='$(DIST_ARCHIVES)'; for i in $$list; do echo $$i; done) | \
	  sed -e 1h -e 1s/./=/g -e 1p -e 1x -e '$$p' -e '$$x'
distuninstallcheck:
	@test -n '$(distuninstallcheck_dir)' || { \
	  echo 'ERROR: trying to run $@ with an empty' \
	       '$$(distuninstallcheck_dir)' >&2; \
	  exit 1; \
	}; \
	$(am__cd) '$(distuninstallcheck_dir)' || { \
	  echo 'ERROR: cannot chdir into $(distuninstallcheck_dir)' >&2; \
	  exit 1; \
	}; \
	test `$(am__distuninstallcheck_listfiles) | wc -l` -eq 0 \
	   || { echo "ERROR: files left after uninstall:" ; \
	        if test -n "$(DESTDIR)"; then \
	          echo "  (check DESTDIR support)"; \
	        fi ; \
	        $(distuninstallcheck_listfiles) ; \
	        exit 1; } >&2
distcleancheck: distclean
	@if test '$(srcdir)' = . ; then \
	  echo "ERROR: distcleancheck can only run from a VPATH build" ; \
	  exit 1 ; \
	fi
	@test `$(distcleancheck_listfiles) | wc -l` -eq 0 \
	  || { echo "ERROR: files left in build directory after distclean:" ; \
	       $(distcleancheck_listfiles) ; \
	       exit 1; } >&2
check-am: all-am
check: $(BUILT_SOURCES)
	$(MAKE) $(AM_MAKEFLAGS) check-recursive
all-am: Makefile
installdirs: installdirs-recursive
installdirs-am:
install: $(BUILT_SOURCES)
	$(MAKE) $(AM_MAKEFLAGS) install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-recursive
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)
	-test -z "$(DISTCLEANFILES)" || rm -f $(DISTCLEANFILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
	-test -z "$(BUILT_SOURCES)" || rm -f $(BUILT_SOURCES)
	-test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
clean: clean-recursive

clean-am: clean-generic clean-libtool clean-local mostlyclean-am

distclean: distclean-recursive
	-rm -f $(am__CONFIG_DISTCLEAN_FILES)
	-rm -f Makefile
distclean-am: clean-am distclean-generic distclean-libtool \
	distclean-tags

dvi: dvi-recursive

dvi-am:

html: html-recursive

html-am: html-local

info: info-recursive

info-am:

install-data-am:

install-dvi: install-dvi-recursive

install-dvi-am:

install-exec-am:

install-html: install-html-recursive

install-html-am:

install-info: install-info-recursive

install-info-am:

install-man:

install-pdf: install-pdf-recursive

install-pdf-am:

install-ps: install-ps-recursive

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-recursive
	-rm -f $(am__CONFIG_DISTCLEAN_FILES)
	-rm -rf $(top_srcdir)/autom4te.cache
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-recursive

mostlyclean-am: mostlyclean-generic mostlyclean-libtool

pdf: pdf-recursive

pdf-am:

ps: ps-recursive

ps-am:

uninstall-am:

.MAKE: $(am__recursive_targets) all check install install-am \
	install-strip

.PHONY: $(am__recursive_targets) CTAGS GTAGS TAGS all all-am \
	am--refresh check check-am clean clean-cscope clean-generic \
	clean-libtool clean-local cscope cscopelist-am ctags ctags-am \
	dist dist-all dist-bzip2 dist-gzip dist-lzip dist-shar \
	dist-tarZ dist-xz dist-zip distcheck distclean \
	distclean-generic distclean-libtool distclean-tags \
	distcleancheck distdir distuninstallcheck dvi dvi-am html \
	html-am html-local info info-am install install-am \
	install-data install-data-am install-dvi install-dvi-am \
	install-exec install-exec-am install-html install-html-am \
	install-info install-info-am install-man install-pdf \
	install-pdf-am install-ps install-ps-am install-strip \
	installcheck installcheck-am installdirs installdirs-am \
	maintainer-clean maintainer-clean-generic mostlyclean \
	mostlyclean-generic mostlyclean-libtool pdf pdf-am ps ps-am \
	tags tags-am uninstall uninstall-am

.PRECIOUS: Makefile


# Local Variables:
# mode: makefile
# End:
.PHONY: $(COVERAGE_TARGETS)

#coverage-reset:
#	$(AM_V_at)$(LCOV) --zerocounters --quiet --directory $(top_builddir)

#coverage-genhtml:
#	$(AM_V_at)$(LCOV) $(lcov_capture_options)
#	$(AM_V_at)$(LCOV) $(lcov_remove_options)
#	$(AM_V_at)$(GENHTML) $(lcov_genhtml_options)
#	@echo 'REPORT: file://$(abs_top_builddir)/$(LCOV_DIR)/index.html'

#coverage: all
#	$(MAKE) $(AM_MAKEFLAGS) coverage-reset
#	-$(MAKE) $(AM_MAKEFLAGS) -k check
#	$(MAKE) $(AM_MAKEFLAGS) coverage-genhtml

#coverage-clean:
#	-rm -rf $(LCOV_DIR) $(LCOV_INFO)

$(COVERAGE_TARGETS):
	@echo You must configure $(PACKAGE_NAME) with support for gcov.
	@echo E.g., ./configure --enable-debug --enable-coverage

# Local Variables:
# mode: makefile
# End:

.PHONY: doc html
doc: html

html-local: doc/Doxyfile
	$(AM_V_GEN) $(DOXYGEN) doc/Doxyfile
doc/Doxyfile: $(top_srcdir)/doc/Doxyfile.in
	$(AM_V_GEN) $(edit) $(top_srcdir)/doc/Doxyfile.in >doc/Doxyfile

README: README.md
	$(AM_V_GEN)$(PERL) -wnle '$(perl_strip_md)' README.md > $@

clean-local: coverage-clean
	rm -rf $(CLEANDIRS)

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
