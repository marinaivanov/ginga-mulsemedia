#ifndef FACIALEXPRECOGNITION_H
#define FACIALEXPRECOGNITION_H

#include "InteractionModule.h"
#include "../lib/nlohmann/json.hpp"
#include "../lib/InteractionManager.h"

using json = nlohmann::json;
using std::string;

class FacialExpressionRecognition : public InteractionModule {
	private:
		bool _run;
		json _userKeyList;

	public:
		FacialExpressionRecognition(InteractionManager *_intManager);
		void start();
		void setUserKeyList(json);
		void stop();
		InteractionManager *intManager;
};

#endif