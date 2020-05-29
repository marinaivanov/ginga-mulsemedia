

#ifndef INTERACTION_MANAGER_H
#define INTERACTION_MANAGER_H

#include "ginga.h"
#include "Event.h"
#include <list>
#include <string>
#include "../intMod/InteractionModule.h"

GINGA_NAMESPACE_BEGIN
class InteractionManager
{
	public:

	    // Os parâmetros poderam ser as opções do documento tendo a especificações dos modulos
	    InteractionManager(Ginga *ginga);

	    void start();
	    bool notifyInteraction(InteractionModule::eventTransition, std::string &user, std::string &key);

		void setUserKeyListModules();
	    void startInteractionModule(Event::Type mod);
	    void setUserkeyListInteractionModule(Event::Type mod, json _userKey);
	    void startModules();

//	    void addInteractionModule(InteractionModule *elem);
//	    void stopInteractionModule(std::string &idModulo);

		
	private:

		Ginga *ginga;
		std::map<Event::Type,InteractionModule *> ExtModules;

};

GINGA_NAMESPACE_END
#endif // INTERACTION_MANAGER_H
