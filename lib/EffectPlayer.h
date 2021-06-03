#ifndef EFFECT_PLAYER_H
#define EFFECT_PLAYER_H

#include "Formatter.h"
#include "DeviceCalibration.h"

GINGA_NAMESPACE_BEGIN

//class Formatter;
class Effect;
class EffectPlayer
{
public:
  enum State
  {
    SLEEPING = 1, // stopped
    OCCURRING,    // playing
    PAUSED,       // paused
    PREPARING,    // preparing
  };

  enum Property // known properties
  {
    PROP_UNKNOWN = 0,
    PROP_DURATION,
    PROP_LOCATION,
    PROP_TIME,
    PROP_TYPE,
    PROP_ACTIVATE,
    PROP_INTENSITY_VALUE,
    PROP_INTENSITY_RANGE,
    PROP_AZIMUTHAL,
    PROP_POLAR,
    PROP_SCENT,
    PROP_COLOR    
  };

  /*enum CommandType // known commands
  {
    ACTIVATE = 0,
    DEACTIVATE,
    UP_INTENSITY,
    DOWN_INTENSITY   
  };*/

  EffectPlayer (Formatter *, Effect *);
  virtual ~EffectPlayer ();

  State getState ();
  
  Time getTime ();
  void incTime (Time);
  bool isFocused ();

  Time getDuration ();
  void setDuration (Time);

  bool getPrepared ();
  //bool getEOS ();
  void setPrepared (bool);
  //void setEOS (bool);

  virtual void start ();
  virtual void stop ();
  virtual void pause ();
  virtual void resume ();
  virtual void startPreparation ();
  virtual void prepareStop ();

  string getProperty (const string &);
  void setProperty (const string &, const string &);
  void resetProperties ();
  void resetProperties (set<string> *);
  //void schedulePropertyAnimation (const string &, const string &,
  //                                const string &, Time);
  //virtual void reload ();
  //virtual void redraw (cairo_t *);

  virtual void sendKeyEvent (const string &, bool);

  // For now, only for the lua player (which reimplements it).
  virtual void
  sendPresentationEvent (const string &, const string &)
  {
  }

  // Static.
  //static string getCurrentFocus ();
  //static void setCurrentFocus (const string &);
  static Property getEffectPlayerProperty (const string &, string *);
  static EffectPlayer *createEffectPlayer (Formatter *, Effect *, const string &type = "");

protected:
  Formatter *_formatter;     // formatter handle
  Effect *_effect;             // associated effect object
  string _id;                // id of the associated effect object
  State _state;              // current state
  Time _time;                // playback time
  //bool _eos;                 // true if content was exhausted
  bool _prepared;            // true if effect is prepared 
  
  
  map<string, string> _properties; // property table
  struct
  {
    Time duration;     // explicit duration
  //  bool debug;        // true if debugging mode is on
    bool activate;      // true if activate
    string type;       // content mime-type
    string intensityValue;     //effect intensity
    string maxIntensity;       //maximum intensity supported by the effect
    string minIntensity;       //minimum intensity supported by the effect
    string azimuthal;
    string polar;
    string scent;
    string color;
  //  string uri;        // content URI
  } _prop;

  virtual bool doSetProperty (Property, const string &, const string &);
 
};

GINGA_NAMESPACE_END

#endif // EFFECT_PLAYER_H
