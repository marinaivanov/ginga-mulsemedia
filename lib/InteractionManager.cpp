
#include "Formatter.h"
#include <iostream>
#include <iomanip>
#include "InteractionManager.h"
#include "../intMod/VoiceRecognition.h"
#include "../intMod/GazeRecognition.h"
#include "../intMod/FacialExpressionRecognition.h"
#include "aux-glib.h"
#include <cairo.h>
#include <gtk/gtk.h>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::setw;

GINGA_NAMESPACE_BEGIN

InteractionManager::InteractionManager (Ginga *ginga)
{
	this->ginga = ginga;
}

int userAuthorization(std::string msg)
{
	GtkWidget *dialog;
    gint response;
 
    dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_YES_NO, msg.c_str());
	//gtk_message_dialog_set_markup (GTK_MESSAGE_DIALOG (dialog),markup);
    response = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    if (response == GTK_RESPONSE_YES)
 		return 1;
    else
		return 0;
}

void InteractionManager::createProfileLinks()
{
/*
	map<Event::Type,bool> interactions = (((Formatter *)ginga)->getDocument())->getInteractions();
	createProfileLinks();
	int cont = 0;
	for (auto it=interactions.begin(); it!=interactions.end(); ++it)
*/
	map<string,user> usrs = ((Formatter *)ginga)->getDocument()->getUsers();

	for (auto obj : *((Formatter *)ginga)->getDocument()->getObjects())
		for (auto evt : *obj->getEvents())
		{
		    //adiconar um evt para cada usuário por perfil
			//printf("\n **********Dono do evento : %s\n", evt->getOwner().c_str());
			for (auto usr=usrs.begin(); usr!=usrs.end(); ++usr)
			{
				string perfil;
		
				if ((usr->second.profile.compare(evt->getOwner().c_str())==0))
				{
					string Key;
					evt->getParameter("key",&Key);
				//	obj->addInteractionEvent (role->eventType,act.value, act.owner);
					obj->addInteractionEvent (evt->getType(),Key.c_str(), usr->second.id.c_str());

					printf("\n!!!!!!!!ADICIONAR EVENTO %s\n", usr->second.profile.c_str());


					printf("\n!!!!!!!!Dados  EVENTO key:  %s\n", Key.c_str());
					
				}

	    		printf("\n **********Usuario: %s profile: %s\n", usr->second.id.c_str(),usr->second.profile.c_str());
			}
		}
	//if (usrs.empty())
	 //  printf("Lista Vaia de ususarios!!!!!!!!!!!!!!");

		
		
		

}


void InteractionManager::start()
{

   _userManager = new UserContextManager(ginga); 
   _userManager->start();

   createProfileLinks();


	map<Event::Type,bool> interactions = (((Formatter *)ginga)->getDocument())->getInteractions();
	
	
	
	int cont = 0;
	for (auto it=interactions.begin(); it!=interactions.end(); ++it)
	{
		if (it->second)
		{
			switch (it->first)
			{
				case Event::VOICE_RECOGNITION:
				{	
					if (userAuthorization("Permite habilitar seu microfone?\n"))
					{
						InteractionModule * umExtModule =  new VoiceRecognition(this);
						ExtModules.insert(std::pair<std::string,InteractionModule *>(Event::getEventTypeAsString(it->first), umExtModule));
					}
 					break;
				}
				case Event::EYE_GAZE:
				{
					map<Event::Type,list<Key>> keyList = (((Formatter *)ginga)->getDocument())->getKeyList();

					list<Key>gazeList = keyList[it->first];

					for (auto it1=gazeList.begin(); it1!=gazeList.end(); it1++)
					{
						InteractionModule * umEyeGaze =  new GazeRecognition(this);
						ExtModules.insert(std::pair<std::string,InteractionModule *>(Event::getEventTypeAsString(it->first), umEyeGaze));
					}
					break;
				}
				case Event::FACE_RECOGNITION:
				{
					if (userAuthorization("Permite habilitar sua camera?\n"))
					{
						InteractionModule * umExtModule =  new FacialExpressionRecognition(this);
						ExtModules.insert(std::pair<std::string,InteractionModule *>(Event::getEventTypeAsString(it->first), umExtModule));
					}
					break;
				}


			}
		}
	}
}

bool InteractionManager::notifyInteraction(Event::Type ev, Event::Transition tran, std::string &user, std::string &key)
{   printf("\nUser: %s e Key: %s", user.c_str(),key.c_str());
	switch (ev)
	{
		case Event::VOICE_RECOGNITION:
		{
			if (!(ginga->sendKey (std::string(key),std::string(user),true)))
				return false;
	        if (!(ginga->sendKey (std::string(key), std::string(user),false)))
				return false;
	        return true;
		}
		case Event::EYE_GAZE:
		{
			if (!(((Formatter *)ginga)->sendViewed (tran, user, key)))
				return false;
	        return true;
		}
		case Event::FACE_RECOGNITION:
		{
			//printf("\nUser: %s e Key: %s", user.c_str(),key.c_str());
			if (!(ginga->sendKey (std::string(key),std::string(user),true)))
				return false;
	        if (!(ginga->sendKey (std::string(key), std::string(user),false)))
				return false;
	        return true;
		}

	}
    return false;
}

//void InteractionManager::addInteractionModule(InteractionModule *elem)
//{

//	std::list<InteractionModule * >::iterator itList;
//	itList = ExtModules.end();
//	++itList;
//	ExtModules.insert(itList,elem);

//}
//void InteractionManager::stopInteractionModule(std::string &idModulo)
//{

//}
void InteractionManager::startInteractionModule(std::string mod)
{
	  auto it = ExtModules.find (mod);
	  if (it == ExtModules.end ())
	    return;

	  it->second->start();
}
void InteractionManager::setUserkeyListInteractionModule(std::string mod, json _userKey)
{
	  auto it = ExtModules.find (mod);
	  if (it == ExtModules.end ())
	    return;
	  it->second->setUserKeyList(_userKey);
}

void InteractionManager::setUserKeyListModules()
{
	map<Event::Type,list<Key>> keyList = (((Formatter *)ginga)->getDocument())->getKeyList();


//	map<Event::Type,map<string, list<string>>> keyListUser;

	map<Event::Type,map<string, list<Key>>> keyListUser;
 
	for (auto it1=keyList.begin(); it1!=keyList.end(); ++it1)
	{
		for (auto it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
		{
//			keyListUser[it1->first][it2->user].push_back(it2->key);
			keyListUser[it1->first][it2->user].push_back(*it2);
		}
	}

	for (auto it1=keyListUser.begin(); it1!=keyListUser.end(); ++it1)
	{
		for (auto it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
		{
			switch (it1->first)
			{
                case Event::FACE_RECOGNITION:
                case Event::GESTURE_RECOGNITION:
				case Event::VOICE_RECOGNITION:
				{
					json userKeyList_voice={};

					json keys={};
					for (auto it3=it2->second.begin(); it3!=it2->second.end(); ++it3)
					{
//						keys+=(*it3);
						keys+=(it3->key);
					}

					userKeyList_voice.push_back({{"user",it2->first}, {"key",keys}});
					string strEvent  = Event::getEventTypeAsString(it1->first);
					setUserkeyListInteractionModule(strEvent,userKeyList_voice);
					startInteractionModule(strEvent);

					break;

				}
				case Event::EYE_GAZE:
				{

					json UserKeyList;

					const GingaOptions *options = ginga->getOptions();

					UserKeyList.emplace("user", it2->first);
					UserKeyList.emplace("screenWidth", options->width);
					UserKeyList.emplace("screenHeight", options->height);

					json keys={};
					Object * md;

					for (auto it3=it2->second.begin(); it3!=it2->second.end(); ++it3)
					{
//						md = (((Formatter *)ginga)->getDocument())->getObjectById(it3->c_str());
						md = (((Formatter *)ginga)->getDocument())->getObjectById(it3->component);

						string left = md->getProperty("left");
						string top = md->getProperty("top");
						string width = md->getProperty("width");
						string height = md->getProperty("height");

						json media;
						
						media.emplace("id",it3->component);
						media.emplace("left",left.c_str());
						media.emplace("top",top.c_str());
						media.emplace("width",width.c_str());
						media.emplace("height",height.c_str());
					
						keys+=(media);

					}
					//printf("\n%s:%s:%s:%s \n", left.c_str(),top.c_str(), width.c_str(),height.c_str());
					UserKeyList.push_back({"key", keys});
					string strEvent  = Event::getEventTypeAsString(it1->first);
					setUserkeyListInteractionModule(strEvent, UserKeyList);

					break;
				}
			}
		}
	}
}

void InteractionManager::startModules()
{
	for (auto it=ExtModules.begin(); it!=ExtModules.end(); ++it){
		(it->second)->start();
	}
}


GINGA_NAMESPACE_END
