

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "ginga.h"
#include "Event.h"
#include <list>
#include <string>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/uri.h>

GINGA_NAMESPACE_BEGIN
class UserManager
{
	public:
	    // Os parâmetros poderam ser as opções do documento tendo a especificações dos modulos
	    UserManager(Ginga *ginga);
	    void start();
		void setUsersProfile();	
	private:
		Ginga *ginga;

};

GINGA_NAMESPACE_END
#endif // INTERACTION_MANAGER_H
