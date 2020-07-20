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

include $(top_srcdir)/build-aux/Makefile.am.common

#Eye Gaze Module paths
#set(TOBII_CXXFLAGS  "${top_srcdir}/lib/tobii")
#set(TOBII_LDFLAGS "/usr/lib/tobii/libtobii_stream_engine.so")

#nao aceitou a forma abaixo
#TOBII_CXXFLAGS = -I$(top_builddir)/lib/tobii
#TOBII_LDFLAGS = -L/usr/lib/tobii/libtobii_stream_engine.so 


AM_CPPFLAGS= -I$(top_srcdir)/lib -I$(top_builddir)/lib
AM_CXXFLAGS= $(WARN_CFLAGS) $(WERROR_CFLAGS)\
  $(CAIRO_CFLAGS) $(GLIB_CFLAGS) $(GTK_CFLAGS) -I/../lib/tobii
AM_LDFLAGS= $(CAIRO_LIBS) $(GLIB_LIBS) $(GTK_LIBS) -L/usr/lib/tobii/libtobii_stream_engine.so

bin_PROGRAMS= ginga
if WITH_OPENGL
bin_PROGRAMS+= ginga-gl
endif

ginga_CXXFLAGS= $(AM_CXXFLAGS)
ginga_LDFLAGS= $(AM_LDFLAGS)
ginga_SOURCES= ginga.cpp
ginga_LDADD= $(top_builddir)/lib/libginga.la

ginga_gl_CXXFLAGS= $(AM_CXXFLAGS) $(OPENGL_CFLAGS)
ginga_gl_LDFLAGS= $(AM_LDFLAGS) $(OPENGL_LIBS)
ginga_gl_SOURCES= ginga-gl.cpp
ginga_gl_LDADD= $(top_builddir)/lib/libginga.la

#AM_CXXFLAGS= -I/$(top_builddir)/lib/tobii/tobii.h
#AM_LDFLAGS= -L/usr/lib/tobii/libtobii_stream_engine.so -lXXXX