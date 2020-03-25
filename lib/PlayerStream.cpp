#include "aux-ginga.h"
#include "aux-gl.h"
#include <gst/gst.h>
#include <gst/rtp/rtp.h>
#include <stdlib.h>
#include "PlayerStream.h"
#include <thread>   
  
GINGA_NAMESPACE_BEGIN
#define gstx_element_get_state(elt, st, pend, tout)                        \
  g_assert (gst_element_get_state ((elt), (st), (pend), (tout))            \
            != GST_STATE_CHANGE_FAILURE)

#define gstx_element_get_state_sync(elt, st, pend)                         \
  gstx_element_get_state ((elt), (st), (pend), GST_CLOCK_TIME_NONE)

#define gstx_element_set_state(elt, st)                                    \
  g_assert (gst_element_set_state ((elt), (st)) != GST_STATE_CHANGE_FAILURE)

#define gstx_element_set_state_sync(elt, st)                               \
  G_STMT_START                                                             \
  {                                                                        \
    gstx_element_set_state ((elt), (st));                                  \
    gstx_element_get_state_sync ((elt), nullptr, nullptr);                 \
  }                                                                        \
  G_STMT_END

/* This function is called when the STOP button is clicked */
static void stop_cb (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->playbin, GST_STATE_READY);
}

/* This function is called when the main window is closed */
static void delete_event_cb (GtkWidget *widget, GdkEvent *event, CustomData *data) {
  stop_cb (NULL, data);
  gtk_main_quit ();
}

PlayerStream::PlayerStream (Formatter *formatter, Media *media) : Player (formatter, media)
{
  GstMessage *msg;
  GstBus *bus;
  GError *error = NULL;
  gulong ret;
  GstPad *pad;
  GstPad *ghost;
  
  _playbin = nullptr;
  _audio.bin = nullptr;
  _audio.volume = nullptr;
  _audio.pan = nullptr;
  _audio.equalizer = nullptr;
  _audio.convert = nullptr;
  _audio.sink = nullptr;
  _video.bin = nullptr;
  _video.caps = nullptr;
  _video.sink = nullptr;
        
  if (!gst_is_initialized ())
  {
    GError *error = nullptr;
    if (unlikely (!gst_init_check (nullptr, nullptr, &error)))
      {
        g_assert_nonnull (error);
        ERROR ("%s", error->message);
        g_error_free (error);
      }
  }

  GdkWindow *video_window_xwindow;
  GtkWidget *video_window;
  gulong embed_xid;
  GstStateChangeReturn sret;

  gtk_init (NULL,NULL);
  
  // Initialize our data structure 
  memset (&data, 0, sizeof (data));
 
  GstElement *udpsrc, *appxrtp, *depay, *parse, *omxh264dec, *videoConvert, *sink;

  data.playbin = gst_pipeline_new ("playbin");
  
  if (!data.playbin) {
    g_printerr ("Not all elements could be created.\n");
    exit(1);
  }

  udpsrc = gst_element_factory_make ("udpsrc", NULL); g_assert(udpsrc);

  //Set CAPS
  g_object_set (G_OBJECT (udpsrc), "port", 5600, NULL);
  GstCaps * xrtpcaps = gst_caps_from_string("application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264");
  g_object_set (udpsrc, "caps", xrtpcaps, NULL);

  depay = gst_element_factory_make ("rtph264depay", NULL); g_assert(depay);
  parse = gst_element_factory_make ("h264parse", NULL); g_assert(parse);
  omxh264dec = gst_element_factory_make ("avdec_h264", NULL); g_assert(omxh264dec);
  videoConvert = gst_element_factory_make ("videoconvert", NULL); g_assert(videoConvert);
  sink = gst_element_factory_make ("xvimagesink", NULL); g_assert(sink);
  g_object_set (G_OBJECT (sink), "sync", FALSE, NULL);
  //ADD
  gst_bin_add_many (GST_BIN (data.playbin), udpsrc, depay, parse, omxh264dec, videoConvert, sink, NULL);
  
  //LINK
  g_assert(gst_element_link (udpsrc, depay));
  g_assert(gst_element_link (depay, parse));
  g_assert(gst_element_link (parse, omxh264dec));
  g_assert(gst_element_link (omxh264dec, videoConvert));
  g_assert(gst_element_link (videoConvert, sink));

  /* prepare the ui */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);   

  g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (delete_event_cb), (gpointer) data.playbin);
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
  gtk_window_set_title (GTK_WINDOW (window), "GstVideoOverlay Gtk+ demo");

  video_window = gtk_drawing_area_new ();
  gtk_container_add (GTK_CONTAINER (window), video_window);
  gtk_container_set_border_width (GTK_CONTAINER (window), 2);

  gtk_widget_show_all (window);
  gtk_window_fullscreen(GTK_WINDOW(window));

  video_window_xwindow = gtk_widget_get_window (video_window);
  embed_xid = GDK_WINDOW_XID (video_window_xwindow);
  gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (sink), embed_xid);

}

/*PlayerStream::PlayerStream (Formatter *formatter, Media *media): Player (formatter, media)
  {
    GstBus *bus;
    gulong ret;
    GstPad *pad;
    GstPad *ghost;
    GstElement *_udpsrc, *_rtpjitterbuffer, *_rtph264depay, *_avdec_h264, *_decodebin, *_videoconvert;
    GstCaps *caps;
    
    _playbin = nullptr;
    _audio.bin = nullptr;
    _audio.volume = nullptr;
    _audio.pan = nullptr;
    _audio.equalizer = nullptr;
    _audio.convert = nullptr;
    _audio.sink = nullptr;
    _video.bin = nullptr;
    _video.caps = nullptr;
    _video.sink = nullptr;
          
    if (!gst_is_initialized ())
      {
        GError *error = nullptr;
        if (unlikely (!gst_init_check (nullptr, nullptr, &error)))
          {
            g_assert_nonnull (error);
            ERROR ("%s", error->message);
            g_error_free (error);
          }
      }

    //autovideosink", &error

    _playbin = gst_element_factory_make ("playbin", "playbin");
    g_assert_nonnull (_playbin);

    _udpsrc = gst_element_factory_make("udpsrc", "udpsrc0");
    g_assert_nonnull (_udpsrc);
    g_object_set (G_OBJECT (_udpsrc), "caps", gst_caps_from_string("application/x-rtp,payload=96"), NULL);

    _rtpjitterbuffer = gst_element_factory_make("rtpjitterbuffer", "rtpjitterbuffer0");
    g_assert_nonnull (_rtpjitterbuffer);
    _rtph264depay = gst_element_factory_make("rtph264depay", "rtph264depay0");
    g_assert_nonnull (_rtph264depay);
    _avdec_h264 = gst_element_factory_make("avdec_h264", "avdec_h2640");
    g_assert_nonnull (_avdec_h264);
     //decoder, converter, queue
  
    
    _decodebin = gst_element_factory_make ("decodebin", NULL);
    _videoconvert = gst_element_factory_make ("videoconvert", NULL);
    _video.sink = gst_element_factory_make ("appsink", "video.sink");
    
    bus = gst_pipeline_get_bus (GST_PIPELINE (_playbin));
    g_assert_nonnull (bus);
    ret = gst_bus_add_watch (bus, (GstBusFunc) cb_Bus, this);
    //g_signal_connect (bus, "message::buffering",
    //  G_CALLBACK (on_message_buffering), this);
    //g_signal_connect (bus, "message::async-done",
    //  G_CALLBACK (on_message_async_done), this);
    g_assert (ret > 0);  
    gst_object_unref (bus);

    is_buffering = FALSE;

    // Setup audio pipeline.
    _audio.bin = gst_bin_new ("audio.bin");
    g_assert_nonnull (_audio.bin);

    _audio.volume = gst_element_factory_make ("volume", "audio.volume");
    g_assert_nonnull (_audio.volume);

    _audio.pan = gst_element_factory_make ("audiopanorama", "audio.pan");
    g_assert_nonnull (_audio.pan);

    _audio.equalizer
        = gst_element_factory_make ("equalizer-3bands", "audio.equalizer");
    g_assert_nonnull (_audio.equalizer);

    _audio.convert
        = gst_element_factory_make ("audioconvert", "audio.convert");
    g_assert_nonnull (_audio.convert);

    // Try to use ALSA if available.
    _audio.sink = gst_element_factory_make ("alsasink", "audio.sink");
    if (_audio.sink == nullptr)
      _audio.sink = gst_element_factory_make ("autoaudiosink", "audio.sink");
    g_assert_nonnull (_audio.sink);

    g_assert (gst_bin_add (GST_BIN (_audio.bin), _audio.volume));
    g_assert (gst_bin_add (GST_BIN (_audio.bin), _audio.pan));
    g_assert (gst_bin_add (GST_BIN (_audio.bin), _audio.equalizer));
    g_assert (gst_bin_add (GST_BIN (_audio.bin), _audio.convert));
    g_assert (gst_bin_add (GST_BIN (_audio.bin), _audio.sink));
    g_assert (gst_element_link (_audio.volume, _audio.pan));
    g_assert (gst_element_link (_audio.pan, _audio.equalizer));
    g_assert (gst_element_link (_audio.equalizer, _audio.convert));
    g_assert (gst_element_link (_audio.convert, _audio.sink));
    
    pad = gst_element_get_static_pad (_audio.volume, "sink");
    g_assert_nonnull (pad);
    ghost = gst_ghost_pad_new ("sink", pad);
    g_assert_nonnull (ghost);
    g_assert (gst_element_add_pad (_audio.bin, ghost));
    gst_object_unref (pad);
    g_object_set (G_OBJECT (_playbin), "audio-sink", _audio.bin, nullptr);

    // Setup video pipeline.
    _video.bin = gst_bin_new ("video.bin");
    g_assert_nonnull (_video.bin);

    _video.caps = gst_element_factory_make ("capsfilter", "video.filter");
    g_assert_nonnull (_video.caps);

    //_video.sink = gst_element_factory_make ("appsink", "video.sink");
    //g_assert_nonnull (_video.sink);

  //#ifdef __APPLE__
  //  g_object_set (_video.sink, "max-buffers", 1, "drop", true, nullptr);
  //#else
  //  g_object_set (_video.sink, "max-buffers", 100, "drop", true, nullptr);
  //#endif

    g_assert (gst_bin_add (GST_BIN (_video.bin), _video.caps));
    g_assert (gst_bin_add (GST_BIN (_video.bin), _video.sink));
    g_assert (gst_element_link (_video.caps, _video.sink));
    
      gst_bin_add_many (GST_BIN(_playbin),
      _udpsrc, _rtpjitterbuffer, _rtph264depay, _avdec_h264, _video.bin, NULL);

    if (!gst_element_link_many (_udpsrc, _rtpjitterbuffer, _rtph264depay, _avdec_h264, _video.bin, NULL)) {
      g_warning ("Failed to link pipeline");
      exit(1);
    }

    g_object_set (_udpsrc, "port", 5600, NULL);

    // Callbacks.
    _callbacks.eos = nullptr;
    _callbacks.new_preroll = nullptr;
    _callbacks.new_sample = cb_NewSample;
    gst_app_sink_set_callbacks (GST_APP_SINK (_video.sink), &_callbacks, this,
                                nullptr);
    
    pad = gst_element_get_static_pad (_video.caps, "sink");
    g_assert_nonnull (pad);
    ghost = gst_ghost_pad_new ("sink", pad);
    g_assert_nonnull (ghost);
    g_assert (gst_element_add_pad (_video.bin, ghost));
    gst_object_unref (pad);
    g_object_set (G_OBJECT (_playbin), "video-sink", _video.bin, nullptr);

    // Initialize some handled properties.
    static set<string> handled = {"balance", "bass",   "freeze", "mute",
                                  "speed",   "treble", "volume"};
    this->initProperties (&handled);

    // stackAction (Player::PROP_TIME, "time", "76s");
    
  }*/
  
static gpointer run (gpointer data)
{
  gtk_main();
  return NULL;
}

void PlayerStream::start()
{
  GstCaps *caps;
  GstStructure *st;
  GstStateChangeReturn ret;

  g_assert (_state != OCCURRING);
  TRACE ("starting %s", _id.c_str ());

  //ANTIGO
  st = gst_structure_new_empty ("application/x-rtp");
  gst_structure_set (st, "format", G_TYPE_STRING, "BGRA", nullptr);

  caps = gst_caps_new_full (st, nullptr);
  g_assert_nonnull (caps);
  g_object_set (_video.caps, "caps", caps, nullptr);
  gst_caps_unref (caps);

  Player::setEOS (false);
  g_atomic_int_set (&_sample_flag, 0);

  g_object_set (_audio.volume, "volume", _prop.volume, "mute", _prop.mute,
                nullptr);

  g_object_set (_audio.pan, "panorama", _prop.balance, nullptr);

  g_object_set (_audio.equalizer, "band0", _prop.bass, "band1",
                _prop.treble, "band2", _prop.treble, nullptr);

  //ret = gst_element_set_state (data.playbin, GST_STATE_PLAYING);
  //if (unlikely (ret == GST_STATE_CHANGE_FAILURE))
  //  Player::setEOS (true);

  /* run the pipeline */
  ret = gst_element_set_state (data.playbin, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE)
    gst_element_set_state (data.playbin, GST_STATE_NULL);
  else{
    /* run the time-consuming operation in a separate thread */
    GThread    *thread;
    thread = g_thread_new ("run", run, NULL);
    g_thread_unref (thread);
  }

  //gst_object_unref (data.playbin);

  Player::start ();   
}

PlayerStream::~PlayerStream ()
{
}

void
PlayerStream::stop ()
{
  g_assert (_state != SLEEPING);
  TRACE ("stopping %s", _id.c_str ());

  //gstx_element_set_state_sync (data.playbin, GST_STATE_NULL);
  //gst_object_unref (data.playbin);
  //_playbin = nullptr;
  _stack_actions.clear ();
  gtk_window_unfullscreen (GTK_WINDOW(window));
  stop_cb (NULL, &data);
  gtk_main_quit ();
  Player::stop ();

}

void
PlayerStream::pause ()
{
  gtk_widget_hide (window);
 // g_assert (_state != PAUSED && _state != SLEEPING);
 // TRACE ("pausing %s", _id.c_str ());

 // gstx_element_set_state_sync (_playbin, GST_STATE_PAUSED);
 // Player::pause ();
}

void
PlayerStream::resume ()
{
  /*g_assert (_state == PAUSED);
  TRACE ("resuming %s", _id.c_str ());

  gstx_element_set_state_sync (_playbin, GST_STATE_PLAYING);
  Player::resume ();*/
  gtk_widget_show_all (window);
}

void
PlayerStream::seek (gint64 value)
{
  TRACE ("seek %s to: %" GST_TIME_FORMAT, _id.c_str (),
         GST_TIME_ARGS (value));

  GstState curr, pending;
  GstStateChangeReturn ret;

  if (unlikely (!gst_element_seek (_playbin, _prop.speed, GST_FORMAT_TIME,
                                   GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET,
                                   value, GST_SEEK_TYPE_NONE,
                                   (gint64) GST_CLOCK_TIME_NONE)))
    TRACE ("seek failed");

  ret = gst_element_get_state (_playbin, &curr, &pending,
                               GST_CLOCK_TIME_NONE);
  if (unlikely (ret == GST_STATE_CHANGE_FAILURE))
    {
      string m = gst_element_state_change_return_get_name (ret);
      TRACE ("%s to change state", m.c_str ());
    }
}

void
PlayerStream::speed (double value)
{
  TRACE ("speed %s to: %f", _id.c_str (), value);
  if (doubleeq (value, 0.))
    {
      return; // nothing to do
    }

  gint64 position = getStreamMediaTime ();
  GstEvent *seek_event;
  if (value > 0)
    {
      seek_event = gst_event_new_seek (
          value, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET,
          position, GST_SEEK_TYPE_NONE, 0);
    }
  else
    {
      seek_event = gst_event_new_seek (
          value, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, 0,
          GST_SEEK_TYPE_NONE, position);
    }
  if (unlikely (!gst_element_send_event (_video.sink, seek_event)))
    TRACE ("speed failed");
  gst_event_unref (seek_event);
}

void
PlayerStream::startPreparation ()
{
  
}

void
PlayerStream::redraw (cairo_t *cr)
{
  GstSample *sample;
  GstVideoFrame v_frame;
  GstVideoInfo v_info;
  GstBuffer *buf;
  GstCaps *caps;
  guint8 *pixels;
  int width;
  int height;
  int stride;

  static cairo_user_data_key_t key;
  cairo_status_t status;

  g_assert (_state != SLEEPING);

  if (Player::getEOS ())
    goto done;

  if (!g_atomic_int_compare_and_exchange (&_sample_flag, 1, 0))
    goto done;

  TRACE("AQUI---------------2\n");
  sample = gst_app_sink_pull_sample (GST_APP_SINK (_video.sink));
  if (sample == nullptr)
    goto done;
  
  TRACE("AQUI---------------3\n");
  buf = gst_sample_get_buffer (sample);
  g_assert_nonnull (buf);
  
  caps = gst_sample_get_caps (sample);
  g_assert_nonnull (caps);

  g_assert (gst_video_info_from_caps (&v_info, caps));
  g_assert (gst_video_frame_map (&v_frame, &v_info, buf, GST_MAP_READ));

  pixels = (guint8 *) GST_VIDEO_FRAME_PLANE_DATA (&v_frame, 0);
  width = GST_VIDEO_FRAME_WIDTH (&v_frame);
  height = GST_VIDEO_FRAME_HEIGHT (&v_frame);
  stride = (int) GST_VIDEO_FRAME_PLANE_STRIDE (&v_frame, 0);

  if (_opengl)
    {
      if (_gltexture)
        GL::delete_texture (&_gltexture);
      // FIXME: Do not create a new texture for each frame.
      GL::create_texture (&_gltexture, width, height, pixels);
      gst_video_frame_unmap (&v_frame);
      gst_sample_unref (sample);
    }
  else
    {
      if (_surface != nullptr)
        cairo_surface_destroy (_surface);

      _surface = cairo_image_surface_create_for_data (
          pixels, CAIRO_FORMAT_ARGB32, width, height, stride);
      g_assert_nonnull (_surface);
      gst_video_frame_unmap (&v_frame);
      status = cairo_surface_set_user_data (
          _surface, &key, (void *) sample,
          (cairo_destroy_func_t) gst_sample_unref);
      g_assert (status == CAIRO_STATUS_SUCCESS);
    }

done:
  Player::redraw (cr);
}

bool
PlayerStream::doSetProperty (Property code, unused (const string &name),
                            const string &value)
{
  
  return true;
}

gint64
PlayerStream::getStreamMediaTime ()
{
  gint64 cur;
  if (unlikely (
          !gst_element_query_position (_playbin, GST_FORMAT_TIME, &cur)))
    TRACE ("Get %s time failed", _id.c_str ());
  return cur;
}

GstFlowReturn
PlayerStream::cb_NewSample (unused (GstAppSink *appsink), gpointer data)
{
  TRACE("CB_NEWSAMPLE\n");
  PlayerStream *player = (PlayerStream *) data;
  g_assert_nonnull (player);
  g_atomic_int_compare_and_exchange (&player->_sample_flag, 0, 1);
  return GST_FLOW_OK;
}

string
PlayerStream::getPipelineState ()
{
  GstState curr;
  GstState pending;
  GstStateChangeReturn ret;
  ret = gst_element_get_state (_playbin, &curr, &pending, 0);

  if (unlikely (ret == GST_STATE_CHANGE_FAILURE))
    {
      return "NULL";
    }
  return gst_element_state_get_name (curr);
}

gboolean
PlayerStream::cb_Bus (GstBus *bus, GstMessage *msg, PlayerStream *player)
{
  g_assert_nonnull (bus);
  g_assert_nonnull (msg);
  g_assert_nonnull (player);

  switch (GST_MESSAGE_TYPE (msg))
    {
    case GST_MESSAGE_EOS:
      {
        if (unlikely (!player->getFreeze ()))
          player->setEOS (true);
        TRACE ("EOS of %s", player->_id.c_str ());
        break;
      }
    case GST_MESSAGE_ERROR:
    case GST_MESSAGE_WARNING:
      {
        GstObject *obj = nullptr;
        GError *error = nullptr;

        obj = GST_MESSAGE_SRC (msg);
        g_assert_nonnull (obj);

        if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR)
          {
            gst_message_parse_error (msg, &error, nullptr);
            g_assert_nonnull (error);
            ERROR ("%s", error->message);
          }
        else
          {
            gst_message_parse_warning (msg, &error, nullptr);
            g_assert_nonnull (error);
            WARNING ("%s", error->message);
          }
        g_error_free (error);
        gst_object_unref (obj);
        break;
      }
    case GST_MESSAGE_STATE_CHANGED:
      {
        if (player->getPipelineState () == "PAUSED"
            || player->getPipelineState () == "PLAYING")
          {
            player->doStackedActions ();
          }
        break;
      }
    case GST_MESSAGE_BUFFERING:
      {
        TRACE("BUFFERING\n");
        //player->doPreparationActions ();
      }
    default:
      break;
    }
  // gst_object_unref (msg);
  return TRUE;
}

void
PlayerStream::initProperties (set<string> *props)
{
  Property code;
  string defval;
  for (auto name : *props)
    {
      code = Player::getPlayerProperty (name, &defval);
      if (code == Player::PROP_UNKNOWN)
        continue;

      switch (code)
        {
        case PROP_BALANCE:
          _prop.balance = xstrtodorpercent (defval, nullptr);
          break;
        case PROP_BASS:
          _prop.bass = xstrtodorpercent (defval, nullptr);
          break;
        case PROP_FREEZE:
          _prop.freeze = ginga::parse_bool (defval);
          break;
        case PROP_MUTE:
          _prop.mute = ginga::parse_bool (defval);
          break;
        case PROP_SPEED:
          _prop.speed = xstrtod (defval);
          break;
        case PROP_TREBLE:
          _prop.treble = xstrtodorpercent (defval, nullptr);
          break;
        case PROP_VOLUME:
          _prop.volume = xstrtodorpercent (defval, nullptr);
          break;
        default:
          break;
        }
    }
}

void
PlayerStream::doStackedActions ()
{
  while (!_stack_actions.empty ())
    {
      PlayerStreamAction act;
      act = _stack_actions.front ();
      _stack_actions.pop_front ();

      this->doSetProperty (act.code, act.name, act.value);
    }
}

bool
PlayerStream::getFreeze ()
{
  return _prop.freeze;
}


GINGA_NAMESPACE_END