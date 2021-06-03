#include "ParserDeviceFile.h"
#include <libxml/parser.h>
#include "DeviceScent.h"
#include "DeviceLight.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>

using namespace std;

enum EffectType // known effect type
  {
    SCENT_EFFECT = 0,
    WIND_EFFECT,
    LIGHT_EFFECT,
    VIBRATION_EFFECT,
    TASTE_EFFECT,
    FOG_EFFECT,
    TACTILE_EFFECT,
    RIGIDBODYMOTION_EFFECT,
    FLASH_EFFECT,
    TEMPERATURE_EFFECT      
  };

static map<string,int> effect_type_map{
    {"scent",SCENT_EFFECT},
    {"wind",WIND_EFFECT},
    {"light",LIGHT_EFFECT},
    {"vibration",VIBRATION_EFFECT},
    {"taste",TASTE_EFFECT},
    {"fog",FOG_EFFECT},
    {"tactile",TACTILE_EFFECT},
    {"rigidbodymotion",RIGIDBODYMOTION_EFFECT},
    {"flash", FLASH_EFFECT},
    {"temperature", TEMPERATURE_EFFECT}
};

map<string,Device*> 
ParserDeviceFile::parseFile(const char * filename)
{
    map<string,Device*> _devices;
    xmlDocPtr doc;
    xmlNodePtr cur;
    
    uid_t uid = getuid();
    char * home_dir = getpwuid( uid )->pw_dir;

    const char * path = strcat(home_dir,filename);
    doc = xmlParseFile(path);

    if (doc == NULL ) 
    {
        fprintf(stderr,"Error to parser device config file. \n");
    }
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        fprintf(stderr,"Error to parser device config file: empty document\n");
        xmlFreeDoc(doc);
    }
    if (xmlStrcmp(cur->name, (const xmlChar *) "devices")) 
    {
        fprintf(stderr,"Document of the wrong type, root node != devices");
        xmlFreeDoc(doc);
    }
    
    // Retrieving Element Content
    cur = cur->xmlChildrenNode;
    while (cur != NULL) 
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"deviceDescription")))
        {
            Device* _dev = parseDeviceDescription (doc, cur);
            g_assert_nonnull(_dev);
            _devices.insert({_dev->getEffectType(),_dev});
        }
        cur = cur->next;
    }
    
    return _devices;
}

Device*
ParserDeviceFile::parseDeviceDescription(xmlDocPtr doc,xmlNodePtr cur)
{
    xmlChar *deviceDescription;
    cur = cur->xmlChildrenNode;
    Device* _dev = new Device();
    list<string> supportedValues;
    string id, effectType, minIntensity, maxIntensity, zerothOrderDelayTime, locator, 
        unit, numOfLevels;
    while (cur != NULL) 
    {
        char* value = (char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
       
        if (!xmlStrcmp(cur->name, (const xmlChar *)"id")) 
        {
            _dev->setId(value);            
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"effectType")) 
        {
            _dev->setEffectType(value);
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"minIntensity")) 
        {
            _dev->setIntensity("min",atoi(value));
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"maxIntensity")) 
        {
            _dev->setIntensity("max",atoi(value));
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"preparationTime")) 
        {
            parsePreparationTime(_dev,doc,cur);
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"locator"))
        {
            _dev->setLocator(value);
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"unit"))
        {
            _dev->setUnit(value);
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"numOfLevels"))
        {
            _dev->setNumOfLevels(atoi(value));
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"supportedValues"))
        {
            supportedValues = parseSupportedValues (doc, cur);
            _dev->setSupportedValues(supportedValues);
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *)"ipAddress"))
        {
            _dev->setAddress(value);
        }
        cur = cur->next;
    }

    return _dev;
}

void 
ParserDeviceFile::parsePreparationTime(Device* dev,xmlDocPtr doc,xmlNodePtr cur)
{
    list<string> times;
    cur = cur->xmlChildrenNode;

    while (cur != NULL) 
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *)"start"))
        {
            char* value = (char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            g_assert_nonnull(value);
            dev->setStartPreparationTime(atoi(value));   
        }
        else if  (!xmlStrcmp(cur->name, (const xmlChar *)"stop"))
        {
            char* value = (char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            g_assert_nonnull(value);
            dev->setStopPreparationTime(atoi(value));   
        }
        cur = cur->next;
    }
}

list<string>
ParserDeviceFile::parseSupportedValues(xmlDocPtr doc,xmlNodePtr cur)
{
    list<string> values;
    xmlChar *supportedValue;
    cur = cur->xmlChildrenNode;

    while (cur != NULL) 
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *)"value"))
        {
            char* value = (char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            g_assert_nonnull(value);
            values.push_back(value);        
        }
        cur = cur->next;
    }

    return values;
}