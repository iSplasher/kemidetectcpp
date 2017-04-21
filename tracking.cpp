#include "tracking.h"

void Tracking::applyImage( cv::Mat im ) {
	image = im;
}

void Tracking::openImage( std::string path ) {
	applyImage( cv::imread( path ) );
}

void Tracking::showDebugWindows() {
	for( auto p : debug_win ) {
		if( !p.second.empty() ) {
			imshow( p.first, p.second );
		}
	}
}

void Tracking::createDebugWindow( Mat im ) {
	auto winname = "Debug " + std::to_string( debug_num );
	namedWindow( winname, CV_WINDOW_AUTOSIZE );
	debug_num += 1;
	debug_win.push_back( DebugElement( winname, im ) );
}

void Tracking::convertBGRtoHSV() {
	if( !image.empty() ) {
		cvtColor( image, hsv, CV_BGR2HSV );
		split( hsv, channels );
	}
}
