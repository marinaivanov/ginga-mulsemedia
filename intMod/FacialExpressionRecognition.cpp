

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


extern "C" {
  #include "../lib/mqtt/include/posix_sockets.h"
  #include "../lib/mqtt/src/mqtt.h"
}
//#include "InteractionModule.h"
#include "FacialExpressionRecognition.h"

#include <iostream>
#include <iomanip>

using json = nlohmann::json;

using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::vector;

char *FR_ADDRESS = {"localhost"};
//char * ADDRESS = {"broker.mqttdashboard.com"};
char *FR_PORTA = {"1883"};
char *FR_CLIENT = {"expressao_facial"};
char *FR_TOPIC = {"fexp_recog"};
int FR_QOS = 1;


json FR_userKeyListShared;

InteractionManager *FR_intManagerShared;
//extern int open_nb_socket(const char* addr, const char* port);
void FR_exit_VR(int status, int sockfd, pthread_t *client_daemon);
void FR_publish_callback(void **unused, struct mqtt_response_publish *published);
void *FR_client_refresher(void *client);

void FacialExpressionRecognition::setUserKeyList(json userKeyList)
{
	_userKeyList = userKeyList;
	FR_userKeyListShared = _userKeyList;
	FR_intManagerShared = intManager;
}

FacialExpressionRecognition::FacialExpressionRecognition(InteractionManager *_intManager)
{
	_run = false;
	intManager = _intManager;
}

void FacialExpressionRecognition::start()
{

	if (!_run)
	{
		FR_intManagerShared = intManager;
		_run = true;
		//printf("\n------------->   Comecouo Face Recognition");
		pthread_t client_daemon;
		/* start a thread to refresh the client (handle egress and ingree client traffic) */
		if (pthread_create(&client_daemon, NULL, FR_client_refresher, NULL))
		{

			fprintf(stderr, "Failed to start client daemon.\n");
			//exit_VR(EXIT_FAILURE, sockfd,NULL);
		}
	}
}

void FacialExpressionRecognition::stop()
{
	_run = false;
	printf("\n%s disconnecting from %s\n", FR_ADDRESS, FR_PORTA);
	sleep(1);
	// exit_VR(EXIT_SUCCESS);
}

void FR_exit_VR(int status, int sockfd, pthread_t *client_daemon)
{
	if (sockfd != -1)
		close(sockfd);
	if (client_daemon != NULL)
		pthread_cancel(*client_daemon);
	exit(status);
}

void FR_publish_callback(void **unused, struct mqtt_response_publish *published)
{
	/* note that published->topic_name is NOT null-terminated (here we'll change it to a c-string) */
	char *topic_name = (char *)malloc(published->topic_name_size + 1);
	memcpy(topic_name, published->topic_name, published->topic_name_size);
	topic_name[published->topic_name_size] = '\0';

	char *message = (char *)malloc(published->application_message_size + 1);
	memcpy(message, published->application_message, published->application_message_size);
	message[published->application_message_size] = '\0';

	std::string msg = (const char *)message;
	//  printf("Mansagem no topico: %s\n", msg.c_str());

	TRACE("---> Mesagem capturada! do MQTT");

//    printf("\nMensagem: %s ", msg.c_str());
	
	int pos = msg.find_first_of(':');
	std::string user = msg.substr(0, pos),
				key = msg.substr(pos + 1);

	for (auto &c : user)
		c = toupper(c);
	for (auto &c : key)
		c = toupper(c);
	/*
    printf("Received publish('%s'): %s\n", topic_name, (const char*) published->application_message);
    printf("Received publish('%s'): %s\n", topic_name, message);
    printf("User: %s Key: %s\n", user.c_str(),key.c_str());
	*/

	//std::cout << std::setw(4) << FR_userKeyListShared << "\n";

	for (auto &itUser : FR_userKeyListShared)
	{
		//	printf("\nUser:\n");
		//  std::cout << std::setw(4) << itUser["user"] << "\n";
		std::string userDoc = (string)itUser["user"];
		for (auto &c : userDoc)
			c = toupper(c);
		if (user.compare(userDoc) == 0)
		{

			for (auto &itKey : itUser["key"])
			{
				std::string keyDoc = (string)itKey;
				for (auto &c : keyDoc)
					c = toupper(c);
				if (key.compare(keyDoc) == 0)
				{
//printf("\nUser: %s e Key: %s", user.c_str(),key.c_str());
					FR_intManagerShared->notifyInteraction(Event::FACE_RECOGNITION, Event::STOP, user, key);
//printf("\n VOltei do notify");
					break;
				}
			}
			break;
		}
	}

	//   printf("Received publish('%s'): %s\n", topic_name, (const char*) published->application_message);

	free(topic_name);
}

void *FR_client_refresher(void *d)
{
	struct mqtt_client client;
	int FR_sockfd;
	FR_sockfd = open_nb_socket(FR_ADDRESS, FR_PORTA);

	if (FR_sockfd == -1)
	{
		perror("Failed to open socket: ");
		FR_exit_VR(EXIT_FAILURE, FR_sockfd, NULL);
	}

	uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
	uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */

	mqtt_init(&client, FR_sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), FR_publish_callback);

	/* Ensure we have a clean session */
	uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
	/* Send connection request to the broker. */
	const char *client_id = "expressao_facial";

	mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);

	/* check that we don't have any errors */
	if (client.error != MQTT_OK)
	{
		fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
		FR_exit_VR(EXIT_FAILURE, FR_sockfd, NULL);
	}
	/* subscribe */
	mqtt_subscribe(&client, FR_TOPIC, 0);
	while (1)
	{
		mqtt_sync((struct mqtt_client *)&client);
		usleep(10000U);
	}
	return NULL;
}
