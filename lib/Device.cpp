#include "Device.h"
#include <string.h>
#include "aux-ginga.h"

Device::Device()
{
    _effectType = "";
    _maxIntensity = 0;
    _minIntensity = 0;
    _startPreparationTime = 0;  
    _stopPreparationTime = 0;         
    _updateIntensityLevelTime = 0;         
    _numOfLevels = 0;
    _unit = "";
    _active = false;
    _address = "";
    locator_string = "";
}

Device::~Device()
{
    
}

void Device::activate(string , int)
{

}

void Device::deactivate(string )
{

}

bool Device::connectDevice()
{
    return true;
}

void Device::disconnect()
{
    
}

void 
Device::setId (string id )
{
    _id = id;
}

void 
Device::setEffectType (string type)
{
    _effectType = type;
}

void 
Device::setIntensity (string name, int value)
{
    if (name.compare("max")==0)
    {
        _maxIntensity = value;
    }
    else if (name.compare("min")==0)
    {
        _minIntensity = value;
    }
}

void 
Device::setStartPreparationTime (int time)
{
    _startPreparationTime = time;
}

void 
Device::setStopPreparationTime (int time)
{
    _stopPreparationTime = time;
}

void 
Device::setUpdateIntensityLevelTime (int time){
    _updateIntensityLevelTime = time;   
} 

void 
Device::setLocator (string locator)
{
    locator_string = locator;
    std::size_t found = locator.find_first_of(":");
    std::size_t initialPosition = found + 1;

    if (found < locator.length())
    {
        _locator.x_axis = locator.substr(0,found);
    }

    found = locator.find_first_of(":",found+1);
    if (found < locator.length())
    {
        _locator.y_axis = locator.substr(initialPosition,found-initialPosition);
    }

    if (found+1 < locator.length())
    {
        _locator.z_axis = locator.substr(found+1,locator.length());
    }
}

void 
Device::setNumOfLevels (int levels)
{
    _numOfLevels = levels;
}

void 
Device::setUnit (string unit)
{
    _unit = unit;
}

void 
Device::setAddress(string address)
{
    _address = address;
}

void 
Device::setSupportedValues(list<string> listValues)
{
    _supportedValues = listValues;
}

void 
Device::addSupportedValue (string value)
{
    _supportedValues.push_back(value);
}

string 
Device::getId()
{
    return _id;
}

string 
Device::getEffectType()
{
    return _effectType;
}

int
Device::getMaxIntensity()
{
    return _maxIntensity;
}

int 
Device::getMinIntensity()
{
    return _minIntensity;
}

int 
Device::getStartPreparationTime ()
{
    return _startPreparationTime;
}

int 
Device::getStopPreparationTime ()
{
    return _stopPreparationTime;
}

int 
Device::getUpdateIntensityLevelTime()
{
    return _updateIntensityLevelTime;
}

Locator 
Device::getLocator ()
{
    return _locator;
}

int 
Device::getNumOfLevels ()
{
    return _numOfLevels;
}

string 
Device::getUnit ()
{
    return _unit;
}

list<string> 
Device::getSupportedValues ()
{
    return _supportedValues;
}

string 
Device::getAddress()
{
    return _address;
}

bool
Device::isActive ()
{
    return _active;
}

string
Device::getDeviceTypeAsString ()
{
    return "Device";
}

string
Device::getLocatorAsString()
{
    return locator_string;
}