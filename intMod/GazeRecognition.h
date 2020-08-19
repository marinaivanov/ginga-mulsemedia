#ifndef GAZERECOGNITION_H
#define GAZERECOGNITION_H

#include "../lib/nlohmann/json.hpp"
#include "../lib/InteractionManager.h"
#include "InteractionModule.h"

using json = nlohmann::json;

class GazeRecognition : public InteractionModule
{
    private:
        bool run;
    public:
        InteractionManager* intManager;
        GazeRecognition(InteractionManager*);
        ~GazeRecognition();
        void start();
        void setUserKeyList(json);
        void stop();
};

#endif
