#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C" {
  #include "../lib/mqtt/include/posix_sockets.h"
}

#include "VoiceRecognition.h"

using namespace std;
using json = nlohmann::json;

//GINGA_NAMESPACE_BEGIN

char * ADDRESS = {"localhost"};
char * PORTA = {"1883"};
char * CLIENT = {"voz"};
char * TOPIC = {"voiceReg"};
int QOS = 1;

static bool _run;
static int _sockfd;
static struct mqtt_client _client;
static pthread_t _client_daemon;
static json userKeyList;

void exit_VR(int status)
{
   if (_sockfd != -1) close(_sockfd);
   if (_client_daemon != NULL) pthread_cancel(_client_daemon);
   exit(status);
}

void publish_callback(void** unused, struct mqtt_response_publish *published)
{
   /* note that published->topic_name is NOT null-terminated (here we'll change it to a c-string) */
   char* topic_name = (char*) malloc(published->topic_name_size + 1);

   memcpy(topic_name, published->topic_name, published->topic_name_size);
   topic_name[published->topic_name_size] = '\0';

   std::string msg = topic_name;
   int pos = msg.find_first_of(':');
   std::string user = msg.substr(0, pos),
               cmd  = msg.substr(pos+1);

   for (auto& element : userKeyList)
   {
      if (user.compare(element["user"]) == 0)
      {
         for (auto& command : element["key"])
         {
            if (cmd.compare(command) == 0)
            {
               cout << "User '" << user << "' ";
               cout << "invoked command '" << cmd << "'" << endl;
               //notifyInteraction("voiceRecognition", user, cmd);
               break;
            }
         }
         break;
      }
   }

   printf("Received publish('%s'): %s\n", topic_name, (const char*) published->application_message);

   free(topic_name);
}

void* client_refresher(void* client)
{
   while(1)
   {
       mqtt_sync((struct mqtt_client*) client);
       sleep(1);
   }
   return NULL;
}

void VoiceRecognition::setUserKeyList(json _userKeyList){
   userKeyList = _userKeyList;
}

 void VoiceRecognition::start()
{
	if (!(_run))
	{
		_client_daemon = NULL;

		_run = true;

		_sockfd = open_nb_socket(ADDRESS, PORTA);

		if (_sockfd == -1)
		{
			perror("Failed to open socket: ");
			exit_VR(EXIT_FAILURE);
		}

		uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
		uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */

		mqtt_init(&_client, _sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);

		/* Ensure we have a clean session */
		uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
		/* Send connection request to the broker. */

		mqtt_connect(&_client, CLIENT, NULL, NULL, 0, NULL, NULL, connect_flags, 400);

		/* check that we don't have any errors */
		if (_client.error != MQTT_OK) {
			fprintf(stderr, "error: %s\n", mqtt_error_str(_client.error));
			exit_VR(EXIT_FAILURE);
		}

		/* start a thread to refresh the client (handle egress and ingree client traffic) */
		if(pthread_create(&_client_daemon, NULL, client_refresher, &_client))
		{
			fprintf(stderr, "Failed to start client daemon.\n");
			exit_VR(EXIT_FAILURE);
		}

		/* subscribe */
		mqtt_subscribe(&_client, TOPIC, 0);
	}
}

 void VoiceRecognition::stop()
{
   _run = false;
   printf("\n%s disconnecting from %s\n", ADDRESS, PORTA);
   sleep(1);
  // exit_VR(EXIT_SUCCESS);
}


//GINGA_NAMESPACE_END
