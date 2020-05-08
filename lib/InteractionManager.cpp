
#include "InteractionManager.h"
//#include "../interactionModules/voiceRecognition.hpp"
#include "Formatter.h"
#include <iostream>
#include <iomanip>
using json = nlohmann::json;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::setw;


InteractionManager::InteractionManager (Ginga *ginga)
{
	this->ginga = ginga;
}

void InteractionManager::start()
{
	map<Event::Type,bool> interactions = (((Formatter *)ginga)->getDocument())->getInteractions();

	//std::list<InteractionModule * >::iterator itList;
	//itList = ExtModules.begin();

	int cont = 0;
	for (auto it=interactions.begin(); it!=interactions.end(); ++it)
	{
		if (it->second)
		{
			switch (it->first)
			{
				case Event::VOICE_RECOGNITION:
				{

				//	InteractionModule * umExtModule = new voiceRecognition();
				//	umExtModule->start();
				//	ExtModules.insert(std::pair<>it->first, umExtModule);

					break;
				}

			}
		}
	}
}


bool InteractionManager::notifyInteraction(InteractionModule::eventTransition ev, std::string &user, std::string &key)
{
	switch (ev)
	{
		case InteractionModule::eventTransition::onVoiceRecognition:
		{
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
void InteractionManager::startInteractionModule(Event::Type mod)
{
	  auto it = ExtModules.find (mod);
	  if (it == ExtModules.end ())
	    return;

	  it->second->start();

}


void InteractionManager::setUserKeyListModules()
{

	map<Event::Type,Key > keyList = (((Formatter *)ginga)->getDocument())->getKeyList();
//userKeyList padrao
	map<Event::Type,map<string, list<string>>> keyListUser;

	for (auto it=keyList.begin(); it!=keyList.end(); ++it)
	{
		//TRACE("-------------SetUserKeyListModules--------------------");
		//TRACE("Evento: %d; user: %s; key: %s", it->first, it->second.user.c_str(), it->second.key.c_str());

		keyListUser[it->first][it->second.user].push_back(it->second.key);
	}

	for (auto it1=keyListUser.begin(); it1!=keyListUser.end(); ++it1)
		for (auto it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
			for (auto it3=it2->second.begin(); it3!=it2->second.end(); ++it3)
			{
				TRACE("-------------SetUserKeyListModules--------------------");
				TRACE("Evento: %d; user: %s; key: %s", it1->first, it2->first.c_str(), (*it3).c_str());
			}

	for (auto it1=keyListUser.begin(); it1!=keyListUser.end(); ++it1)
	{
		switch (it1->first)
		{
			case Event::VOICE_RECOGNITION:
			{
				json userKeyList_voice={};

				for (auto it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
				{
					json user={};

					user={"user",it2->first};

					json keys={};

					for (auto it3=it2->second.begin(); it3!=it2->second.end(); ++it3)
					{
						keys+=(*it3);
					}

					json atrs;

					atrs = {"key", keys};

					json oneUserKeyList= {};

					oneUserKeyList.push_back({user,atrs});

					userKeyList_voice.push_back(oneUserKeyList);

				    std::cout << std::setw(4) << userKeyList_voice << "\n";
				}
				startInteractionModule(it1->first);
				break;
			}

		}
	}

}

