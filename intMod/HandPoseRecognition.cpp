#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C" {
	#include "../lib/mqtt/include/posix_sockets.h"
}
#include "HandPoseRecognition.h"
#include "InteractionModule.h"

#include <iostream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::vector;

const char *HP_ADDRESS = {"localhost"};
//char * ADDRESS = {"broker.mqttdashboard.com"};
const char *HP_PORTA = {"1883"};
const char *HP_CLIENT = {"gesto"};
const char *HP_TOPIC = {"handpose_recog"};
const int HP_QOS = 1;

json HP_userKeyListShared;

InteractionManager *HP_intManagerShared;

void HP_exit_VR(int status, int sockfd, pthread_t *client_daemon);
void HP_publish_callback(void **unused, struct mqtt_response_publish *published);
void *HP_client_refresher(void *client);

void HandPoseRecognition::setUserKeyList(json userKeyList)
{
	_userKeyList = userKeyList;
	HP_userKeyListShared = _userKeyList;
	HP_intManagerShared = intManager;
}

HandPoseRecognition::HandPoseRecognition(InteractionManager *_intManager)
{
	_run = false;
	intManager = _intManager;
}

void HandPoseRecognition::start()
{

	if (!_run)
	{
		HP_intManagerShared = intManager;
		_run = true;

		pthread_t client_daemon;
		/* start a thread to refresh the client (handle egress and ingree client traffic) */
		if (pthread_create(&client_daemon, NULL, HP_client_refresher, NULL))
		{
			fprintf(stderr, "Failed to start client daemon.\n");
			//exit_VR(EXIT_FAILURE, sockfd,NULL);
		}
	}
}

void HandPoseRecognition::stop()
{
	_run = false;
	printf("\n%s disconnecting from %s\n", HP_ADDRESS, HP_PORTA);
	sleep(1);
	// exit_VR(EXIT_SUCCESS);
}

void HP_exit_VR(int status, int sockfd, pthread_t *client_daemon)
{
	if (sockfd != -1)
		close(sockfd);
	if (client_daemon != NULL)
		pthread_cancel(*client_daemon);
	exit(status);
}

void HP_publish_callback(void **unused, struct mqtt_response_publish *published)
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

	//std::cout << std::setw(4) << HP_userKeyListShared << "\n";

	for (auto &itUser : HP_userKeyListShared)
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
					HP_intManagerShared->notifyInteraction(Event::HANDPOSE_RECOGNITION, Event::STOP, user, key);
					break;
				}
			}
			break;
		}
	}

	//   printf("Received publish('%s'): %s\n", topic_name, (const char*) published->application_message);

	free(topic_name);
}

void *HP_client_refresher(void *d)
{
	struct mqtt_client client;
	int HP_sockfd;
	HP_sockfd = open_nb_socket(HP_ADDRESS, HP_PORTA);

	if (HP_sockfd == -1)
	{
		perror("Failed to open socket: ");
		HP_exit_VR(EXIT_FAILURE, HP_sockfd, NULL);
	}

	uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
	uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */

	mqtt_init(&client, HP_sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), HP_publish_callback);

	/* Ensure we have a clean session */
	uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
	/* Send connection request to the broker. */
	const char *client_id = "gesto";

	mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);

	/* check that we don't have any errors */
	if (client.error != MQTT_OK)
	{
		fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
		HP_exit_VR(EXIT_FAILURE, HP_sockfd, NULL);
	}
	/* subscribe */
	mqtt_subscribe(&client, HP_TOPIC, 0);
	while (1)
	{
		mqtt_sync((struct mqtt_client *)&client);
		usleep(100000U);
	}
	return NULL;
}
