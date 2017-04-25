#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>

#include "object.h"


class Tracking {
public:
	Tracking( bool debug = false ) {
		this->debug = debug;
	}

	virtual ~Tracking() {}

	void applyImage( cv::Mat im );

	void openImage( std::string path );

	virtual std::vector< Object >& getObjects() = 0;

	Mat getImage() const { return image; }

	void showDebugWindows();

	void resetDebugWindows();

protected:

	void createDebugWindow( Mat& im );

	void convertBGRtoHSV();

	cv::Mat image;
	Mat hsv;
	Mat gray;
	Mat channels[3];

private:

	bool debug;
	using DebugElement = std::pair< std::string, Mat >;
	std::map< std::string, Mat > debug_win;
	unsigned debug_num = 0;

};
