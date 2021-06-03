#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <list>

using std::string;
using std::list;


typedef struct Locator
  {
    string x_axis;
    string y_axis;
    string z_axis;
    string azimuthal;
    string polar;
  }Locator;

class Device
{
  public:

    Device();
    ~Device();

    void activate(string , int); 
    void deactivate(string );

    bool connectDevice();
    void disconnect();

    void setId (string );
    void setEffectType (string );
    void setIntensity (string , int );
    void setStartPreparationTime (int );
    void setStopPreparationTime (int );
    void setUpdateIntensityLevelTime (int); 
    void setLocator (string);
    void setNumOfLevels (int);
    void setUnit (string );
    void setAddress (string );
    void addSupportedValue (string );
    void setSupportedValues(list<string> );

    string getId();
    string getEffectType();
    int getMaxIntensity();
    int getMinIntensity();
    int getStopPreparationTime ();
    int getStartPreparationTime ();
    int getUpdateIntensityLevelTime(); 
    Locator getLocator ();
    int getNumOfLevels ();
    string getUnit ();
    string getAddress ();
    list<string> getSupportedValues ();
    string getLocatorAsString();
    string getDeviceTypeAsString ();
    bool isActive ();   //indicates if the device is active or no

  protected:
    string _id;
    string _effectType;
    int _maxIntensity;
    int _minIntensity;
    int _startPreparationTime;         //time necessary to prepare the start of device
    int _stopPreparationTime;         //time necessary to prepare the end device
    int _updateIntensityLevelTime;          //time necessary to modify the effect intensity (one level up or one down)
    Locator _locator;
    string locator_string;
    int _numOfLevels;
    string _unit;
    list<string> _supportedValues;       //only applied to light and scent devices
    bool _active;
    string _address;
};

#endif // DEVICE_H