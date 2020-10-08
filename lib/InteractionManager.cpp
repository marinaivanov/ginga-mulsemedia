
#include "Formatter.h"
#include <iostream>
#include <iomanip>
#include "InteractionManager.h"
#include "../intMod/VoiceRecognition.h"
#include "../intMod/GazeRecognition.h"

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

void InteractionManager::start()
{
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
					InteractionModule * umExtModule =  new VoiceRecognition(this);
					ExtModules.insert(std::pair<std::string,InteractionModule *>(Event::getEventTypeAsString(it->first), umExtModule));

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
				}

			}
		}
	}
}

bool InteractionManager::notifyInteraction(Event::Type ev, Event::Transition tran, std::string &user, std::string &key)
{
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
