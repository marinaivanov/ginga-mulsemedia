#ifndef PARSER_DEVICE_FILE_H
#define PARSER_DEVICE_FILE_H

#include "aux-ginga.h"
#include <libxml/parser.h>
#include <libxml/xmlreader.h>

GINGA_NAMESPACE_BEGIN

class ParserUserDescription
{

  public:
    static map<string,string> parseFile (const char *);

};

GINGA_NAMESPACE_END

#endif // PARSER_DEVICE_FILE_H