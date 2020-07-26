#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <string>
#include <assert.h>
#include "EyeGazeModule.h"

typedef struct point {
    double x, y;
} Point;

typedef struct region {
    string id;
    Point topLeft;
    Point bottomRight;
} Region;

std::string user = "";
std::string viewedRegions = "";
vector<Region> regionList;
InteractionManager* sharedIntManager;


bool pointIsInsideRegion(Region R, Point P)
{
    return  (P.x <= R.bottomRight.x) && (P.x >= R.topLeft.x) && 
		    (P.y >= R.bottomRight.y) && (P.y <= R.topLeft.y);
}

void gaze_point_callback( tobii_gaze_point_t const* gaze_point, void* user_data)
{
    // Check that the data is valid before using it
    if( gaze_point->validity == TOBII_VALIDITY_VALID ){
        /*printf( "Gaze point: %f, %f\n",
            gaze_point->position_xy[ 0 ], 
            gaze_point->position_xy[ 1 ] );*/

        Point p;
        p.x = static_cast<double>(gaze_point->position_xy[ 0 ]);
        p.y = static_cast<double>(gaze_point->position_xy[ 1 ]);

        for (auto& reg : regionList)
        {
            if (pointIsInsideRegion(reg, p))
            {
                if(viewedRegions.empty()){
                    viewedRegions += ";";
                }
                viewedRegions += reg.id;
            }
        }

        if (!viewedRegions.empty())
        {
            sharedIntManager->notifyInteraction(
                InteractionModule::eventTransition::onEyeGaze, user, viewedRegions);
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

void eyeTrackingStart()
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

        //colocar condição de parada;
    }

    // Cleanup
    result = tobii_gaze_point_unsubscribe( device );
    assert( result == TOBII_ERROR_NO_ERROR );
    result = tobii_device_destroy( device );
    assert( result == TOBII_ERROR_NO_ERROR );
    result = tobii_api_destroy( api );
    assert( result == TOBII_ERROR_NO_ERROR );
}



EyeGazeModule::EyeGazeModule(InteractionManager* manager)
{
    intManager = manager;
}

EyeGazeModule::~EyeGazeModule()
{

}

void EyeGazeModule::start()
{
    if(!run){
        run = true;
        sharedIntManager = intManager;
        eyeTrackingStart();
    }
}

void EyeGazeModule::setUserKeyList(json userKeyList)
{
    double screenWidth, screenHeight, left, top, width, height;
    Region region;

    user = userKeyList["user"];
	screenWidth = static_cast<double>(userKeyList["screenWidth"]);
	screenHeight = static_cast<double>(userKeyList["screenHeight"]);

    for (auto& key : userKeyList["key"])
	{
		Region region;
		double left, top, width, height;

		left = static_cast<double>(key["left"]);
		top = static_cast<double>(key["top"]);
		width = static_cast<double>(key["width"]);
		height = static_cast<double>(key["height"]);

		region.id = key["id"];
		region.topLeft.x = left/screenWidth;
		region.topLeft.y = top/screenHeight;
		region.bottomRight.x = (left + width)/screenWidth;
		region.bottomRight.y = (top + height)/screenHeight;

		regionList.push_back(region);
	}
}

void EyeGazeModule::stop()
{
    run = false;
}



