#include "colortracking.h"

std::vector< Object > ColorTracking::getObjects() {
	Object red( "red" ), black( "black" ), green( "green" );

	addColorRange( red.getHSVmin(), red.getHSVmax() );
	addColorRange( black.getHSVmin(), red.getHSVmax() );
	addColorRange( green.getHSVmin(), red.getHSVmax() );

	for( auto r: color_ranges ) {
		auto circles = process( r );

		for( auto c : circles ) {
			Object o;
			o.setXPos( c[ 0 ] );
			o.setYPos( c[ 1 ] );
			o.setRadius( c[ 2 ] );

			objects.push_back( o );
		}
	}
	return objects;
}

void ColorTracking::drawObjects() {
	for( auto o : objects ) {
		circle( image, Point( o.getXPos(), o.getYPos() ), o.getRadius(), Scalar( 200, 200, 0 ) );
	}
}

std::vector< cv::Vec3f > ColorTracking::process( ColorRange range ) {
	std::vector< cv::Vec3f > circles;

	if( image.empty() )
		return circles;

	// reduce noise by blurring
	medianBlur( image, image, 3 );

	convertBGRtoHSV();

	cv::GaussianBlur( hsv, hsv, cv::Size( 9, 9 ), 2, 2 );

	// tresh image, keep on image in color range
	Mat in_range;
	inRange( hsv, range.first, range.second, in_range );
	temp_imgs.push_back( in_range );
	createDebugWindow( in_range );

	// blur again to void false positives

	// Use the Hough transform to detect circles in the image

	//cv::HoughCircles( in_range, circles, CV_HOUGH_GRADIENT, 1, 1, 150, 50, 0, 0 );
	return circles;
}
