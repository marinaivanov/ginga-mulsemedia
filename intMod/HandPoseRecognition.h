#ifndef HANDPOSERECOGNITION_H
#define HANDPOSERECOGNITION_H

#include "InteractionModule.h"
extern "C" {
    #include "../lib/mqtt/src/mqtt.h"
}
#include "../lib/nlohmann/json.hpp"
#include "../lib/InteractionManager.h"
using json = nlohmann::json;
using std::string;

class HandPoseRecognition : public InteractionModule {
    private:
        bool _run;
        json _userKeyList;

    public:
        HandPoseRecognition(InteractionManager *_intManager);
        void start();
        void setUserKeyList(json);
        void stop();
        InteractionManager *intManager;
};

#endif