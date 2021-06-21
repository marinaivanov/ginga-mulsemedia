
#include "Formatter.h"
#include <iostream>
#include <iomanip>
#include "InteractionManager.h"
#include "../intMod/VoiceRecognition.h"
#include "../intMod/FacialExpressionRecognition.h"
#include "../intMod/HandPoseRecognition.h"
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
	map<string,user> usrs = ((Formatter *)ginga)->getDocument()->getUsers();

	for (auto obj : *((Formatter *)ginga)->getDocument()->getObjects())
	{
		Context* _ctx;
		Composition* comp;
        comp = obj->getParent();
		if (comp != nullptr && instanceof (Context *, comp))
   			_ctx = cast (Context *, comp);
		for (auto link : *_ctx->getLinks ())
		{
			for (auto cond : link.first)
			{
				for (auto usr=usrs.begin(); usr!=usrs.end(); ++usr)
				{
					if ((usr->second.profile.compare(cond.owner.c_str())==0)||(cond.owner.compare("all")==0))
					{
						string key;
						cond.event->getParameter("key",&key);

						Key userKey;
						userKey.key = key;
						userKey.user = usr->second.id.c_str();
                	
						for (auto & c: userKey.key) c = toupper(c);
                		for (auto & c: userKey.user) c = toupper(c);
				
						((Formatter *)ginga)->getDocument()->addKeyList(cond.event->getType(),userKey);
						obj->addInteractionEvent (cond.event->getType(),key.c_str(), usr->second.id.c_str());
						Action condition;
                		condition.event = obj->getInteractionEvent (cond.event->getType(), key.c_str(), usr->second.id.c_str());
	                	g_assert_nonnull (condition.event);
                		condition.event->setParameter ("key", key.c_str());
                		condition.event->setParameter ("user", usr->second.id.c_str());
				   		condition.transition = Event::STOP;
        				condition.predicate = nullptr;
						condition.owner = usr->second.id.c_str();
						list<Action> conditions;
						list<Action> actions = link.second;
						conditions.push_back(condition);
						_ctx->addLink(conditions, actions);
					}
				}
			}
		}	
	}
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
				case Event::HANDPOSE_RECOGNITION:
				{
					if (userAuthorization("Permite habilitar sua camera?\n"))
					{
						InteractionModule * umExtModule =  new HandPoseRecognition(this);
						ExtModules.insert(std::pair<std::string,InteractionModule *>(Event::getEventTypeAsString(it->first), umExtModule));
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
{   


//printf("\nUser: %s e Key: %s", user.c_str(),key.c_str());
	switch (ev)
	{
		case Event::VOICE_RECOGNITION:
		{

//printf("\n**********Foi dito %s pelo %s ****************\n",key.c_str(), user.c_str());

			if (!(ginga->sendKey (std::string(key),std::string(user),true)))
				return false;
	        if (!(ginga->sendKey (std::string(key), std::string(user),false)))
				return false;
	        return true;
		}
		case Event::FACE_RECOGNITION:
		case Event::HANDPOSE_RECOGNITION:
		{
			printf("\nUser: %s e Key: %s", user.c_str(),key.c_str());
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
	{   json userKeyList_voice={};

		for (auto it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
		{
			switch (it1->first)
			{
                case Event::FACE_RECOGNITION:
                case Event::HANDPOSE_RECOGNITION:
				case Event::VOICE_RECOGNITION:
				{
					json keys={};
					for (auto it3=it2->second.begin(); it3!=it2->second.end(); ++it3)
					{
//						keys+=(*it3);
						keys+=(it3->key);
					}

					userKeyList_voice.push_back({{"user",it2->first}, {"key",keys}});

					break;

				}
			}
		}
		string strEvent  = Event::getEventTypeAsString(it1->first);
		setUserkeyListInteractionModule(strEvent,userKeyList_voice);
		startInteractionModule(strEvent);

	}
}

void InteractionManager::startModules()
{
	for (auto it=ExtModules.begin(); it!=ExtModules.end(); ++it){
		(it->second)->start();
	}
}


GINGA_NAMESPACE_END
