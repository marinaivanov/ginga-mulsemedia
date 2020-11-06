
#include "Formatter.h"
#include "MediaSettings.h"
#include "UserContextManager.h"

using std::string;



GINGA_NAMESPACE_BEGIN
/// Flags to LibXML parser.
#define PARSER_LIBXML_FLAGS (XML_PARSE_NOERROR | XML_PARSE_NOWARNING)
UserContextManager::UserContextManager (Ginga *ginga)
{
	this->ginga = ginga;
}

/**
 * @brief starts all the elements to the configuration of the user.
 * @todo It will be necessary to get all the element referenced with the user to load them features.
 *       This elements to be in the document in the list.
 */
void UserContextManager::start()
{
	map<string,user> userList = (((Formatter *)ginga)->getDocument())->getUsers();
	map<string,profile> profileList = (((Formatter *)ginga)->getDocument())->getProfiles();
	map<string,MediaSettings *> userSettingsList = (((Formatter *)ginga)->getDocument())->getuserSettings();
    
	printf("\nUsuários:\n ");
	for (auto it=userList.begin(); it!=userList.end(); ++it)
	{
		printf("\nId: %s", it->second.id.c_str());
		printf("\nprofile: %s", it->second.profile.c_str());
		printf("\nsrc: %s", it->second.src.c_str());
		string path = it->second.src.c_str();
	 	string uri = path;
		// Makes the path absolute based in the current dir
  		if (!xpathisabs ( path))
    		uri = xpathmakeabs (path);
		uri = xurifromsrc (uri, "");
  		printf("\nsrc: %s", uri.c_str());
	
		printf("\ntype: %s\n", it->second.type.c_str());

		string type = it->second.type.c_str();
		int pos = type.rfind(":");
		string str = type.substr(0,pos -1);
		printf("\nInicio: %s\n", str.c_str());
	
	}
	
	printf("\nProfile:\n ");
	for (auto it=profileList.begin(); it!=profileList.end(); ++it)
	{
		printf("\nId: %s", it->second.id.c_str());
	//	printf("\nprofile: %s", it->second.profile.c_str());
		printf("\nsrc: %s", it->second.src.c_str());
		string path = it->second.src.c_str();
	 	string uri = path;
		// Makes the path absolute based in the current dir
  		if (!xpathisabs (path))
    		uri = xpathmakeabs (path);
		uri = xurifromsrc (uri, "");
  		printf("\nsrc: %s", uri.c_str());
	
		printf("\ntype: %s\n", it->second.type.c_str());


		string type = it->second.type.c_str();
		

//		int pos = type.rfind(":");
		int pos = strchr(type.c_str(),':') - type.c_str();
	
		string str = type.substr(0,pos);
		printf("\nInicio: %s\n", str.c_str());
	

	}
	printf("\nUserSettings:\n ");
	for (auto it1=userSettingsList.begin(); it1!=userSettingsList.end(); ++it1)
	{ 
		map<string, string> props = it1->second->getProperties();
		for (auto it2=props.begin(); it2!=props.end(); ++it2)
		{
			printf("\n: %s : %s\n", it2->first.c_str(),it2->second.c_str());
		}	
	}
}

//void UserContextManager::saveUsersProfile()
//{
//}



GINGA_NAMESPACE_END
