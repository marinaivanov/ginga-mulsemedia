#ifndef EFFECTPLAYER_LIGHT_H
#define EFFECTPLAYER_LIGHT_H

#include "EffectPlayer.h"
#include "DeviceLight.h"

GINGA_NAMESPACE_BEGIN
class Effect;
class EffectPlayerLight : public EffectPlayer
{

public:
  EffectPlayerLight (Formatter *, Effect *);
  ~EffectPlayerLight ();
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
    string rgb;
  } _prop;
  
  bool connectionFault;
  void doPreparationActions ();
  DeviceLight* device;
  void initProperties (set<string> *);
  
};

GINGA_NAMESPACE_END

#endif // EFFECTPLAYER_Light_H
