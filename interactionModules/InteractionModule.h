

#ifndef INTERACTION_MODULE_H
#define INTERACTION_MODULE_H

//#include "json.h"
//#include "InteractionManager.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

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

	explicit InteractionModule ();

	virtual void start();
	virtual void setUserKeyList(json);
	virtual void stop();


    void setEvent(eventTransition valor) { event = valor;}
    eventTransition getEvent(void) { return event;}
    std::string getId(){return id;}

  private:
    std::string id;
    InteractionModule *intManager;
    eventTransition event;

};

#endif // INTERACTION_MODULE_H
