#include "DeviceLight.h"
#include <sys/socket.h>

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1982 
#define BUFFER_LENGTH 1000

DeviceLight::DeviceLight(): Device ()
{
    
}

void 
DeviceLight::activate(string _color, int _intensity){
    bool activate = true;
    if (this->socket_fd != -1)
    {
        /*
            Define the light color
        */
        string method = "toggle";
        string params = "";
        
        string msgOn="{\"id\":" + _color + to_string(this->command_id) + ",\"method\":\"";
        msgOn += method + "\",\"params\":[" + params + "]}\r\n";
        this->command_id++;
        /* Sends the read message to the server through the socket */
        if (send(this->socket_fd, msgOn.c_str(), strlen(msgOn.c_str()), 0) == -1)
        {
            g_print("Fault to activate light device!\n");
            activate = false;
        }
        else
        {
            method = "set_rgb";
                
            string msgColor="{\"id\":" + _color + to_string(this->command_id) + ",\"method\":\"";
            msgColor += method + "\",\"params\":[" + _color + "]}\r\n";
        
            this->command_id++;
            /* Sends the read message to the server through the socket */
            if (send(this->socket_fd, msgColor.c_str(), strlen(msgColor.c_str()), 0) == -1)
            {
                g_print("Fault to set light color!\n");
                activate = false;
            }
        }
    }
    else
    {
        activate = false;
    }
    _active = activate;
} 

void 
DeviceLight::deactivate(string id)
{

    if (this->socket_fd != -1)
    {
        string method = "toggle";
        string params = "";
        
        string msgOn="{\"id\":" + to_string(this->command_id) + ",\"method\":\"";
        msgOn += method + "\",\"params\":[" + params + "]}\r\n";
        this->command_id++;
        /* Sends the read message to the server through the socket */
        if (send(this->socket_fd, msgOn.c_str(), strlen(msgOn.c_str()), 0) != -1)
        {
            _active = false;
        }
    }
   
}

bool 
DeviceLight::connectDevice()
{
    /* Server socket */
    struct sockaddr_in server;
    
    this->port = 55443;
    this->ip_address = this->getAddress();//"192.168.1.7";
    this->command_id = 0;
    this->socket_fd = -1;

    int len = sizeof(server);
    int slen;

    //g_print ("Connecting with light device...\n");

    /*
     * Cria um socket para o cliente que ira conectar com a lampada
     */
    if ((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        g_print("Error on connect with light device");
        return false;
    }

    /* Defines the connection properties */
    server.sin_family = AF_INET;
    server.sin_port = htons(this->port);
    server.sin_addr.s_addr = inet_addr(this->ip_address.c_str());
    memset(server.sin_zero, 0x0, 8);

    // Tries to connect to the server 
    if (connect(this->socket_fd, (struct sockaddr*) &server, len) == -1) {
        g_print("Can't connect to server\n");
        return false;
    }

    return true;
}

void
DeviceLight::setColor(string _color)
{
    if (this->socket_fd != -1)
    {
        /*
            Define the light color
        */
        string method = "set_rgb";
                
        string msgColor="{\"id\":" + _color + to_string(this->command_id) + ",\"method\":\"";
        msgColor += method + "\",\"params\":[" + _color + "]}\r\n";
       
        this->command_id++;
        /* Sends the read message to the server through the socket */
        send(this->socket_fd, msgColor.c_str(), strlen(msgColor.c_str()), 0);
        /*if (send(this->socket_fd, msgColor.c_str(), strlen(msgColor.c_str()), 0) == -1)
        {
            printf("Error to set light color\n");
        }*/

    }
}

void
DeviceLight::setLightIntensity(string _intensity)
{
    if (this->socket_fd != -1)
    {
        /*
            Define the light intensity
        */
        string method = "set_bright";
                
        string msg="{\"id\":" + _intensity + to_string(this->command_id) + ",\"method\":\"";
        msg += method + "\",\"params\":[" + _intensity + "]}\r\n";
       
        this->command_id++;
        /* Sends the read message to the server through the socket */
        send(this->socket_fd, msg.c_str(), strlen(msg.c_str()), 0);
        /*if (send(this->socket_fd, msg.c_str(), strlen(msg.c_str()), 0) == -1)
        {
            printf("Error to set intensity value!\n");
        }*/

    }
}

void 
DeviceLight::disconnect()
{
    close(this->socket_fd);
} 

string
DeviceLight::getDeviceTypeAsString()
{
    return "light";
}

void
DeviceLight::setLocator(Locator l)
{
    this->_locator = l;
}
