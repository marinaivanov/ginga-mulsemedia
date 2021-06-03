#include "DeviceScent.h"

#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"

#include <iostream>
#include <curl/curl.h>

map<string,string> scent_table = 
{
    {"urn:mpeg:mpeg-v:01-SI-ScentCS-NS:out_at_sea","Sea Breeze"},
    {"urn:mpeg:mpeg-v:01-SI-ScentCS-NS:coconut", "Monoi de Tahiti"},
    {"urn:mpeg:mpeg-v:01-SI-ScentCS-NS:aquamarine", "Amber Marine" },
    {"urn:mpeg:mpeg-v:01-SI-ScentCS-NS:sweet_sherry", "Sweet Sand"}    
}; 

DeviceScent::DeviceScent(): Device ()
{
    this->device_key = ""; 
    this->fan_volume = "";
    this->box_status = ""; 
    conn = NULL;
    settings = json::array();
}

void
DeviceScent::init(){
    

}

void
DeviceScent::setLocator(Locator l)
{
    this->_locator = l;
}

void 
DeviceScent::activate(string _scent, int _intensity){
    
    bool _scentFound = true;
    
    json boxes = this->getBoxes();
    json slot0, slot1, slot2, slot3;
       
    if (boxes!=NULL)
    {
        if (_scent == "Sea")
        {           
            slot0.emplace("fan_speed",_intensity);
            slot0.emplace("fan_active",true);
            slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
            /*slot1.emplace("fan_speed",this->settings[1]["fan_speed"]);
            slot1.emplace("fan_active",this->settings[1]["fan_active"]);
            slot2.emplace("fan_speed",this->settings[2]["fan_speed"]);
            slot2.emplace("fan_active",this->settings[2]["fan_active"]);
            slot3.emplace("fan_speed",this->settings[3]["fan_speed"]);
            slot3.emplace("fan_active",this->settings[3]["fan_active"]);*/
            
        }
        else if (_scent == "Coconuts")
        {
            slot1.emplace("fan_speed",_intensity);
            slot1.emplace("fan_active",true);
            slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
            slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
            /*slot0.emplace("fan_speed",this->settings[0]["fan_speed"]);
            slot0.emplace("fan_active",this->settings[0]["fan_active"]);
            slot2.emplace("fan_speed",this->settings[2]["fan_speed"]);
            slot2.emplace("fan_active",this->settings[2]["fan_active"]);
            slot3.emplace("fan_speed",this->settings[3]["fan_speed"]);
            slot3.emplace("fan_active",this->settings[3]["fan_active"]);*/
        }
        else if (_scent == "Amber Marine")
        {
            slot2.emplace("fan_speed",_intensity);
            slot2.emplace("fan_active",true);
            slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
            slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
            /*slot1.emplace("fan_speed",this->settings[1]["fan_speed"]);
            slot1.emplace("fan_active",this->settings[1]["fan_active"]);
            slot0.emplace("fan_speed",this->settings[0]["fan_speed"]);
            slot0.emplace("fan_active",this->settings[0]["fan_active"]);
            slot3.emplace("fan_speed",this->settings[3]["fan_speed"]);
            slot3.emplace("fan_active",this->settings[3]["fan_active"]);*/
        }
        else if (_scent == "Sweet")
        {
            slot3.emplace("fan_speed",_intensity);
            slot3.emplace("fan_active",true);
            slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
            /*slot1.emplace("fan_speed",this->settings[1]["fan_speed"]);
            slot1.emplace("fan_active",this->settings[1]["fan_active"]);
            slot2.emplace("fan_speed",this->settings[2]["fan_speed"]);
            slot2.emplace("fan_active",this->settings[2]["fan_active"]);
            slot0.emplace("fan_speed",this->settings[0]["fan_speed"]);
            slot0.emplace("fan_active",this->settings[0]["fan_active"]);*/
        }
        else
        {
            g_print("Scent %s not found\n", _scent.c_str());
            _scentFound = false;
        }
    }
   
    if (_scentFound)
    {
        boxes.emplace("settings_slot0",slot0);
        boxes.emplace("settings_slot1",slot1);
        boxes.emplace("settings_slot2",slot2);
        boxes.emplace("settings_slot3",slot3);
        conn->AppendHeader("Content-Type", "application/json");
        RestClient::Response r  = conn->post("/boxes/",boxes.dump().c_str()); 
        if (r.code == 200){
            json body_response;
            body_response = json::parse(r.body.c_str());
            //std::cout << body_response.dump(4) << std::endl;
            _active = true;
        }   
    }
} 

void 
DeviceScent::deactivate(string _scent)
{
   bool _scentFound = true;

    json boxes = this->getBoxes();
    json slot0, slot1, slot2, slot3;
       
    if (boxes!=NULL)
    {
        if (_scent == "Sea")
        {            
            /*slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
            slot1.emplace("fan_speed",this->settings[1]["fan_speed"]);
            slot1.emplace("fan_active",this->settings[1]["fan_active"]);
            slot2.emplace("fan_speed",this->settings[2]["fan_speed"]);
            slot2.emplace("fan_active",this->settings[2]["fan_active"]);
            slot3.emplace("fan_speed",this->settings[3]["fan_speed"]);
            slot3.emplace("fan_active",this->settings[3]["fan_active"]);*/
            slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
            slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
            
        }
        else if (_scent == "Coconuts")
        {
            /*slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot0.emplace("fan_speed",this->settings[0]["fan_speed"]);
            slot0.emplace("fan_active",this->settings[0]["fan_active"]);
            slot2.emplace("fan_speed",this->settings[2]["fan_speed"]);
            slot2.emplace("fan_active",this->settings[2]["fan_active"]);
            slot3.emplace("fan_speed",this->settings[3]["fan_speed"]);
            slot3.emplace("fan_active",this->settings[3]["fan_active"]);*/
            slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
            slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
        }
        else if (_scent == "Amber Marine")
        {
            /*slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot1.emplace("fan_speed",this->settings[1]["fan_speed"]);
            slot1.emplace("fan_active",this->settings[1]["fan_active"]);
            slot0.emplace("fan_speed",this->settings[0]["fan_speed"]);
            slot0.emplace("fan_active",this->settings[0]["fan_active"]);
            slot3.emplace("fan_speed",this->settings[3]["fan_speed"]);
            slot3.emplace("fan_active",this->settings[3]["fan_active"]);*/
            slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
            slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
        }
        else if (_scent == "Sweet")
        {
            /*slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
            slot1.emplace("fan_speed",this->settings[1]["fan_speed"]);
            slot1.emplace("fan_active",this->settings[1]["fan_active"]);
            slot2.emplace("fan_speed",this->settings[2]["fan_speed"]);
            slot2.emplace("fan_active",this->settings[2]["fan_active"]);
            slot0.emplace("fan_speed",this->settings[0]["fan_speed"]);
            slot0.emplace("fan_active",this->settings[0]["fan_active"]);*/
            slot3.emplace("fan_speed",0);
            slot3.emplace("fan_active",false);
            slot1.emplace("fan_speed",0);
            slot1.emplace("fan_active",false);
            slot2.emplace("fan_speed",0);
            slot2.emplace("fan_active",false);
            slot0.emplace("fan_speed",0);
            slot0.emplace("fan_active",false);
        }
        else
        {
            g_print("Scent %s not found\n", _scent.c_str());
            _scentFound = false;
        }
    }
   
    if (_scentFound)
    {
        boxes.emplace("settings_slot0",slot0);
        boxes.emplace("settings_slot1",slot1);
        boxes.emplace("settings_slot2",slot2);
        boxes.emplace("settings_slot3",slot3);
        conn->AppendHeader("Content-Type", "application/json");
        RestClient::Response r  = conn->post("/boxes/",boxes.dump().c_str()); 
        if (r.code == 200){
            _active = false;
        }   
    }
}

bool 
DeviceScent::connectDevice()
{
    return login();
}

void 
DeviceScent::disconnect()
{
    RestClient::disable();
} 

//Moodo Device
bool 
DeviceScent::login()
{
    RestClient::init();
    
    conn = new RestClient::Connection("https://rest.moodo.co/api");
    if (conn != NULL)
    {
        RestClient::HeaderFields headers;
        headers["accept"] = "application/json";
        headers["Content-Type"] = "application/json";
        conn->SetHeaders(headers); 

        string _body = "{  \"email\": \"marii.ivanov@gmail.com\",  \"password\": \"m1d14c0m\"}";
        RestClient::Response r  = conn->post("/login",_body);
        // HTTPS response code
        if (r.code == 200 && !r.body.empty())
        {
            //Successful Login
            json body_response;
            string response = r.body;
            body_response = json::parse(response.c_str());
            // Set token 
            conn->AppendHeader("token",body_response["token"]);
        }
        else
        {
            //Error response
            g_print("Unable to obtain access token\n");
            return false;
        }
    }
    else
    {
        g_print("Unable to connect with Moodo Device\n");
        return false;
    }
    return true;
}

bool 
DeviceScent::signup()
{

}

bool 
DeviceScent::logout()
{

}

string
DeviceScent::getDeviceTypeAsString(){
    return "scent";
}

json
DeviceScent::getBoxes()
{
    RestClient::Response r  = conn->get("/boxes/");
    
    if (r.code == 200 && !r.body.empty())
    {    
        json response = json::parse(r.body.c_str());
        json box = response["boxes"];
        //TRACE ("Return: %s\n", box[0].dump(4).c_str());
        this->device_key = box[0]["device_key"].dump().c_str();
        this->fan_volume = box[0]["fan_volume"].dump().c_str();
        this->box_status = box[0]["box_status"].dump().c_str();
        this->settings = box[0]["settings"];
        json boxes_info;
        boxes_info.emplace("device_key",box[0]["device_key"]);
        boxes_info.emplace("fan_volume",box[0]["fan_volume"]);
        boxes_info.emplace("box_status",box[0]["box_status"]);
        return boxes_info;
    }
    else
    {
        json error = json::parse(r.body.c_str());
        g_print ("Error: %s\n", error["error"].dump().c_str());
        return NULL;
    }
}