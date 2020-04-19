

#ifndef INTERACTION_MODULE_H
#define INTERACTION_MODULE_H

//#include "json.h"
#include "InteractionManager.h"

class InteractionModule
{
  public:


	enum eventTransition
	{
		/**
		 * @brief Voice Recognition event.
		 *
		 *Stands for the recognition of a  specific key (this case word), which is
		 * directed to the object.
		 */
		onVoiceRecognition = 0,
		onEyeMotion,
	};

	virtual void startModule(InteractionModule *intMan);
	//virtual void setUserKeyList(json userKeyList);
	virtual void stopModule();
    void setEvent(eventTransition valor) { event = valor;}
    eventTransition getEvent(void) { return event;}

  private:
    InteractionModule *intManager;
    eventTransition event;

};

#endif // INTERACTION_MODULE_H
