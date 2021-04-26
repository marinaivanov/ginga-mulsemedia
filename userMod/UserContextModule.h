

#ifndef USER_CONTEXT_MODULE_H
#define USER_CONTEXT_MODULE_H

#include <string>
#include "../lib/nlohmann/json.hpp"


using json = nlohmann::json;
using std::string;

class UserContextModule
{
  public:

    virtual void setUserPropList(json)= 0;
    virtual json getUserPropList()= 0;
    virtual void postUserPropList(json)= 0;
    std::string getConnection();

  protected:
      std::string connection;

};

#endif // USER_CONTEXT_MODULE_H
