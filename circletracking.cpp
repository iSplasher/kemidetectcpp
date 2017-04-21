#include "circletracking.h"

std::vector< Object > CircleTracking::getObjects() {
	vector< Object > o;
	process();
	return o;
}

void CircleTracking::process() {
	if( image.empty() )
		return;

	convertBGRtoHSV();
	// using only v channel
	auto& v_ch = channels[ 2 ];

	// apply Otsu tresh
	threshold( v_ch, bw_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU );

	createDebugWindow( bw_image );

	// close small gaps
	auto kernel = getStructuringElement( MORPH_ELLIPSE, Size( 3, 3 ) );
	Mat morph;
	morphologyEx( bw_image, morph, CV_MOP_CLOSE, kernel );

	// take distance transform
	distanceTransform( morph, dist, CV_DIST_L2, CV_DIST_MASK_PRECISE );
	createDebugWindow( dist );

	// add a black border to distance transformed image. we are going to do
	// template matching. to get a good match for circles in the margin, we are adding a border
	auto border_size = 75;
	Mat distborder( dist.rows + 2 * border_size, dist.cols + 2 * border_size, dist.depth() );
	copyMakeBorder( dist, distborder,
	                border_size, border_size, border_size, border_size,
	                BORDER_CONSTANT | BORDER_ISOLATED, Scalar( 0, 0, 0 ) );

	// create template from the sizes of the circles in the image
	// a ~75 radius disk looks reasonable, so the borderSize was selected as 75

	auto kernel2 = getStructuringElement( MORPH_ELLIPSE, Size( 2 * border_size + 1, 2 * border_size + 1 ) );
	createDebugWindow( dist_templ );

	// erode the ~75 radius disk a bit
	erode( kernel2, kernel2, kernel, Point( -1, -1 ), 10 );

	// take its distance tranform, this is the template
	distanceTransform( kernel2, dist_templ, CV_DIST_L2, CV_DIST_MASK_PRECISE );

	// match template
	Mat nxcor;
	matchTemplate( distborder, dist_templ, nxcor, CV_TM_CCOEFF_NORMED );
	minMaxLoc( nxcor, &min, &max );

	// threshold the resulting image. we should be able to get peak regions.
	// we'll locate the peak of each of these peak regions
	Mat peaks, peaks8u;
	threshold( nxcor, peaks, max * .5, 255, CV_THRESH_BINARY );
	convertScaleAbs( peaks, peaks8u );

	// find connected components. we'll use each component as a mask for distance transformed image,
	// then extract the peak location and its strength. strength corresponds to the radius of the circle
	findContours( peaks8u, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point( 0, 0 ) );
	for( int idx = 0; idx >= 0; idx = hierarchy[ idx ][ 0 ] ) {
		// prepare the mask
		peaks8u.setTo( Scalar( 0, 0, 0 ) );
		drawContours( peaks8u, contours, idx, Scalar( 255, 255, 255 ), -1 );

		// find the max value and its location in distance transformed image using mask
		minMaxLoc( dist, nullptr, &max, nullptr, &maxLoc, peaks8u );

		// draw the circles
		circle( image, maxLoc, int( max ), Scalar( 0, 0, 255 ), 2 );
	}
}
