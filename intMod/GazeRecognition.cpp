#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <string>
#include <assert.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include "GazeRecognition.h"
#include "aux-ginga.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::time_t;
using std::ctime;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::ifstream;

//Media Region's possible states.
typedef enum state{ 
    sleeping = 0,
    initiated, 
    finished, 
} State;

typedef struct point {
    double x, y;
} Point;

typedef struct region {
    string id;
    Point topLeft;
    Point bottomRight;
    time_point<system_clock> startTime; // Time instant where the user started to look at the region
    bool gazed = false;
    State regState = sleeping;
} Region;

string user = "";
string startRegions = "";
string stopRegions = "";
string abortRegions = "";
vector<Region> regionList;
InteractionManager* sharedIntManager;

/**
 * @brief Amount of time (in seconds) that the user must look at a region to COMPLETE the eyeGaze event.
 */
double durationGaze;

/**
 * @brief Constant between 0 and 1. It represents a time slice of the fixation duration (variable 
 *        "durationGaze"). It is used to define the amount of time that the user must look at a 
 *        region to START the eyeGaze event.
*/
double startConstant;



/*** Auxiliary functions *****************************************/

/**
 * @brief Verifies if a Point P is inside a Region R.
 * @param R Is one of the media's region.
 * @param P Is the point read by the eye-tracker (the gaze point).
 * @return Return TRUE if the point is inside the region and FALSE if it is not.
 */
bool pointIsInsideRegion(Region R, Point P)
{
    return  (P.x >= R.topLeft.x) && (P.x <= R.bottomRight.x) && 
		    (P.y >= R.topLeft.y) && (P.y <= R.bottomRight.y);
}

/**
 * @brief This callback function will be called when is new gaze data available. It is also the core 
 *        processing of eye gaze recognition. Its signature cannot be modified. 
 * @param gaze_point Is a pointer to a struct containing the gaze point data (the x and y position 
 *        points are some of them.
 * @param user_data Is a pointer with data passed unmodified to this callback function.
 */
void gaze_point_callback( tobii_gaze_point_t const* gaze_point, void* user_data)
{
    // Check that the data is valid before using it
    if( gaze_point->validity == TOBII_VALIDITY_VALID )
    {
        Point p;
        p.x = static_cast<double>(gaze_point->position_xy[ 0 ]);
        p.y = static_cast<double>(gaze_point->position_xy[ 1 ]);

        // Save the current time to use in the gaze duration.
        time_point<system_clock> curTimeStamp = system_clock::now();

        // For each region sent by the interaction manager.
        for (auto& reg : regionList)
        {
            // If the gaze point IS inside the current region.
            if (pointIsInsideRegion(reg, p))
            {
                // If the current region was already seen.
                if (reg.gazed)
                {
                    chrono::duration<double> gazeTime = curTimeStamp - reg.startTime;

                    // If the region was not sent to the formatter to start yet AND  
                    // The user is looking at it sufficient time to start the gaze event.
                    if ( (reg.regState == sleeping) && 
                         (gazeTime.count() >= durationGaze*startConstant) && 
                         (gazeTime.count() < durationGaze) )
                    {
                        if(!startRegions.empty()){
                            startRegions += ";";
                        }
                        startRegions += reg.id;
                        reg.regState = initiated;
                    }
                    // If the region was already sent to the formatter to start AND 
                    // The user already looked it sufficient time to complete the gaze event.
                    else{
                        if ((reg.regState == initiated) && (gazeTime.count() >= durationGaze))
                        {
                            if(!stopRegions.empty()){
                                stopRegions += ";";
                            }
                            stopRegions += reg.id;
                            reg.regState = finished;
                        }
                    }
                }
                //If is the user was not looking to this region before and started now.
                else
                {
                    // Marks the current region as seen.
                    reg.gazed = true;
                    reg.startTime = curTimeStamp;
                }
            }
            // If the gaze point IS NOT inside the current region.
            else
            {
                // If the user was looking to this region before and is not anymore.
                if (reg.gazed)
                {
                    // If the region was already sent to the formatter to start and needs to be abort.
                    if (reg.regState == initiated)
                    {
                        if(!abortRegions.empty()){
                            abortRegions += ";";
                        }
                        abortRegions += reg.id;
                    }
                    // Resets the current region's information
                    reg.gazed = false;
                    reg.regState = sleeping;
                }
            }
        }

        // Sent to the Interaction Manager, the media that need to start, stop, and abort.

        if (!startRegions.empty())
        {
            cout << "notified media START: " << startRegions << endl;
            sharedIntManager->notifyInteraction(Event::EYE_GAZE, Event::START, user, startRegions);
            startRegions = "";
        }

        if (!stopRegions.empty())
        {
            cout << "notified media STOP: " << stopRegions << endl;
            sharedIntManager->notifyInteraction(Event::EYE_GAZE, Event::STOP, user, stopRegions);
            stopRegions = "";
        }

        if (!abortRegions.empty())
        {
            cout << "notified media ABORT: " << abortRegions << endl;
            sharedIntManager->notifyInteraction(Event::EYE_GAZE, Event::ABORT, user, abortRegions);
            abortRegions = "";
        }
    }
}

/**
 * @brief This function will be called for each eye-tracker compatible found. Its signature cannot be modified. 
 * @param url Is the string for the device, zero-terminated.
 * @param user_data Is the custom pointer sent in to tobii_enumerate_local_device_urls 
 *        (inside eyeTrackingStart() function)..
 */
void url_receiver( char const* url, void* user_data )
{
    char* buffer = (char*)user_data;
    if( *buffer != '\0' ) return; // only keep first value

    if( strlen( url ) < 256 )
        strcpy( buffer, url );
}

/**
 * @brief Routine that will be executed by pthread_create (inside start()) once the 
 *        thread is created. This routine initializes the eye-tracker and reads the 
 *        gaze points.
 * @param data Is the thread's data.
 */
void* eyeTrackingStart(void* data)
{
    tobii_api_t* api = NULL;
    tobii_error_t result = tobii_api_create( &api, NULL, NULL );
    assert( result == TOBII_ERROR_NO_ERROR );

    // Enumerate devices to find connected eye trackers, keep the first
    char url[ 256 ] = { 0 };
    result = tobii_enumerate_local_device_urls( api, url_receiver, url );
    assert( result == TOBII_ERROR_NO_ERROR );
    if(*url == '\0')
    {
        cout << "error: no eye-tracker device found." << endl;
        return NULL;
    }

    // Connect to the first tracker found
    tobii_device_t* device = NULL;
    result = tobii_device_create( api, url, &device );
    assert( result == TOBII_ERROR_NO_ERROR );

    // Subscribe to gaze data
    result = tobii_gaze_point_subscribe( device, gaze_point_callback, 0 );
    assert( result == TOBII_ERROR_NO_ERROR );

    while(true){
        // Puts the thread to sleep until there are new callbacks available to process.
        result = tobii_wait_for_callbacks( 1, &device );
        assert( result == TOBII_ERROR_NO_ERROR || result == TOBII_ERROR_TIMED_OUT );

        // Process callbacks on this thread if data is available
        result = tobii_device_process_callbacks( device );
        assert( result == TOBII_ERROR_NO_ERROR );
    }

    // Cleanup
    result = tobii_gaze_point_unsubscribe( device );
    assert( result == TOBII_ERROR_NO_ERROR );
    result = tobii_device_destroy( device );
    assert( result == TOBII_ERROR_NO_ERROR );
    result = tobii_api_destroy( api );
    assert( result == TOBII_ERROR_NO_ERROR );
}

/**
 * @brief This function initializes some eyeGaze event parameters: the duration of the 
 *        eyeGaze event and the amount of time needed to initiate it. The parameters are 
 *        read from a JSON configuration file.
 */
void setGazeConfigurations()
{
    json jsonObj;
    ifstream configFile("interaction-config.json");

    durationGaze = 1;
    startConstant = 0.33;

    if(configFile.good())
    {
        configFile >> jsonObj;
        cout << "configuration file 'interaction-config.json' successfully read." << endl;

        if (jsonObj["eyeGaze"]["duration"].is_number())
        {
            durationGaze = static_cast<double>(jsonObj["eyeGaze"]["duration"]);
        }
        else
        {
            cout << "error: the informed duration is not a number. Thus, the default duration of 1 second will be used." << endl;
        }

        if (jsonObj["eyeGaze"]["constant"].is_number())
        {
            double auxConstant = static_cast<double>(jsonObj["eyeGaze"]["constant"]);

            if (auxConstant >= 0 && auxConstant <= 1)
            {
                startConstant = auxConstant;
            }
            else
            {
                cout << "error: the informed constant is not valid: must be between 0 and 1. Thus, the default constant of 0.33 will be used." << endl;
            }
        }
        else
        {
            cout << "error: the informed constant is not a number. Thus, the default constant of 0.33 will be used." << endl;
        }
    }
    else
    {
        cout << "error: the configuration file 'interaction-config.json' could not be found in XXXXXXX. Thus, the default configuration will be used." << endl;
    }

    configFile.close();
}



/*** GazeRecognition class functions *****************************************/

/**
 * @brief GazeRecognition's constructor. 
 * @param InteractionManager's instance.
 */
GazeRecognition::GazeRecognition(InteractionManager* manager)
{
    intManager = manager;
    run = false;
}

/**
 * @brief Starts the eye gaze recognition thread (eyeTrackingStart()).
 */
void GazeRecognition::start()
{
    if(!run){
        run = true;
        sharedIntManager = intManager;
        setGazeConfigurations();

        pthread_t pDaemon;
        if (pthread_create(&pDaemon, NULL, eyeTrackingStart, NULL))
        {
            cout << "error: failure to create a pthread client daemon." << endl;
        }
    }
}

/**
 * @brief Extracts the media's region data from a JSON object and normalizes it, populating 
 *        the regions list.
 * @param JSON object with data list of media regions (keys) and user.
 */
void GazeRecognition::setUserKeyList(json userKeyList)
{
    Region region;
    double screenWidth, screenHeight, left, top, width, height;

    user = userKeyList["user"];
	screenWidth = static_cast<double>(userKeyList["screenWidth"]);
	screenHeight = static_cast<double>(userKeyList["screenHeight"]);

    for (auto& key : userKeyList["key"])
	{
		left = ginga::parse_percent(key["left"], screenWidth, 0, G_MAXINT);
		top = ginga::parse_percent(key["top"], screenHeight, 0, G_MAXINT);
		width = ginga::parse_percent(key["width"], screenWidth, 0, G_MAXINT);
		height = ginga::parse_percent(key["height"], screenHeight, 0, G_MAXINT);

		region.id = key["id"];
		region.topLeft.x = left/screenWidth;
		region.topLeft.y = top/screenHeight;
		region.bottomRight.x = (left + width)/screenWidth;
		region.bottomRight.y = (top + height)/screenHeight;

		regionList.push_back(region);
	}
}

/**
 * @brief Stops the eye gaze recognition module.
 */
void GazeRecognition::stop()
{
    run = false;
}
