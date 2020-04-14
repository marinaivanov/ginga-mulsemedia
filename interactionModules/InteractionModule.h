

#ifndef INTERACTION_MODULE_H
#define INTERACTION_MODULE_H

//#include "json.h"
#include "InteractionManager.h"

class InteractionModule
{
  public:


	enum EventType
	{
		/**
		 * @brief Voice Recognition event.
		 *
		 *Stands for the recognition of a  specific key (this case word), which is
		 * directed to the object.
		 */
		VOICE_RECOGNITION = 0,
		EYE_MOTION,
	};

	virtual void startModule(InteractionModule *intMan);
	//virtual void setUserKeyList(json userKeyList);
	virtual void stopModule();
    void setEvent(EventType valor) { event = valor;}
    EventType getEvent(void) { return event;}

  private:
    InteractionModule *intManager;
    EventType event;

};

#endif // INTERACTION_MODULE_H
