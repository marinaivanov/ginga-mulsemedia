

#ifndef INTERACTION_MANAGER_H
#define INTERACTION_MANAGER_H

#include "ginga.h"
#include "Event.h"
#include <list>
#include <string>
#include "../intMod/InteractionModule.h"
#include "UserContextManager.h"

GINGA_NAMESPACE_BEGIN
class InteractionManager
{
	public:

	    // Os parâmetros poderam ser as opções do documento tendo a especificações dos modulos
	    InteractionManager(Ginga *ginga);

	    void start();
//	    bool notifyInteraction(InteractionModule::eventTransition, std::string &user, std::string &key);
	    bool notifyInteraction(Event::Type, Event::Transition, std::string &user, std::string &key);

		void createProfileLinks();
		void setUserKeyListModules();
	    void startInteractionModule(std::string mod);
	    void setUserkeyListInteractionModule(std::string mod, json _userKey);
	    void startModules();

//	    void addInteractionModule(InteractionModule *elem);
//	    void stopInteractionModule(std::string &idModulo);

		
	private:

		Ginga *ginga;
		std::map<std::string,InteractionModule *> ExtModules;
  	/// @brief User Context Manager.
  	UserContextManager *_userManager = NULL;
};

GINGA_NAMESPACE_END
#endif // INTERACTION_MANAGER_H
