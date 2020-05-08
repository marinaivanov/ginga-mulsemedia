#include <iostream>
#include "mqtt/async_client.h"
//#include "json.hpp"
#include "voiceRecognition.hpp"

using namespace std;
using json = nlohmann::json;

const string SERVER_ADDRESS   { "tcp://broker.mqttdashboard.com:1883" };
const string CLIENT_ID        { "listener-cpp" };
const string TOPIC            { "topic/voice_recog" };
const int QOS = 1;
mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);



explicit  voiceRecognition::voiceRecognition ()
{

}

void voiceRecognition::handler(){
   while (this->run) {
      auto msg = cli.consume_message();
      if (!msg) break;

      int pos = msg->to_string().find_first_of(':');
      std::string user = msg->to_string().substr(0, pos),
                  cmd  = msg->to_string().substr(pos+1);

      for (auto& element : this->userKeyList) {
         if (user.compare(element["user"]) == 0) {
            for (auto& command : element["key"]){
               if (cmd.compare(command) == 0) {
                  cout << "User '" << user << "' ";
                  cout << "invoked command '" << cmd << "'" << endl;
                  //notifyInteraction("voiceRecognition", user, cmd);
                  break;
               }
            }
            break;
         }
      }
   }
}

void voiceRecognition::setUserKeyList(json userKeyList){
   this->userKeyList = userKeyList;
}

void voiceRecognition::start(){

   this->run = true;

   mqtt::connect_options connOpts;
   connOpts.set_keep_alive_interval(20);
   connOpts.set_clean_session(true);

   try {
      cout << "Connected as '" << CLIENT_ID << "' to '" << SERVER_ADDRESS << "'" << endl;
      cli.connect(connOpts)->wait();
      cli.start_consuming();
      cli.subscribe(TOPIC, QOS)->wait();

      // Consume messages
      std::thread tRunning(&voiceRecognition::handler, this);
      tRunning.detach();

   }
   catch (const mqtt::exception& exc) {
      cerr << exc.what() << endl;
   }

}

void voiceRecognition::stop(){

   this->run = false;

   try {
      cout << "Shutting down and disconnecting from the MQTT server... " << flush;
      cli.unsubscribe(TOPIC)->wait();
      cli.stop_consuming();
      cli.disconnect()->wait();
      cout << "OK" << endl;
   }
   catch (const mqtt::exception& exc) {
      cerr << exc.what() << endl;
   }

}
