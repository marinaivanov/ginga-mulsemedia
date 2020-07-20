#ifndef EYEGAZEMODULE_H
#define EYEGAZEMODULE_H

#include "../lib/nlohmann/json.hpp"
#include "../lib/InteractionManager.h"
#include "InteractionModule.h"

using json = nlohmann::json;

class EyeGazeModule : public InteractionModule
{
    private:
        bool run;
    public:
        InteractionManager* intManager;
        EyeGazeModule(InteractionManager*);
        ~EyeGazeModule();
        void start();
        void setUserKeyList(json);
        void stop();
};

#endif
