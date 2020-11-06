

#ifndef INTERACTION_MODULE_H
#define INTERACTION_MODULE_H

#include <string>
#include "../lib/nlohmann/json.hpp"
#include "../lib/Event.h"

using json = nlohmann::json;
using std::string;

//class InteractionManager;

class InteractionModule
{
  public:

    virtual void start()= 0;
    virtual void setUserKeyList(json)= 0;
    virtual void stop()= 0;

    void setEvent(Event::Type valor);
    Event::Type getEvent(void);
    std::string getId();

  protected:
    std::string id;
   // InteractionManager *intManager;
   Event::Type event;

};

#endif // INTERACTION_MODULE_H
