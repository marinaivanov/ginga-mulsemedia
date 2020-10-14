
#include "Formatter.h"
#include <iostream>
#include <iomanip>
#include "UserManager.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::setw;

GINGA_NAMESPACE_BEGIN

UserManager::UserManager (Ginga *ginga)
{
	this->ginga = ginga;
}

/**
 * @brief starts all the elements to the configuration of the user.
 * @todo It will be necessary to get all the element referenced with the user to load them features.
 *       This elements to be in the document in the list.
 */
void UserManager::start()
{
	map<Event::Type,bool> interactions = (((Formatter *)ginga)->getDocument())->getInteractions();

	
}

void UserManager::loadUsersProfile()
{
}


GINGA_NAMESPACE_END
