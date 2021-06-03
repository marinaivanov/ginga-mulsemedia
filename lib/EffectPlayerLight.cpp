#include "EffectPlayerLight.h"
#include "DeviceLight.h"
#include "ParserDeviceFile.h"

static map<string, string >
    parser_syntax_color_table = {
        {"red",  "16777215"},
        {"blue", "10" },
        {"green", "256" },
        {"yellow","65792"},
        {"bluesky", "10"},
        {"darkGreen", "5597999"},
        {"orange","16747520"},
        {"purple","10040012"},
    };

EffectPlayerLight::EffectPlayerLight (Formatter *fmt, Effect *effect) :  EffectPlayer (fmt, effect)
{
    _formatter = fmt;
    _effect = effect;
    _prepared = false;
    connectionFault = false;
    
    Device* dev = fmt->getDevice(fmt,"LightType");
    if (dev != NULL)
    {
        device = new DeviceLight();
        device->setEffectType(dev->getEffectType());
        device->setIntensity("max",dev->getMaxIntensity());
        device->setIntensity("min",dev->getMinIntensity());
        device->setStartPreparationTime(dev->getStartPreparationTime());
        device->setStopPreparationTime(dev->getStopPreparationTime());
        device->setLocator(dev->getLocator());
        device->setUnit(dev->getUnit());
        device->setNumOfLevels(dev->getNumOfLevels());
        device->setAddress(dev->getAddress());
        g_assert_nonnull(device);
    }    

    // Initialize some handled properties.
    static set<string> handled = {"activate", "intensityValue", "color"};
    this->initProperties (&handled);

}

EffectPlayerLight::~EffectPlayerLight ()
{
}

void
EffectPlayerLight::initProperties (set<string> *props)
{
  Property code;
  string defval;
  for (auto name : *props)
    {
      code = EffectPlayer::getEffectPlayerProperty (name, &defval);
      if (code == EffectPlayer::PROP_UNKNOWN)
        continue;

      switch (code)
        {
        case PROP_ACTIVATE:
          _prop.activate = ginga::parse_bool (defval);
          break;
        case PROP_INTENSITY_VALUE:
          _prop.intensity = atoi (defval.c_str());
          break;
        case PROP_COLOR:
          _prop.rgb = defval;
          break;
        default:
          break;
        }
    }
}

void 
EffectPlayerLight::start ()
{
    if (!this->connectionFault){
        if (this->_prepared)
        {
            auto it = parser_syntax_color_table.find(_prop.rgb.c_str());
            string color = "";
            if (it != parser_syntax_color_table.end ())
            {
                color = it->second;
                //Start effect presentation            
                device->activate(color, atoi(getProperty("intensityValue").c_str()));
                EffectPlayer::start ();
            }
            else
            {
                size_t pos = 0;
                string rgbVector[3] = {"","",""};
                std::string token;
                int i = 0;
                string rgb = _prop.rgb;
                while ((pos = rgb.find(",")) != std::string::npos) {
                    rgbVector[i] = rgb.substr(0, pos);
                    i++;
                    rgb.erase(0, pos + 1);
                }
                rgbVector[i] = rgb;
                
                if (rgbVector[0]!= "" && rgbVector[1]!= "" && rgbVector[2]!= "")
                {
                    int r,g,b, finalColor;
                    r = atoi(rgbVector[0].c_str())*65536;
                    g = atoi(rgbVector[1].c_str())*256;
                    b = atoi(rgbVector[2].c_str());
                    finalColor = r + g + b;
                    //Start effect presentation            
                    device->activate(to_string(finalColor), atoi(getProperty("intensityValue").c_str()));
                    EffectPlayer::start ();
                }
                else
                {
                    g_print("Light color not found!\n");
                }
            }
        }
        else
        {
            this->startPreparation();
            this->start ();            
        }   
    } 
}

void 
EffectPlayerLight::stop ()
{
    g_assert_nonnull(device);
    if (device->isActive())
        device->deactivate(getProperty("scent"));
    EffectPlayer::stop ();      
}

void
EffectPlayerLight::prepareStop ()
{
}

void 
EffectPlayerLight::pause ()
{

}

void 
EffectPlayerLight::resume ()
{

}

void 
EffectPlayerLight::startPreparation ()
{
    TRACE("Start Preparation");
    g_assert_nonnull(device);
    this->connectionFault = !device->connectDevice();
    if (this->connectionFault)
        device->disconnect();
    else
        this->_prepared = true;
        
}

bool 
EffectPlayerLight::doSetProperty (Property code, const string &name, const string &value)
{
    switch (code)
    {
        case PROP_ACTIVATE:
            _prop.activate = ginga::parse_bool (value);
            break;
        case PROP_INTENSITY_VALUE:
            _prop.intensity = atoi (value.c_str());
            this->device->setLightIntensity(value);
            break;
        case PROP_COLOR:
            _prop.rgb = value;
            auto it = parser_syntax_color_table.find(value);
            string color = "";
            if (it != parser_syntax_color_table.end ())
                color = it->second;
            this->device->setColor(color);
            break;
    }
    return true;
}

