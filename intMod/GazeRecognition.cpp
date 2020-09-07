#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <string>
#include <assert.h>
#include <chrono>
#include <ctime>
#include <iostream>
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

//Media Region's possible states.
typedef enum state{ 
    sleeping = 0,
    iniciated, 
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
    double durationGaze; // Amount of much time that the user must look at the region.
    bool gazed = false;
    State regState = sleeping;
} Region;

string user = "";
string startRegions = "";
string stopRegions = "";
string abortRegions = "";
vector<Region> regionList;
InteractionManager* sharedIntManager;


//Auxiliary functions ****************************************

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
                         (gazeTime.count() >= reg.durationGaze/3) && 
                         (gazeTime.count() < reg.durationGaze) )
                    {
                        if(!startRegions.empty()){
                            startRegions += ";";
                        }
                        startRegions += reg.id;
                        reg.regState = iniciated;
                    }
                    // If the region was already sent to the formatter to start AND 
                    // The user already looked it sufficient time to complete the gaze event.
                    else{
                        if ((reg.regState == iniciated) && (gazeTime.count() >= reg.durationGaze))
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
                    if (reg.regState == iniciated)
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
            cout << "\n\n Midia notificada START: " << startRegions << "\n\n";
            sharedIntManager->notifyInteraction(Event::EYE_GAZE, Event::START, user, startRegions);
            startRegions = "";
        }

        if (!stopRegions.empty())
        {
            cout << "\n\n Midia notificada STOP: " << stopRegions << "\n\n";
            sharedIntManager->notifyInteraction(Event::EYE_GAZE, Event::STOP, user, stopRegions);
            stopRegions = "";
        }

        if (!abortRegions.empty())
        {
            cout << "\n\n Midia notificada ABORT: " << abortRegions << "\n\n";
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
        printf("Error: No device found\n");
        //return 1;
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


//GazeRecognition class functions ****************************************

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

        pthread_t pDaemon;
        if (pthread_create(&pDaemon, NULL, eyeTrackingStart, NULL))
        {
            printf("Error: failure to create a pthread client daemon");
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
    double duration;
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

        region.durationGaze = static_cast<double>(key["duration"]);

        if (key["duration"].empty()){
            region.durationGaze = 1;
        }
        else{
            region.durationGaze = static_cast<double>(key["duration"]);
        }

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
