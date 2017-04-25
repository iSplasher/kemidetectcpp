#include "edgetracking.h"
#include <mutex>

std::vector< Object >& EdgeTracking::getObjects() {
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
	Object black( "black" ), red( "red" ), green( "green" );

	//first find black objects
	convertBGRtoHSV();
	inRange( hsv, black.getHSVmin(), black.getHSVmax(), threshold );
	createDebugWindow( threshold );
	morphOps( threshold );
	createDebugWindow( threshold );

	//auto black_count = trackFilteredObject( black, threshold, hsv, cameraFeed );
	////then reds
	//cvtColor( cameraFeed, hsv, COLOR_BGR2HSV );
	//inRange( hsv, red.getHSVmin(), red.getHSVmax(), threshold );
	//createDebugWindow( threshold );
	//morphOps( threshold );
	//auto red_count = trackFilteredObject( red, threshold, hsv, cameraFeed );
	////then greens
	//cvtColor( cameraFeed, hsv, COLOR_BGR2HSV );
	//inRange( hsv, green.getHSVmin(), green.getHSVmax(), threshold );
	//createDebugWindow( threshold );
	//morphOps( threshold );
	//auto green_count = trackFilteredObject( green, threshold, hsv, cameraFeed );

}

void EdgeTracking::drawObject( vector< Object > theObjects, Mat& frame, Mat& temp, vector< vector< Point > > contours, vector< Vec4i > hierarchy ) const {
	for( int i = 0; i < theObjects.size(); i++ ) {
		cv::drawContours( frame, contours, i, theObjects.at( i ).getColor(), 3, 8, hierarchy );
		cv::circle( frame, cv::Point( theObjects.at( i ).getXPos(), theObjects.at( i ).getYPos() ), 5, theObjects.at( i ).getColor() );
		cv::putText( frame, std::to_string( theObjects.at( i ).getXPos() ) + " , " + std::to_string( theObjects.at( i ).getYPos() ), cv::Point( theObjects.at( i ).getXPos(), theObjects.at( i ).getYPos() + 20 ), 1, 1, theObjects.at( i ).getColor() );
		cv::putText( frame, theObjects.at( i ).getType(), cv::Point( theObjects.at( i ).getXPos(), theObjects.at( i ).getYPos() - 20 ), 1, 2, theObjects.at( i ).getColor() );
	}
}

void EdgeTracking::morphOps( Mat& thresh ) {

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement( MORPH_ELLIPSE, Size( 5, 5 ) );
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_ELLIPSE, Size( 5, 5 ) );

	erode( thresh, thresh, erodeElement );
	erode( thresh, thresh, erodeElement );

	dilate( thresh, thresh, dilateElement );
	dilate( thresh, thresh, dilateElement );
}

int EdgeTracking::trackFilteredObject( Object theObject, Mat threshold, Mat HSV, Mat& cameraFeed ) {
	objects.clear();
	Mat temp;
	threshold.copyTo( temp );
	//these two vectors needed for output of findContours
	vector< vector< Point > > contours;
	vector< Vec4i > hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if( hierarchy.size() > 0 ) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if( numObjects < MAX_NUM_OBJECTS ) {
			for( int index = 0; index >= 0; index = hierarchy[ index ][ 0 ] ) {

				Moments moment = moments( ( cv::Mat )contours[ index ] );
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if( area > MIN_OBJECT_AREA ) {

					Object object;

					object.setXPos( moment.m10 / area );
					object.setYPos( moment.m01 / area );
					object.setType( theObject.getType() );
					object.setColor( theObject.getColor() );

					objects.push_back( object );

					objectFound = true;

				} else
					objectFound = false;
			}
			//let user know you found an object
			if( objectFound == true ) {
				//draw object location on screen
				drawObject( objects, cameraFeed, temp, contours, hierarchy );
			}

		} else
			putText( cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point( 0, 50 ), 1, 2, Scalar( 0, 0, 255 ), 2 );
	}
	return objects.size();
}
