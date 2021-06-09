#include "ParserUserDescription.h"
#include <libxml/parser.h>
#include <unistd.h>
#include <sys/types.h>

#include <iostream>

using namespace std;


map<string,string> ParserUserDescription::parseFile(const char * filename)
{
    map<string,string> _properties;
    xmlDocPtr doc;
    xmlNodePtr cur;
   
    const char * path = filename;

    doc = xmlParseFile(path);

    if (doc == NULL ) 
    {
        fprintf(stderr,"Error to parser user file. \n");
    }
    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr,"Error to parser user file: empty document\n");
        xmlFreeDoc(doc);
    }
    if (xmlStrcmp(cur->name, (const xmlChar *) "UD")) 
    {
        fprintf(stderr,"Document of the wrong type, root node != UD");
        xmlFreeDoc(doc);
    }

    // Retrieving Element Content
    cur = cur->xmlChildrenNode;

    if (cur != NULL) 
    {

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"UserProfile"))|| (!xmlStrcmp(cur->name, (const xmlChar *)"User")))
        {
            cur = cur->xmlChildrenNode;
 
            while (cur != NULL) 
            {

                string key = (char*) cur->name;
                string value = (char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                //char* value = (char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                _properties.insert({key,value});
                cur = cur->next;
            }
        }
        else
       {
            fprintf(stderr,"Document of the wrong type, node != UserProfile and User");
            xmlFreeDoc(doc);
       }
    }
    return _properties;
}
