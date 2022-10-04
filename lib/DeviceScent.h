#ifndef DEVICE_SCENT_H
#define DEVICE_SCENT_H

#include "Device.h"

#include "ginga.h"
#include "aux-ginga.h"
#include "restclient-cpp/restclient.h"
#include "nlohmann/json.hpp"
#include "restclient-cpp/connection.h"

using json = nlohmann::json;

GINGA_NAMESPACE_BEGIN

/**
 * This class implements a scent device that enables Ginga controls the Moodo device
 */ 

class DeviceScent : public Device
{
public:
    DeviceScent ();
    ~DeviceScent ();

    //Device
    void activate (string , int) ; 
    void deactivate (string ) ;
    bool connectDevice () ;
    void disconnect () ;
    string getDeviceTypeAsString () ;
    void setLocator (Locator);

    //Moodo Device
    bool login ();
    bool signup ();
    bool logout ();
    json getBoxes ();

    //Rest client
    void init();

    void setFragance();       //define the fragance to be used
    void setIntensityValue();        //define the intensity to be used
    
private:
    //Methods and attributes related to the Moodo Device
    const char* device_key; 
    const char* fan_volume;
    const char* box_status;
    string fragance;
    string intensity; 

    //Moodo
    RestClient::Connection* conn;
    json settings;
};


GINGA_NAMESPACE_END

#endif // DEVICE_SCENT_H
