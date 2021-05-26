
#include "Formatter.h"
#include "MediaSettings.h"
#include "UserContextManager.h"
#include "../loadUDMod/ParserUserDescription.h"
#include "aux-glib.h"
#include <cairo.h>
#include <gtk/gtk.h>


#include <pwd.h>


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
    map<string,profile> profileList = (((Formatter *)ginga)->getDocument())->getProfiles();
	map<string,user> userList = (((Formatter *)ginga)->getDocument())->getUsers();
	map<string,MediaSettings *> userSettingsList = (((Formatter *)ginga)->getDocument())->getuserSettings();
	map<string,map<string,string>>profileProps;
  	//int pos;      
	//Verificar se há profile. Só tem sentido carregar as informações de profiel se existe este profile vindo junto a aplicação.	
   	if (!profileList.empty())  //Tem tag userProfile portanto tem que carregar os userSetingsNode com propriedades de usuários compativeis
	{
		for (auto prof=profileList.begin(); prof!=profileList.end(); ++prof)
		{
	    	if (!prof->second.src.empty())
			{
//				printf ("\n*********** Caminho: %s \n ", prof->second.src.c_str());
//				printf ("\n*********** profielCaminho: %s \n ", prof->second.id.c_str());
				map<string,string> _properties = ParserUserDescription::parseFile (prof->second.src.c_str());
				if (!_properties.empty())
				{
					for (auto prop=_properties.begin(); prop!=_properties.end(); ++prop)
					{
//						printf ("\n**Propriedade: %s -- Valor: %s \n ", prop->first.c_str(), prop->second.c_str());
					}
				}
				profileProps.insert({prof->second.id.c_str(),_properties});
			}
		}
	}
    uid_t uid = getuid();
    char * home_dir = getpwuid( uid )->pw_dir;
	const char * path = strcat(home_dir,"/gingaFiles/");
 	DIR *dir = NULL;
    struct dirent *lsdir = NULL;
    dir = opendir(path);
	//lsdir = readdir(dir);
	list<string> arqUsrsList;
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
		if (strlen(lsdir->d_name)>3)
		{
			string pathFile = path;
			string str = lsdir->d_name;
			std::remove(str.begin(), str.end(), ' ');
            pathFile+=str;
			arqUsrsList.push_back(pathFile);
//			printf("\n Caminho : %s\n", path);
//			printf("\n Arquivo : %s\n", pathFile.c_str());
		}		
    }
	
	map<string,map<string,string>>usersProps;
  	if (!arqUsrsList.empty())  //Tem tag userProfile portanto tem que carregar os userSetingsNode com propriedades de usuários compativeis
	{
		//printf ("\n*********** USUÀRIOS:\n ");
		for (auto usr=arqUsrsList.begin(); usr!=arqUsrsList.end(); ++usr)
		{
		//		printf ("\n*********** Caminho: %s \n ", usr->c_str());
				map<string,string> _properties = ParserUserDescription::parseFile (usr->c_str());
		//		if (!_properties.empty())
		//		{
		//			for (auto prop=_properties.begin(); prop!=_properties.end(); ++prop)
		//			{
		//				printf ("\n**Propriedade: %s -- Valor: %s \n ", prop->first.c_str(), prop->second.c_str());
		//			}
		//		}
//				usersProps.insert({usr->c_str(),_properties});
				usersProps.insert({_properties["id"],_properties});		
		}
	}
/*
printf("\n****************Imprimir os propriedades de usuários que estvamo em um  arquvio xml !!!!!!!!!!!\n");
		for (auto usrProps=usersProps.begin(); usrProps!=usersProps.end(); ++usrProps)
		{
			printf("\n    Usuário: %s\n", usrProps->first.c_str());
			for (auto usrProp=usrProps->second.begin(); usrProp!=usrProps->second.end(); ++usrProp)
			{
				printf("  Propriedade: %s; valor: %s \n", usrProp->first.c_str(), usrProp->second.c_str());
			}
		}
 */
		for (auto usrProps=usersProps.begin(); usrProps!=usersProps.end(); ++usrProps)
		{
			int adicionar = 1;
			for (auto profile=profileProps.begin(); profile!=profileProps.end(); ++profile)
			{   adicionar = 1;
				for (auto propProfile=profile->second.begin(); propProfile!=profile->second.end(); ++propProfile)
				{
	//				  printf("\n!!!!!!!!Nome da propriedade no profile: %s\n", propProfile->first.c_str());
					  map<string, string>::iterator iter = usrProps->second.find(propProfile->first.c_str());
					  if (iter == usrProps->second.end())
					  {
	//				     printf("\n!!!!!!!!NÃO ACHEI A PROPRIEDADE: %s\n", propProfile->first.c_str());

						  
					 	adicionar = 0;
						break;
					  }							
					  else
					  {  

	//				  printf("\n!!!!!!!!Propriedade Profile: %s; Propriedade Usuario: %s \n", propProfile->second.c_str(),iter->second.c_str());

							if (!(iter->second.compare(propProfile->second.c_str())==0))
							{
	//							printf("\n!!!!!!!!NÃO FOI IGUAL A PROPRIEDADE: %s\n", propProfile->first.c_str());
								adicionar = 0;
								break;
							}
						}
				}
				if(adicionar)
				{
	//				printf("\nAdicionando usuário: %s", usrProps->first.c_str());
					user usr;
					usr.id = usrProps->first.c_str();
					usr.profile = profile->first.c_str();
                 	for (auto & c: usr.id) c = toupper(c);
                 	for (auto & c: usr.profile) c = toupper(c);
					(((Formatter *)ginga)->getDocument())->addUser(usr);
/*
   					map<Event::Type,list<Key>> keyLists = (((Formatter *)ginga)->getDocument())->getKeyList();
					
					for (auto lstKeys=keyLists.begin(); lstKeys!=keyLists.end(); ++lstKeys)
					{
						for (auto itKey=lstKeys->second.begin(); itKey!=lstKeys->second.end(); ++itKey)
						{
							if (itKey->user.compare(usr.profile.c_str())==0)
							{
/*
printf("\n################Achou um key!");	
printf("\n***********Key: %s!\n", itKey->key.c_str());	
printf("\n****************User: %s!\n", usr.id.c_str());						
								Key aKey;
								aKey.key = itKey->key.c_str();
								aKey.user = usr.id.c_str();
							//	(((Formatter *)ginga)->getDocument())->addKeyList(lstKeys->first,aKey);
							}
						}
					}
*/
					//verificar na _keyList do documento se há ocorrências de profiles, caso positvo adicionar o usuário com sua chaves
					//MediaSettings * usrSettings = (((Formatter *)ginga)->getDocument())->addUserSetting(usrProps->first.c_str());				
					//adicionar todas as propriedades no usuário no MediaSttings
				}
			}	

		}
}
GINGA_NAMESPACE_END
