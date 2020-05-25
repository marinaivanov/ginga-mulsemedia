//#include <string>
#include "InteractionModule.h"
using std::string;

//GINGA_NAMESPACE_BEGIN

InteractionModule::InteractionModule()
{
}

void InteractionModule::setEvent(InteractionModule::eventTransition valor)
{
	event = valor;
}
InteractionModule::eventTransition InteractionModule::getEvent()
{
	return event;
}
std::string InteractionModule::getId()
{
	return id;
}

//GINGA_NAMESPACE_END
