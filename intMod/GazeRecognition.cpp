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


typedef enum state{ 
    standby = 0,
    start, 
    stop, 
} State;

typedef struct point {
    double x, y;
} Point;

typedef struct region {
    string id;
    Point topLeft;
    Point bottomRight;
    time_point<system_clock> startTime;
    double durationGaze;
    bool gazed;
    State regState = standby;
} Region;

string user = "";
string startRegions = "";
string stopRegions = "";
string abortRegions = "";
vector<Region> regionList;
InteractionManager* sharedIntManager;


bool pointIsInsideRegion(Region R, Point P)
{
    return  (P.x >= R.topLeft.x) && (P.x <= R.bottomRight.x) && 
		    (P.y >= R.topLeft.y) && (P.y <= R.bottomRight.y);
}

void gaze_point_callback( tobii_gaze_point_t const* gaze_point, void* user_data)
{
    // Check that the data is valid before using it
    if( gaze_point->validity == TOBII_VALIDITY_VALID )
    {
        Point p;
        p.x = static_cast<double>(gaze_point->position_xy[ 0 ]);
        p.y = static_cast<double>(gaze_point->position_xy[ 1 ]);

        time_point<system_clock> curTimeStamp = system_clock::now();

        for (auto& reg : regionList)
        {
            if (pointIsInsideRegion(reg, p))
            {
                if (reg.gazed)
                {
                    chrono::duration<double> gazeTime = curTimeStamp - reg.startTime;

                    if ( (reg.regState != start) && 
                         (gazeTime.count() >= reg.durationGaze/3) && 
                         (gazeTime.count() < reg.durationGaze) )
                    {
                        if(!startRegions.empty()){
                            startRegions += ";";
                        }
                        startRegions += reg.id;
                        reg.regState = start;
                    }
                    else{
                        if ((reg.regState == start) && (gazeTime.count() >= reg.durationGaze))
                        {
                            if(!stopRegions.empty()){
                                stopRegions += ";";
                            }
                            stopRegions += reg.id;
                            reg.regState = stop;
                            reg.gazed = false;
                        }
                    }
                }
                else
                {
                    reg.gazed = true;
                    reg.regState = standby;
                    reg.startTime = curTimeStamp;
                }
            }
            else
            {
                if (reg.gazed)
                {
                    reg.gazed = false;
                    reg.regState = standby;
                    if (reg.regState == start)
                    {
                        if(!abortRegions.empty()){
                            abortRegions += ";";
                        }
                        abortRegions += reg.id;
                    }
                }
            }
        }

        if (!startRegions.empty())
        {
            cout << "\n\n Midia notificada START: " << startRegions << "\n\n";
            sharedIntManager->notifyInteraction(
                InteractionModule::eventTransition::onEyeGaze, user, startRegions);
            startRegions = "";
        }

        if (!stopRegions.empty())
        {
            cout << "\n\n Midia notificada STOP: " << stopRegions << "\n\n";
            sharedIntManager->notifyInteraction(
                InteractionModule::eventTransition::onEyeGaze, user, stopRegions);
            stopRegions = "";
        }

        if (!abortRegions.empty())
        {
            cout << "\n\n Midia notificada ABORT: " << abortRegions << "\n\n";
            sharedIntManager->notifyInteraction(
                InteractionModule::eventTransition::onEyeGaze, user, abortRegions);
            abortRegions = "";
        }
    }
}

void url_receiver( char const* url, void* user_data )
{
    char* buffer = (char*)user_data;
    if( *buffer != '\0' ) return; // only keep first value

    if( strlen( url ) < 256 )
        strcpy( buffer, url );
}

void* eyeTrackingStart(void* d)
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
        // Optionally block this thread until data is available. Especially useful if running in a separate thread.
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



GazeRecognition::GazeRecognition(InteractionManager* manager)
{
    intManager = manager;
    run = false;
}

GazeRecognition::~GazeRecognition()
{

}

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
        region.gazed = false;

		regionList.push_back(region);
	}
}

void GazeRecognition::stop()
{
    run = false;
}