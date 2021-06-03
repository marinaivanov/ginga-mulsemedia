#include "aux-ginga.h"
#include "aux-gl.h"

#include "EffectPlayer.h"
#include "Effect.h"
#include "EffectPlayerScent.h"
#include "EffectPlayerLight.h"

GINGA_NAMESPACE_BEGIN

// Property table.
typedef struct EffectPlayerPropertyInfo
{
  EffectPlayer::Property code; // property code
  bool init;             // whether it should be initialized
  string defval;         // default value
} EffectPlayerPropertyInfo;

static map<string, EffectPlayerPropertyInfo> effect_player_property_map = {
  {"duration", {EffectPlayer::PROP_DURATION, true, "indefinite"} },
  {"location", {EffectPlayer::PROP_LOCATION, false, "0:0:0"} },
  {"time", {EffectPlayer::PROP_TIME, false, "indefinite"} },
  {"activate", {EffectPlayer::PROP_ACTIVATE, true, "true"} },
  {"type", {EffectPlayer::PROP_TYPE, true, ""} },
  {"intensityValue", {EffectPlayer::PROP_INTENSITY_VALUE, true, "10"} },
  {"intensityRange", {EffectPlayer::PROP_INTENSITY_RANGE, false, "indefinite"} },
  {"azimuthal", {EffectPlayer::PROP_AZIMUTHAL, false, "indefinite"} },
  {"polar", {EffectPlayer::PROP_POLAR, false, "indefinite"} },
  {"scent", {EffectPlayer::PROP_SCENT,true,"indefinite"}},
  {"color", {EffectPlayer::PROP_COLOR,true,"indefinite"}}

};

// Public.

EffectPlayer::EffectPlayer (Formatter *formatter, Effect *effect)
{
  g_assert_nonnull (formatter);
  _formatter = formatter;

  g_assert_nonnull (effect);
  _effect = effect;
  _id = effect->getId ();

  _state = SLEEPING;
  _time = 0;
  _prepared = false;
  this->resetProperties ();
}

EffectPlayer::~EffectPlayer ()
{
  //delete _animator;
  //if (_surface != nullptr)
  //  cairo_surface_destroy (_surface);
  //if (_gltexture)
  //  GL::delete_texture (&_gltexture);
  _properties.clear ();
}

EffectPlayer::State
EffectPlayer::getState ()
{
  return _state;
}

Time
EffectPlayer::getTime ()
{
  return _time;
}

void
EffectPlayer::incTime (Time inc)
{
  _time += inc;
}

Time
EffectPlayer::getDuration ()
{
  return _prop.duration;
}

void
EffectPlayer::setDuration (Time duration)
{
  _prop.duration = duration;
}

bool
EffectPlayer::getPrepared ()
{
  return _prepared;
}

void
EffectPlayer::setPrepared (bool prepared)
{
  _prepared = prepared;
}

void
EffectPlayer::start ()
{
  g_assert (_state != OCCURRING);
  _state = OCCURRING;
  _time = 0;
  
    //_eos = false;
  //this->reload ();
  //_animator->scheduleTransition ("start", &_prop.rect, &_prop.bgColor,
  //                               &_prop.alpha, &_crop);
}

void
EffectPlayer::prepareStop ()
{
  g_assert (_state != SLEEPING);
}

void
EffectPlayer::stop ()
{
  g_assert (_state != SLEEPING);
  _state = SLEEPING;
  this->resetProperties ();
}

void
EffectPlayer::pause ()
{
  g_assert (_state != PAUSED && _state != SLEEPING);
  _state = PAUSED;
}

void
EffectPlayer::resume ()
{
  g_assert (_state == PAUSED);
  _state = OCCURRING;
}

void
EffectPlayer::startPreparation ()
{
  g_assert (_state != PREPARING);
  _state = PREPARING;  
}

string
EffectPlayer::getProperty (string const &name)
{
  return (_properties.count (name) != 0) ? _properties[name] : "";
}

EffectPlayer::Property
EffectPlayer::getEffectPlayerProperty (const string &name, string *defval)
{
  map<string, EffectPlayerPropertyInfo>::iterator it;
  EffectPlayerPropertyInfo *info;
  string _name = name;
 
  if ((it = effect_player_property_map.find (_name)) == effect_player_property_map.end ())
    {
          return PROP_UNKNOWN;
    }
  info = &it->second;
  tryset (defval, info->defval);
  return info->code;
}

void
EffectPlayer::setProperty (const string &name, const string &value)
{
  EffectPlayer::Property code;
  bool use_defval;
  string defval;
  string _value;

  //if (name == "transIn" || name == "transOut")
  //  _animator->setTransitionProperties (name, value);

  use_defval = false;
  _value = value;

  code = EffectPlayer::getEffectPlayerProperty (name, &defval);
  if (code == EffectPlayer::PROP_UNKNOWN)
    goto done;
  

  if (_value == "")
    {
      use_defval = true;
      _value = defval;
    }

  if (unlikely (!this->doSetProperty (code, name, _value)))
    {
      ERROR ("property '%s': bad value '%s'", name.c_str (),
             _value.c_str ());
    }

  if (use_defval) // restore value
    _value = "";

done:
  _properties[name] = _value;
  return;
}

void
EffectPlayer::resetProperties ()
{
  for (auto it : effect_player_property_map)
    if (it.second.init)
      this->setProperty (it.first, "");
  _properties.clear ();
}

void
EffectPlayer::resetProperties (set<string> *props)
{
  for (auto name : *props)
    this->setProperty (name, "");
}

/*void
Player::schedulePropertyAnimation (const string &name, const string &from,
                                   const string &to, Time dur)
{
  _animator->schedule (name, from, to, dur);
}*/

void EffectPlayer::sendKeyEvent (unused (const string &key), unused (bool press))
{
}

// Public: Static.

/*EffectPlayer::Property
EffectPlayer::getPlayerProperty (const string &name, string *defval)
{
  map<string, EffectPlayerPropertyInfo>::iterator it;
  EffectPlayerPropertyInfo *info;
  string _name = name;
 
  if ((it = effect_player_property_map.find (_name)) == effect_player_property_map.end ())
    {
      map<string, string>::iterator italias;
      if ((italias = player_property_aliases.find (_name))
          == player_property_aliases.end ())
        {
          tryset (defval, "");
          return PROP_UNKNOWN;
        }
      _name = italias->second;
      it = player_property_map.find (_name);
      g_assert (it != player_property_map.end ());
    }
  info = &it->second;
  tryset (defval, info->defval);
  return info->code;
}*/

EffectPlayer *
EffectPlayer::createEffectPlayer (Formatter *formatter, Effect *effect,
                      const string &type)
{
  EffectPlayer *effectPlayer;
  string effectType = type;
    
  g_assert_nonnull (formatter);
  
  effectPlayer = nullptr;

 if (effectType == "")
    {
      ERROR ("Effect type could not be empty.");
    }

  if (effectType == "ScentType")
    {
      effectPlayer = new EffectPlayerScent(formatter, effect);
    }
  else if (effectType == "TasteType")
    {
      ERROR_NOT_IMPLEMENTED ("Taste Effect is not supported");
    }
  else if (effectType == "FogType")
    {
      ERROR_NOT_IMPLEMENTED ("Fog Effect is not supported");
    }
  else if (effectType == "VibrationType")
    {
      ERROR_NOT_IMPLEMENTED ("Vibration Effect is not supported");
    }
  else if (effectType == "WindType")
    {
      ERROR_NOT_IMPLEMENTED ("Wind Effect is not supported");
    }
  else if (effectType == "TactileType")
    {
      ERROR_NOT_IMPLEMENTED ("Tactile Effect is not supported");
    }
  else if (effectType == "RigidbodymotionType")
    {
      ERROR_NOT_IMPLEMENTED ("Rigid Body Motion Effect is not supported");
    }
  else if (effectType == "LightType")
    {
      effectPlayer = new EffectPlayerLight(formatter, effect);
    }
  else if (effectType == "FlashType")
    {
      ERROR_NOT_IMPLEMENTED ("Flash Effect is not supported");
    }
  else if (effectType == "TemperatureType")
    {
      ERROR_NOT_IMPLEMENTED ("Temperature Effect is not supported");
    }
  else if (effectType == "SprayingType")
    {
      ERROR_NOT_IMPLEMENTED ("Spraying Effect is not supported");
    }
  else
    {
      effectPlayer = new EffectPlayer (formatter, effect);
      WARNING ("unknown effect type '%s': creating an empty player",
                   effectType.c_str ());
    }
  
  g_assert_nonnull (effectPlayer);
  return effectPlayer;
}

// Protected.

bool
EffectPlayer::doSetProperty (Property code, unused (const string &name),
                       const string &value)
{
  switch (code)
    {
    case PROP_LOCATION:
      {
        
        break;
      }
    case PROP_ACTIVATE:
      {
        _prop.activate = ginga::parse_bool (value);
        break;
      }
    case PROP_DURATION:
      {
        if (value == "indefinite")
          _prop.duration = GINGA_TIME_NONE;
        else
          _prop.duration = ginga::parse_time (value);
        break;
      }
    case PROP_TYPE:
      {
        _prop.type = value;
        break;
      }
    case PROP_INTENSITY_VALUE:
      {
        _prop.intensityValue = value;
        break;
      }
    case PROP_INTENSITY_RANGE:
      {
        //_prop.maxIntensity = ;
        //_prop.minIntensity = ;
        break;
      }
    case PROP_AZIMUTHAL:
     {
        _prop.azimuthal = value;
        break;
     }
    case PROP_POLAR:
     {
        _prop.polar = value;
        break;
     } 
    case PROP_SCENT:
     {
       _prop.scent = value;
       break;
     }
    case PROP_COLOR:
     {
       _prop.color = value;
       break;
     }
    default:
      {
        break;
      }
    }
  return true;
}

bool
EffectPlayer::isFocused ()
{
  return false;
  //return _prop.focusIndex != "" && _prop.focusIndex == _currentFocus;
}

GINGA_NAMESPACE_END
