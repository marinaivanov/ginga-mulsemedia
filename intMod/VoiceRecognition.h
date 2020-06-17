
#ifndef VRECOG_H
#define VRECOG_H

#include "InteractionModule.h"
extern "C" {
	#include "../lib/mqtt/src/mqtt.h"
}
#include "../lib/nlohmann/json.hpp"
#include "../lib/InteractionManager.h"
using json = nlohmann::json;
using std::string;

class VoiceRecognition  : public InteractionModule
{
   public:
	 VoiceRecognition(InteractionManager *_intManager);
	 void start();
	 void setUserKeyList(json);
	 void stop();

	 InteractionManager *intManager;
   private:
	 bool _run;
	 json _userKeyList;
};


#endif
