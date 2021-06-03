#ifndef DEVICE_CALIBRATION_H
#define DEVICE_CALIBRATION_H

#include <string>
#include <list>
#include <time.h>
#include "ginga.h"
#include "aux-ginga.h"
#include "Device.h"
#define SELECTION_DELAY 1   //delay between the time to user perceive the sensory effect and press the ENTER button

GINGA_NAMESPACE_BEGIN

/// @brief Types of sensory effect
enum EffectType
{
    SCENT = 0, ///< Scent effect type.
    LIGHT,     ///< Light effect type.
    WIND,      ///< Wind effect type. 
};

class DeviceCalibration
{
public:

    DeviceCalibration ();
    ~DeviceCalibration ();
    void calculatePrepationTime (EffectType);
    void updateDeviceConfigFile (map<string, Device*>);
    void set_time_start_effect ();
    void set_time_user_confirmation ();

private:
    time_t _time_start_effect;
    time_t _time_user_confirmation; 
    int _scent_preparation_time;
    int _light_preparation_time;
    int _wind_preparation_time;   
    string getDevicePropertyValue (Device*, string); 
};
GINGA_NAMESPACE_END

#endif // DEVICE_CALIBRATION_H
