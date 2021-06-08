#ifndef GAZERECOGNITION_H
#define GAZERECOGNITION_H

#include "../lib/nlohmann/json.hpp"
#include "../lib/InteractionManager.h"
#include "InteractionModule.h"

using json = nlohmann::json;

/**
 * @brief Eye gaze recognition module.
 */
class GazeRecognition : public InteractionModule
{
    private:
        /// @brief Control variable for the module execution current status.
        bool run;
    public:
        InteractionManager* intManager;
        GazeRecognition(InteractionManager*);
        void start();
        void setUserKeyList(json);
        void stop();
};

#endif
