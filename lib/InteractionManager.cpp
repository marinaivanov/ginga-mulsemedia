
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

void InteractionManager::notifyInteraction(InteractionModule::EventType, std::string *user, std::string *key)
{
	/*
	 bool status = ginga->sendKey (
	   string (key), g_str_equal ((const char *) type, "voice") == 0);

	  if (free_key)
	    g_free (deconst (char *, key));

	  if (!status)
	    {
	      g_assert (ginga->getState () == GINGA_STATE_STOPPED);
	      gtk_main_quit (); // all done
	    }

	  return status;
	  */
}
