

#ifndef INTERACTION_MANAGER_H
#define INTERACTION_MANAGER_H

#include "ginga.h"
#include "Event.h"
#include <list>
#include <string>

//#include "../interactionModules/InteractionModule.h"
#include "../interactionModules/InteractionModule.h"

//#include "../interactionModules/nlohmann/json.hpp"
//#include "nlohmann/json.hpp"
//using json = nlohmann::json;

class InteractionManager
{
	public:

	    // Os parâmetros poderam ser as opções do documento tendo a especificações dos modulos
	    InteractionManager(Ginga *ginga);

	    void start();
	    bool notifyInteraction(InteractionModule::eventTransition, std::string &user, std::string &key);

		void setUserKeyListModules();
	    void startInteractionModule(Event::Type mod);


//	    void addInteractionModule(InteractionModule *elem);
//	    void stopInteractionModule(std::string &idModulo);

		
	private:

		Ginga *ginga;
		std::map<Event::Type,InteractionModule *> ExtModules;

};


#endif // INTERACTION_MANAGER_H
