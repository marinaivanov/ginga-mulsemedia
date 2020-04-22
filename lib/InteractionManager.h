

#ifndef INTERACTION_MANAGER_H
#define INTERACTION_MANAGER_H

#include "ginga.h"
#include <map>
#include <string>


#include "InteractionManager.h"
#include "../interactionModules/InteractionModule.h"



class InteractionManager
{
	public:

	    bool notifyInteraction(InteractionModule::eventTransition, std::string &user, std::string &key);
	
	    // Os parâmetros poderam ser as opções do documento tendo a especificações dos modulos
	    InteractionManager(Ginga *ginga);
	    ~InteractionManager(void);
	    void addInteractionModule(InteractionModule *elem);
	    void stopInteractionModule(std::string *idModulo);
	    void startInteractionModule(std::string *idModulo);
		void setUserKeyList(std::string * idModulo, std::string *userKeyList);//json userKeyList);
		
	private:
		Ginga *ginga;
		std::map<std::string, InteractionModule *> ExtModules;
  
};


#endif // INTERACTION_MANAGER_H
