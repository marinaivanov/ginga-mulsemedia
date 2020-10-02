

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "ginga.h"
#include "Event.h"
#include <list>
#include <string>

GINGA_NAMESPACE_BEGIN
class UserManager
{
	public:
	    // Os parâmetros poderam ser as opções do documento tendo a especificações dos modulos
	    UserManager(Ginga *ginga);
	    void start();
		void setUsersProfile();
	    void setUserkeyList();
	    void startModules();
		
	private:
		Ginga *ginga;

};

GINGA_NAMESPACE_END
#endif // INTERACTION_MANAGER_H
