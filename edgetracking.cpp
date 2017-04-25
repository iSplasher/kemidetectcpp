#include "edgetracking.h"
#include <mutex>

std::vector< Object >& EdgeTracking::getObjects() {
	objects.clear();
	process( image );
	return objects;
}

void EdgeTracking::process( Mat sourceFeed ) {

	cameraFeed = sourceFeed;
	src = cameraFeed;
	image = cameraFeed;

	if( !src.data )
		return;

	//create some temp objects so that
	Object black( "black" ), red( "red" ), green( "green" ), yellow( "yellow" );

	//first find black objects
	convertBGRtoHSV();

	//first find black objects
	inRange( hsv, black.getHSVmin(), black.getHSVmax(), threshold );
	createDebugWindow( threshold );
	morphOps( threshold );
	auto black_count = trackFilteredObject( black, threshold, hsv, image );
	//std::cout << "Black: " << black_count;
	createDebugWindow( image );
	//then reds
	inRange( hsv, red.getHSVmin(), red.getHSVmax(), threshold );
	createDebugWindow( threshold );
	morphOps( threshold );
	auto red_count = trackFilteredObject( red, threshold, hsv, image );
	//std::cout << " Red: " << red_count;
	createDebugWindow( image );
	//then greens
	inRange( hsv, green.getHSVmin(), green.getHSVmax(), threshold );
	createDebugWindow( threshold );
	morphOps( threshold );
	auto green_count = trackFilteredObject( green, threshold, hsv, image );
	//std::cout << " Green: " << green_count;
	createDebugWindow( image );
	//then yellow
	inRange( hsv, yellow.getHSVmin(), yellow.getHSVmax(), threshold );
	createDebugWindow( threshold );
	morphOps( threshold );
	auto yellow_count = trackFilteredObject( yellow, threshold, hsv, image );
	//std::cout << " Yellow: " << yellow_count << std::endl;
	createDebugWindow( image );

	// calculate skaller

	Object* yellow_obj = nullptr;
	std::vector< Object* > elektroner;
	if( yellow_count ) {
		for( auto& o : objects ) {
			if( o.getType() == "yellow" ) {
				yellow_obj = &o;
			} else if( o.getType() == "red" ) {
				elektroner.push_back( &o );
			}
		}

		if( yellow_obj ) {
			int skaller = 4;
			int radius = 80;
			int interval = 50;
			int error_radius = 20;

			auto x = 1;
			while( x < skaller + 1 ) {

				circle( image, Point( yellow_obj->getXPos(), yellow_obj->getYPos() ), radius, ( 246 , 0 , 255 ) );

				createDebugWindow( image );

				// check if any elektroner are close to it

				for( auto& e : elektroner ) {
					int x1, x2, y1, y2;
					x1 = e->getXPos();
					y1 = e->getYPos();
					x2 = yellow_obj->getXPos();
					y2 = yellow_obj->getYPos();

					if( e->skalle == -1 ) {
						auto center_dist = std::sqrt( std::pow( ( x2 - x1 ), 2 ) + std::pow( ( y2 - y1 ), 2 ) );
						auto delta_dist = center_dist - radius;
						if( delta_dist < error_radius ) {
							e->skalle = x;
						}
					}
				}

				radius += interval;
				x++;
			}
		}
	}

}

void EdgeTracking::morphOps( Mat& thresh ) {

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement( MORPH_ELLIPSE, Size( 2, 2 ) );
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_ELLIPSE, Size( 6, 6 ) );

	erode( thresh, thresh, erodeElement );
	erode( thresh, thresh, erodeElement );

	dilate( thresh, thresh, dilateElement );
	dilate( thresh, thresh, dilateElement );
}

int EdgeTracking::trackFilteredObject( Object theObject, Mat threshold, Mat HSV, Mat& cameraFeed ) {
	Mat temp;
	threshold.copyTo( temp );
	//these two vectors needed for output of findContours
	vector< vector< Point > > contours;
	vector< Vec4i > hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	auto count = 0;
	if( hierarchy.size() > 0 ) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if( numObjects < MAX_NUM_OBJECTS ) {
			for( int index = 0; index >= 0; index = hierarchy[ index ][ 0 ] ) {

				Moments moment = moments( ( cv::Mat )contours[ index ] );
				double area = moment.m00;

				if( area > MIN_OBJECT_AREA ) {

					Object object;

					object.setXPos( moment.m10 / area );
					object.setYPos( moment.m01 / area );
					object.setType( theObject.getType() );
					object.setColor( theObject.getColor() );

					objects.push_back( object );

					count++;
				}
			}

		} else
			putText( cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point( 0, 50 ), 1, 2, Scalar( 0, 0, 255 ), 2 );
	}
	return count;
}
