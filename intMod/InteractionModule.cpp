//#include <string>
#include "InteractionModule.h"
using std::string;

//GINGA_NAMESPACE_BEGIN

void InteractionModule::setEvent(Event::Type valor)
{
	event = valor;
}
Event::Type InteractionModule::getEvent()
{
	return event;
}
std::string InteractionModule::getId()
{
	return id;
}

//GINGA_NAMESPACE_END
