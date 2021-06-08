#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "StreamFile.h"

#include <iostream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::setw;


StreamFile:StreamFile()
{

}
void StreamFile:setUserPropList(json propeties)
{

}
json StreamFile:getUserPropList()
{

}
void StreamFile:postUserPropList(json );
{
	
}

xmlDoc * UserContextManager::loadUsersProfile(const string &path, string *errmsg)
//void UserContextManager::loadUsersProfile()
{

  xmlDoc *xml;
  Document *doc;
  string uri = path;

  // Makes the path absolute based in the current dir
  if (!xpathisabs (path))
    uri = xpathmakeabs (path);

  uri = xurifromsrc (uri, "");

  xml = xmlReadFile (uri.c_str (), nullptr, PARSER_LIBXML_FLAGS);
  if (unlikely (xml == nullptr))
    {
      //tryset (errmsg, xmlGetLastErrorAsString ());

      return nullptr;
    }

 // doc = process (xml, width, height, errmsg);
 // xmlFreeDoc (xml);
 return xml;
}



