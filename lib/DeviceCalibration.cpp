#include "DeviceCalibration.h"
#include <libxml/xmlwriter.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <pwd.h>

#define MY_ENCODING "ISO-8859-1"

list<string> tags = {"id","effectType","minIntensity", "maxIntensity","locator","unit","numOfLevels", "ipAddress"};

DeviceCalibration::DeviceCalibration ()
{
    _time_start_effect = 0;
    _time_user_confirmation = 0;
    _scent_preparation_time = 0;
    _light_preparation_time = 0;
    _wind_preparation_time = 0;
}

DeviceCalibration::~DeviceCalibration ()
{

}

void 
DeviceCalibration::calculatePrepationTime (EffectType type)
{
    switch (type)
    {
    case EffectType::SCENT:
        if (_time_user_confirmation < _time_start_effect)
            _scent_preparation_time = 0;
        else
            _scent_preparation_time = _time_user_confirmation - _time_start_effect;
        break;
    
    default:
        break;
    }
}

void 
DeviceCalibration::updateDeviceConfigFile (map<string, Device*> devices)
{

    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, devDescriptionNode = NULL, supValues = NULL, prepTimeNode = NULL;/* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */
    char buff[256];
    int i, j;

    LIBXML_TEST_VERSION;

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "devices");
    xmlDocSetRootElement(doc, root_node);

    //Devicedescription
    auto iter = devices.begin();
    
    while (iter != devices.end()) {
        devDescriptionNode = xmlNewChild(root_node,NULL,  BAD_CAST "deviceDescription",NULL);
        string tag, value;

        for (list<string>::iterator it = tags.begin(); it != tags.end(); it++)
        {
            value = "";
            tag = *it;
            value = getDevicePropertyValue((iter->second),tag);
            if (!value.empty())
                xmlNewChild(devDescriptionNode, NULL, BAD_CAST tag.c_str(), BAD_CAST value.c_str());
        }

        //supportedValues
        supValues = xmlNewChild(devDescriptionNode,NULL,  BAD_CAST "supportedValues",NULL);
        list<string> values = (iter->second)->getSupportedValues();
        for (list<string>::iterator itVal = values.begin(); 
            itVal != values.end(); itVal++)
        {
            value = *itVal;
            xmlNewChild(supValues, NULL, BAD_CAST "value", BAD_CAST value.c_str());
        
        }

        //preparationTime
        prepTimeNode = xmlNewChild(devDescriptionNode,NULL,  BAD_CAST "preparationTime",NULL);
        if (iter->first == "ScentType" && _scent_preparation_time > 0)
        {
            value = to_string(_scent_preparation_time);
        }
        else
        {
            value = to_string((iter->second)->getStartPreparationTime());
        }
        string valueStop = to_string((iter->second)->getStopPreparationTime());
        xmlNewChild(prepTimeNode, NULL, BAD_CAST "start", BAD_CAST value.c_str());
        xmlNewChild(prepTimeNode, NULL, BAD_CAST "stop", BAD_CAST valueStop.c_str());
        

        iter++;
    }

    const char * filename = "/gingaFiles/sensoryDevices/config.xml";
    uid_t uid = getuid();
    char * home_dir = getpwuid( uid )->pw_dir;

    const char * path = strcat(home_dir,filename);

    xmlSaveFormatFileEnc(path, doc, "UTF-8", 1);

    xmlFreeDoc(doc);

    xmlCleanupParser();

}

void 
DeviceCalibration::set_time_start_effect ()
{
    time(&_time_start_effect);
}

void 
DeviceCalibration::set_time_user_confirmation ()
{
    time(&_time_user_confirmation);
}

string 
DeviceCalibration::getDevicePropertyValue (Device* dev, string prop)
{
    if (prop == "id")
    {
        return dev->getId();
    }        
    else if (prop == "effectType")
    {
        return dev->getEffectType();
    }
    else if (prop == "locator")
    {
        return dev->getLocatorAsString();
    }
    else if (prop == "unit")
    {
        return dev->getUnit();
    }
    else if (prop == "minIntensity")
    {
        return to_string(dev->getMinIntensity());
    }
    else if (prop == "maxIntensity")
    {
        return to_string(dev->getMaxIntensity());
    }
    else if (prop == "numOfLevels")
    {
        return to_string(dev->getNumOfLevels());
    }
    else if (prop ==  "ipAddress")
    {
        return dev->getAddress();
    }
    return "";
    
}

