
#ifndef VRECOG_H
#define VRECOG_H

#include "InteractionModule.h"
extern "C" {
	#include "../lib/mqtt/src/mqtt.h"
}
#include "../lib/nlohmann/json.hpp"
using json = nlohmann::json;
using std::string;

//#include "../lib/mqtt/src/mqtt_pal.h"
//GINGA_NAMESPACE_BEGIN

class VoiceRecognition : public InteractionModule
{
   public:
	 void start();
	 void setUserKeyList(json);
	 void stop();

 //  private:
	//static void publish_callback(void** unused, struct mqtt_response_publish *published);
	//static void* client_refresher(void* client);
	//static void exit_VR(int status);




     //void handler();

};

//GINGA_NAMESPACE_END

#endif
