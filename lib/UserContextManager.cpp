
#include "Formatter.h"
#include "MediaSettings.h"
#include "UserContextManager.h"
//#include "UserDocument.h"
//#include "ParserUserDescription.h"

#include "aux-glib.h"
#include <cairo.h>
#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
using std::string;



GINGA_NAMESPACE_BEGIN
/// Flags to LibXML parser.
#define PARSER_LIBXML_FLAGS (XML_PARSE_NOERROR | XML_PARSE_NOWARNING)
UserContextManager::UserContextManager (Ginga *ginga)
{
	this->ginga = ginga;
}


static bool
keyboard_callback (unused (GtkWidget *widget), GdkEventConfigure *e,
                 unused (gpointer data))
{
  printf("\nClicar:\n ");

  // We must return FALSE here, otherwise the new geometry is not propagated
  // to the draw_callback().
  return false;
}

/**
 * @brief starts all the elements to the configuration of the user.
 * @todo It will be necessary to get all the element referenced with the user to load them features.
 *       This elements to be in the document in the list.
 */
void UserContextManager::start()
{
	int pos;
	map<string,profile> profileList = (((Formatter *)ginga)->getDocument())->getProfiles();
	map<string,user> userList = (((Formatter *)ginga)->getDocument())->getUsers();
	map<string,MediaSettings *> userSettingsList = (((Formatter *)ginga)->getDocument())->getuserSettings();
	list<string> arqProfList;
	list<string> arqUsrsList;
        
	DIR *dir;
    struct dirent *lsdir;
    dir = opendir("/home/fabio/AppAvaliacao/profiles");
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
		if (strlen(lsdir->d_name)>3)
		{
			std:string str = lsdir->d_name;
			std::remove(str.begin(), str.end(), ' ');
			arqProfList.push_back(str.c_str());
		}
    }
    closedir(dir);
    arqProfList.sort();
    int profSrc = 0;
	for (auto prof=profileList.begin(); prof!=profileList.end(); ++prof)
	{
	    if (prof->second.src.empty())
		      profSrc++;
	}
	if (profSrc > 0)
	{
		if (arqProfList.size() == profSrc)
		{
			for (auto prof=profileList.begin(); prof!=profileList.end(); ++prof)
			{
	    		if (prof->second.src.empty())
				{
		      		prof->second.src = arqProfList.front();
					arqProfList.pop_front();
				}
			}
		}
	}
	
  	dir = opendir("/home/fabio/AppAvaliacao/usrs");
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
		if (strlen(lsdir->d_name)>3)
		{
			string str = lsdir->d_name;
			std::remove(str.begin(), str.end(), ' ');
			arqUsrsList.push_back(str.c_str());
		}
    }
    closedir(dir);
    arqUsrsList.sort();

	if (arqUsrsList.size() == userList.size())
	{
		for (auto usr=userList.begin(); usr!=userList.end(); ++usr)
		{
	        if (usr->second.src.empty())
		      		usr->second.src = arqUsrsList.front();
					arqUsrsList.pop_front();
		}
	}

	
printf("\nProfile:\n ");
	for (auto it=profileList.begin(); it!=profileList.end(); ++it)
	{

printf("\nId: %s", it->second.id.c_str());
//printf("\nprofile: %s", it->second.profile.c_str());
printf("\nsrc: %s", it->second.src.c_str());
		string path = it->second.src.c_str();
	 	string uri = path;
		// Makes the path absolute based in the current dir
  		if (!xpathisabs (path))
    		uri = xpathmakeabs (path);
		uri = xurifromsrc (uri, "");
printf("\nCaminho: %s", uri.c_str());
  
  	Document *_docUser;
  	int w , h;
	  w = 20;
	  h = 20;
  	string *errmsg;
  //	if (_docUser == nullptr)
    	//_docUser = ParserUserDescription::parseFile (it->second.src.c_str(), w, h, errmsg);

  //if (unlikely (_docUser == nullptr))
   // return false;

	}


printf("\nUsuários:\n ");
	for (auto it=userList.begin(); it!=userList.end(); ++it)
	{
printf("\nIdList: %s", it->first.c_str());
printf("\nId: %s", it->second.id.c_str());
printf("\nprofile: %s", it->second.profile.c_str());
printf("\nArquivo: %s", it->second.src.c_str());
		string path = it->second.src.c_str();
	 	string uri = path;

		// Makes the path absolute based in the current dir
  		if (!xpathisabs ( path))
    		uri = xpathmakeabs (path);
		uri = xurifromsrc (uri, "");
printf("\nCaminho: %s", uri.c_str());
	}

/*
  g_assert_nonnull (_doc);
  _doc->setData ("formatter", (void *) this);

  Context *root = _doc->getRoot ();
  g_assert_nonnull (root);
  MediaSettings *settings = _doc->getSettings ();
  g_assert_nonnull (settings);
*/

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
