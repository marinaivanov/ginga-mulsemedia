
#ifndef STREAM_FILE_H
#define STREAM_FILE_H

#include "UserContextModule.h"
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;
using std::string;

class StreamFile  : public UserContextModule
{
   public:
	 StreamFile();
     void setUserPropList(json);
     json getUserPropList();
     void postUserPropList(json);
	 xmlDoc * loadUsersProfile(const string &path, string *errmsg);

   private:
	 bool _run;
	 json _userPropList;
};


#endif
