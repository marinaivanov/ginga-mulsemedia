

#ifndef INTERACTION_MODULE_H
#define INTERACTION_MODULE_H

#include "json.h"

class InteractionModule
{
  public:

	enum EventType{
	 /**
	 * @brief Voice Recognition event.
	 *
	 *Stands for the recognition of a  specific key (this case word), which is
	 * directed to the object.
	 */
	 VOICE_RECOGNITION= 0,
	 EYE_MOTION,
  };

	virtual void startModule();
	virtual void setUserKeyList(json userKeyList);
	virtual void stop Module(); 
    void setEvent(EventType valor) { event = valor;}
    void EventType getEvent() { return valor;}

  private:

	EventType event;

};

#endif // INTERACTION_MODULE_H