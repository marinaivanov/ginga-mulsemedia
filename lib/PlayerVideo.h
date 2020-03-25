/* Copyright (C) 2006-2018 PUC-Rio/Laboratorio TeleMidia

This file is part of Ginga (Ginga-NCL).

Ginga is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Ginga is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Ginga.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef PLAYER_VIDEO_H
#define PLAYER_VIDEO_H

#include "Player.h"

GINGA_NAMESPACE_BEGIN
class Media;
class PlayerVideo : public Player
{
public:
  PlayerVideo (Formatter *, Media *);
  ~PlayerVideo ();
  void start () override;
  void stop () override;
  void pause () override;
  void resume () override;
  void startPreparation () override;
  void redraw (cairo_t *) override;

protected:
  bool doSetProperty (Property, const string &, const string &) override;
  void seek (gint64);
  void speed (double);
  gint64 getPipelineTime ();
  gint64 getStreamMediaTime ();
  gint64 getStreamMediaDuration ();

private:
  GstElement *_playbin; // pipeline
  bool is_buffering;
  struct
  {                        // audio pipeline
    GstElement *bin;       // audio bin
    GstElement *volume;    // volume filter
    GstElement *pan;       // balance filter
    GstElement *equalizer; // equalizer filter
    GstElement *convert;   // convert audio format
    GstElement *sink;      // audio sink
  } _audio;
  struct
  {                   // video pipeline
    GstElement *bin;  // video bin
    GstElement *caps; // caps filter
    GstElement *sink; // app sink
  } _video;
  int _sample_flag;               // true if new sample is available
  GstAppSinkCallbacks _callbacks; // video app-sink callback data
  struct
  {
    bool mute;      // true if mute is on
    double balance; // balance sound level
    double volume;  // sound level
    double treble;  // treble level (Default: 0; Range: -24 and +12)
    double bass;    // bass level (Default: 0; Range: -24 and +12)
    bool freeze;    // true if player should freeze
    double speed;   // playback speed (Default: 1)
    double bufferOffset; //offset in the source file of the beginning of the buffer
    double bufferOffsetEnd; // offset in the source file of the end of the buffer
  } _prop;
  typedef struct
  {
    Property code;
    string name;
    string value;
  } PlayerVideoAction;
  list<PlayerVideoAction> _stack_actions;
  void
  initProperties (set<string> *); // Init default values to properties
                                  // without go to doSetProperty function
  void stackAction (Property, const string &, const string &);
  void doStackedActions ();
  void doPreparationActions ();
  bool getFreeze ();
  string getPipelineState ();

  // GStreamer callbacks.
  static gboolean cb_Bus (GstBus *, GstMessage *, PlayerVideo *);
  static GstFlowReturn cb_NewSample (GstAppSink *, gpointer);
  static void on_message_buffering (GstBus *bus, GstMessage *message, gpointer data);
  static void on_message_async_done (GstBus *bus, GstMessage *message, gpointer data);
  static gboolean buffer_timeout (gpointer data);
};

GINGA_NAMESPACE_END

#endif // PLAYER_VIDEO_H
