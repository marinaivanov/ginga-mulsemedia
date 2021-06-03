#ifndef EFFECTPLAYER_SCENT_H
#define EFFECTPLAYER_SCENT_H

#include "EffectPlayer.h"
#include "DeviceScent.h"
#include "DeviceCalibration.h"

GINGA_NAMESPACE_BEGIN
class Effect;
class EffectPlayerScent : public EffectPlayer
{

public:
  EffectPlayerScent (Formatter *, Effect *);
  ~EffectPlayerScent ();
  void start () override;
  void stop () override;
  void pause () override;
  void resume () override;
  void startPreparation () override;
  void prepareStop () override;
  
protected:
  bool doSetProperty (Property, const string &, const string &) override;
  
private:
  //list<Actuator> actuatorDevices;
  struct
  {
    bool activate;      // true if effect is activate
    int intensity;
    string scent;
  } _prop;
  
  void doPreparationActions ();
  DeviceScent* device;

};

GINGA_NAMESPACE_END

#endif // EFFECTPLAYER_SCENT_H
