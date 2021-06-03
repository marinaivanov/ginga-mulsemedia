#ifndef EFFECT_H
#define EFFECT_H

#include "Object.h"
#include "EffectPlayer.h"

GINGA_NAMESPACE_BEGIN

class Effect : public Object
{
public:
  explicit Effect (const string &);
  virtual ~Effect ();

  // Object:
  string getObjectTypeAsString () override;
  string toString () override;
  void setProperty (const string &, const string &, Time dur = 0) override;
  //void sendKey (const string &, bool) override;
  void sendTick (Time, Time, Time) override;
  bool beforeTransition (Event *, Event::Transition) override;
  bool afterTransition (Event *, Event::Transition) override;

  // Effect:
  virtual bool isFocused ();
  virtual Event * getCurrentPreparationEvent ();
  
  //virtual bool getZ (int *, int *);
  //TO DO: virtual bool getIntensity (int *, int *);
  //virtual void redraw (cairo_t *);

protected:
  EffectPlayer *_player; // underlying player
  Event * _currentPreparationEvent;
  string type;

  void createEffectPlayer ();
  void doStop () override;
};

GINGA_NAMESPACE_END

#endif // EFFECT_H