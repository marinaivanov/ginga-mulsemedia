#ifndef PARSER_DEVICE_FILE_H
#define PARSER_DEVICE_FILE_H

#include "Device.h"
#include "aux-ginga.h"
#include <libxml/parser.h>
#include <libxml/xmlreader.h>

GINGA_NAMESPACE_BEGIN

class ParserDeviceFile
{

public:
  static map<string,Device*> parseFile (const char *);
  static void processDeviceNode (xmlNode *, Device* );
  static Device* parseDeviceDescription (xmlDocPtr ,xmlNodePtr);
  static list<string> parseSupportedValues(xmlDocPtr ,xmlNodePtr);
  static void parsePreparationTime(Device* dev,xmlDocPtr doc,xmlNodePtr cur);
  //static void processNode(xmlTextReaderPtr , Device* );

};

GINGA_NAMESPACE_END

#endif // PARSER_DEVICE_FILE_H