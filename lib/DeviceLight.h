#ifndef DEVICE_LIGHT_H
#define DEVICE_LIGHT_H

#include "Device.h"

#include "ginga.h"
#include "aux-ginga.h"

GINGA_NAMESPACE_BEGIN

class DeviceLight : public Device
{
public:
    DeviceLight ();
    ~DeviceLight ();

    //Device
    void activate (string , int) ; 
    void deactivate (string ) ;
    bool connectDevice () ;
    void disconnect () ;
    string getDeviceTypeAsString () ;

    //Yeelight Device    
    void setColor (string );           //define the color to be used
    void setLightIntensity (string );        //define the intensity to be used
    void setLocator(Locator );
    
private:
    //Methods and attributes related to the Yeelight Device
    string color;
    string intensity; 

    //Yeelight device
    string ip_address;          // IP address of bulb
    int port;        //port to connect with the bulb 
    int socket_fd;      //connection file descriptor
    int command_id;

};

GINGA_NAMESPACE_END

#endif // DEVICE_LIGHT_H
