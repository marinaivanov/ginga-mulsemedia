

#ifndef INTERACTION_MODULE_H
#define INTERACTION_MODULE_H

#include <string>
#include "../lib/nlohmann/json.hpp"
//#include "../lib/InteractionManager.h"


using json = nlohmann::json;
using std::string;

//class InteractionManager;

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
		onEyeGaze = 1,
	};

    virtual void start()= 0;
    virtual void setUserKeyList(json)= 0;
    virtual void stop()= 0;

    void setEvent(eventTransition valor);
    eventTransition getEvent(void);
    std::string getId();

  protected:
    std::string id;
   // InteractionManager *intManager;
    eventTransition event;

};
//GINGA_NAMESPACE_END
#endif // INTERACTION_MODULE_H
