
#ifndef VRECOG_H_INCLUDED
#define VRECOG_H_INCLUDED

#include "../nlohmann/json.hpp"
#include "InteractionModule.h"
using json = nlohmann::json;

class voiceRecognition : public InteractionModule {
   private:
      bool run;
      void handler();
   public:
      explicit voiceRecognition ( );

      json userKeyList;
      void start();
      void setUserKeyList(json);
      void stop();
};

#endif
