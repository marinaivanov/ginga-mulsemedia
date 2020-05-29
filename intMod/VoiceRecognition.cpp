#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C" {
  #include "../lib/mqtt/include/posix_sockets.h"
}
#include "VoiceRecognition.h"
#include "InteractionModule.h"

#include <iostream>
#include <iomanip>


using namespace std;
using json = nlohmann::json;

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::setw;



char * ADDRESS = {"localhost"};
char * PORTA = {"1883"};
char * CLIENT = {"voz"};
char * TOPIC = {"voiceReg"};
int QOS = 1;


json userKeyListShared;

InteractionManager *intManagerShared;

void exit_VR(int status, int sockfd, pthread_t *client_daemon);
void publish_callback(void** unused, struct mqtt_response_publish *published);
void* client_refresher(void* client);

void VoiceRecognition::setUserKeyList(json userKeyList){
    _userKeyList = userKeyList;
    userKeyListShared  = _userKeyList;
    intManagerShared = intManager;
}

VoiceRecognition::VoiceRecognition(InteractionManager *_intManager)
{
	_run = false;
	intManager = _intManager;
}

void VoiceRecognition::start()
{

	if (!_run)
	{
	    intManagerShared = intManager;

		printf("Start voiceReg\n ");
		_run = true;

		pthread_t client_daemon;
		/* start a thread to refresh the client (handle egress and ingree client traffic) */
		if(pthread_create(&client_daemon, NULL, client_refresher, NULL))
		{
			fprintf(stderr, "Failed to start client daemon.\n");
			//exit_VR(EXIT_FAILURE, sockfd,NULL);
		}
	}
}

 void VoiceRecognition::stop()
{
   _run = false;
   printf("\n%s disconnecting from %s\n", ADDRESS, PORTA);
   sleep(1);
  // exit_VR(EXIT_SUCCESS);
}

 void exit_VR(int status, int sockfd, pthread_t *client_daemon)
 {
    if (sockfd != -1) close(sockfd);
    if (client_daemon != NULL) pthread_cancel(*client_daemon);
    exit(status);
 }

 void publish_callback(void** unused, struct mqtt_response_publish *published)
 {
	//printf("************************************** Callback: ");

	/* note that published->topic_name is NOT null-terminated (here we'll change it to a c-string) */
    char* topic_name = (char*) malloc(published->topic_name_size + 1);
 //	printf("************************************** Callback: ");
    memcpy(topic_name, published->topic_name, published->topic_name_size);
    topic_name[published->topic_name_size] = '\0';

    std::string msg = (const char*) published->application_message;
    int pos = msg.find_first_of(':');
    std::string user = msg.substr(0, pos),
                key  = msg.substr(pos+1);

    printf("Received publish('%s'): %s\n", topic_name, (const char*) published->application_message);
    printf("User: %s Key: %s\n", user.c_str(),key.c_str());


    std::cout << std::setw(4) << userKeyListShared << "\n";


    for (auto& itUser : userKeyListShared)
    {
    	printf("\nUser:\n");
    	std::cout << std::setw(4) << itUser["user"] << "\n";

       if (user.compare((string)itUser["user"]) == 0)
       {

          for (auto& itKey : itUser["key"])
          {

             if (key.compare(itKey) == 0)
             {
            	 printf("**************Notify**********");
            	intManagerShared->notifyInteraction(InteractionModule::eventTransition::onVoiceRecognition, user, key);
                break;
             }
          }
          break;
       }
    }

    printf("Received publish('%s'): %s\n", topic_name, (const char*) published->application_message);

    free(topic_name);
 }

 void* client_refresher(void *d)
 {
	struct mqtt_client client;
	int sockfd;
	sockfd = open_nb_socket(ADDRESS, PORTA);

	if (sockfd == -1)
	{
		perror("Failed to open socket: ");
		exit_VR(EXIT_FAILURE,sockfd, NULL);
	}

	uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
	uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */

	mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);

	/* Ensure we have a clean session */
	uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
	/* Send connection request to the broker. */
	const char* client_id = "voz";

	mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);

	/* check that we don't have any errors */
	if (client.error != MQTT_OK)
	{
		fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
		exit_VR(EXIT_FAILURE,sockfd, NULL);
	}
	/* subscribe */
	mqtt_subscribe(&client, TOPIC, 0);
    while(1)
    {
    	mqtt_sync((struct mqtt_client*) &client);
        usleep(100000U);
    }
    return NULL;
 }
