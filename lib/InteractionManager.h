

#ifndef INTERACTION_MANAGER_H
#define INTERACTION_MANAGER_H

#include <map>
#include <string.h>

#include "InteractionManager.h"
#include "InteractionModule.h"

GINGA_NAMESPACE_BEGIN

class InteractionManager
{
	public:

	    void notifyInteraction(InteractionModule::EventType, user, key);
	
	    // Os parâmetros poderam ser as opções do documento tendo a especificações dos modulos
	    void InteractionManager(Ginga *ginga);
	    void ~InteractionManager();
	    void addInteractionModule(InteractionModule * elem);
	    void stopInteractionModule(sting * idModulo);
	    void startInteractionModule(sting * idModulo);
		void setUserKeyList(sting * idModulo, json userKeyList);
		
	private:
		Ginga *ginga;
		map<sting, InteractionModule *> ExtModules;
  
};

GINGA_NAMESPACE_END

#endif // INTERACTION_MANAGER_H
