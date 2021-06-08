
#ifndef VRECOG_H
#define VRECOG_H

#include "InteractionModule.h"
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
