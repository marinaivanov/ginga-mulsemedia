#include "EffectPlayerScent.h"
#include "DeviceScent.h"
#include "ParserDeviceFile.h"

EffectPlayerScent::EffectPlayerScent (Formatter *fmt, Effect *effect) :  EffectPlayer (fmt, effect)
{
    _formatter = fmt;
    _effect = effect;
    _prepared = false;

    Device* dev = fmt->getDevice(fmt,"ScentType");
    if (dev != NULL)
    {
        device = new DeviceScent();
        device->setEffectType(dev->getEffectType());
        device->setIntensity("max",dev->getMaxIntensity());
        device->setIntensity("min",dev->getMinIntensity());
        //device->setStartPreparationTime(dev->getStartPreparationTime());
        device->setLocator(dev->getLocator());
        device->setUnit(dev->getUnit());
        device->setNumOfLevels(dev->getNumOfLevels());
        device->setLogin(dev->getLogin());
        device->setPassword(dev->getPassword());
        g_assert_nonnull(device);
    }    
}

EffectPlayerScent::~EffectPlayerScent ()
{
}

void 
EffectPlayerScent::start ()
{
    if (this->_prepared)
    {
        //Start effect presentation        
        device->activate(getProperty("scent"), atoi(getProperty("intensityValue").c_str()));
        EffectPlayer::start ();
    }
    else
    {
        this->startPreparation();        
        if(this->connectionAttempts < 3)
            this->start ();        
    }    
}

void 
EffectPlayerScent::stop ()
{
    g_assert_nonnull(device);
    if (device->isActive())
        device->deactivate(getProperty("scent"));
    EffectPlayer::stop ();      
}

void
EffectPlayerScent::prepareStop ()
{  
}

void 
EffectPlayerScent::pause ()
{

}

void 
EffectPlayerScent::resume ()
{

}

void 
EffectPlayerScent::startPreparation ()
{
    g_assert_nonnull(device);
    if (device->connectDevice())
        this->_prepared = true;
    else
        this->connectionAttempts++;
}

bool 
EffectPlayerScent::doSetProperty (Property code, const string &name, const string &value)
{
    switch (code)
    {
        case PROP_ACTIVATE:
            _prop.activate = ginga::parse_bool (value);
            break;
        case PROP_INTENSITY_VALUE:
            _prop.intensity = atoi (value.c_str());
            break;
        case PROP_SCENT:
            _prop.scent = value;
            break;
    }
    return true;
}
