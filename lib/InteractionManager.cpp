
#include "InteractionManager.h"


InteractionManager::InteractionManager (Ginga *ginga)
{
	this->ginga = ginga;
}

InteractionManager::~InteractionManager (void)
{
}

void InteractionManager::addInteractionModule(InteractionModule *elem)
{

}
void InteractionManager::stopInteractionModule(std::string *idModulo)
{

}
void InteractionManager::startInteractionModule(std::string *idModulo)
{

}
void InteractionManager::setUserKeyList(std::string *idModulo, std::string *userKeyList)
{

}

bool InteractionManager::notifyInteraction(InteractionModule::eventTransition ev, std::string *user, std::string *key)
{

   if (ev == InteractionModule::eventTransition::onVoiceRecognition)
   {
//		 if (!(ginga->sendKey (std::string(user),std::string(key), true)))
//	 if (!(ginga->sendKey (std::string(user),std::string(key),true)))
		return false;
//     if (!(ginga->sendKey (std::string(user),std::string(key), false)))
//     if (!(ginga->sendKey (std::string(user),std::string(key),false)))
//		return false;

     return true;
   }
   else
     return false;

}
