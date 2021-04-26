//#include <string>
#include "UserContextModule.h"
using std::string;

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

