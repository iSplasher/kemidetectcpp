#pragma once

#include <opencv2/opencv.hpp>
#include <memory>
#include <vector>

#include "object.h"


class Tracking {
public:
	Tracking() {}
	virtual ~Tracking() {}

	void applyImage( cv::Mat im );

	void openImage( std::string path );

	virtual std::vector< Object > getObjects() = 0;

	Mat getImage() const { return image; }

	void showDebugWindows();

protected:

	void createDebugWindow( Mat im );

	void convertBGRtoHSV();

	cv::Mat image;
	Mat hsv;
	Mat channels[3];

private:
	using DebugElement = std::pair< std::string, Mat >;
	std::vector< DebugElement > debug_win;
	unsigned debug_num = 0;

};
