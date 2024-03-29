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
MAINTAINERCLEANFILES+= config.h.in config.h.in~

EXTRA_DIST+= ginga.pc.in
DISTCLEANFILES+= ginga.pc

pkgconfigdir= $(libdir)/pkgconfig
pkgconfig_DATA= ginga.pc

REST_CFLAG= -I/usr/include/restclient-cpp
REST_LIB = -L/usr/local/lib -lrestclient-cpp -lcurl

TOBII_CFLAG= -I/usr/include/tobii
TOBII_LIB = -L/usr/lib/tobii -ltobii_stream_engine

AM_CXXFLAGS= $(GINGA_ALL_CXXFLAGS) $(REST_CFLAG)
AM_LDFLAGS= $(GINGA_ALL_LDFLAGS) $(REST_LIB)


include_HEADERS= ginga.h mqtt/src/mqtt.h mqtt/src/mqtt_pal.h mqtt/include/posix_sockets.h 

lib_LTLIBRARIES= libginga.la
libginga_la_CXXFLAGS= $(AM_CXXFLAGS) $(TOBII_CFLAG)
libginga_la_LDFLAGS= $(AM_LDFLAGS) $(TOBII_LIB)
libginga_la_SOURCES= $(src)
src=
src+= Composition.cpp
src+= Context.cpp
src+= Document.cpp
src+= Event.cpp
src+= Effect.cpp
src+= Formatter.cpp
src+= Ginga.cpp
src+= Media.cpp
src+= MediaSettings.cpp
src+= Object.cpp
src+= Parser.cpp
src+= Player.cpp
src+= PlayerAnimator.cpp
src+= PlayerImage.cpp
src+= PlayerSigGen.cpp
src+= PlayerText.cpp
src+= PlayerVideo.cpp
src+= Predicate.cpp
src+= Switch.cpp
src+= TemporalGraph.cpp
src+= PresentationOrchestrator.cpp
src+= mqtt/src/mqtt.c
src+= mqtt/src/mqtt_pal.c
src+= ../intMod/InteractionModule.cpp
src+= ../intMod/VoiceRecognition.cpp
src+= ../intMod/GazeRecognition.cpp
src+= ../intMod/FacialExpressionRecognition.cpp
#src+= ../intMod/HandPoseRecognition.cpp
src+= InteractionManager.cpp
src+= UserContextManager.cpp
src+= ../loadUDMod/ParserUserDescription.cpp
src+= aux-ginga.cpp
src+= aux-gl.cpp
src+= EffectPlayer.cpp
src+= EffectPlayerScent.cpp
src+= Device.cpp
src+= DeviceScent.cpp
src+= EffectPlayerLight.cpp
src+= DeviceLight.cpp
src+= ParserDeviceFile.cpp
src+= DeviceCalibration.cpp

if WITH_LIBRSVG
src+= PlayerSvg.cpp
endif # WITH_LIBRSVG

if WITH_LUA
src+= ParserLua.cpp
endif # WITH_LUA

if WITH_NCLUA
src+= PlayerLua.cpp
endif # WITH_NCLUA

if WITH_CEF
src+= PlayerHTML.cpp
cef_filesdir= $(bindir)
cef_files_DATA=\
  $(cef_dir)/Release/chrome-sandbox\
  $(cef_dir)/Release/libcef.so\
  $(cef_dir)/Release/natives_blob.bin\
  $(cef_dir)/Release/snapshot_blob.bin\
  $(cef_dir)/Resources/cef.pak\
  $(cef_dir)/Resources/cef_100_percent.pak\
  $(cef_dir)/Resources/cef_200_percent.pak\
  $(cef_dir)/Resources/cef_extensions.pak\
  $(cef_dir)/Resources/icudtl.dat\
  $(NULL)
endif # WITH_CEF
