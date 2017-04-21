#pragma once

#include "tracking.h"


class EdgeTracking : public Tracking {
public:

	EdgeTracking() {}

	virtual ~EdgeTracking() {}

	std::vector< Object > getObjects() override;

private:

	void process( Mat );

	void drawObject( vector< Object > theObjects, Mat& frame, Mat& temp, vector< vector< Point > > contours, vector< Vec4i > hierarchy ) const;

	void drawObject( vector< Object > theObjects, Mat& frame ) const;

	static void morphOps( Mat& thresh );

	int trackFilteredObject( Mat threshold, Mat HSV, Mat& cameraFeed ) const;

	int trackFilteredObject( Object theObject, Mat threshold, Mat HSV, Mat& cameraFeed ) const;

	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = false;

	int H_MIN = 0;
	int H_MAX = 256;
	int S_MIN = 0;
	int S_MAX = 256;
	int V_MIN = 0;
	int V_MAX = 256;

	//Matrix to store each frame of the webcam feed
	Mat sourceFeed;
	Mat cameraFeed;
	Mat threshold;
	Rect crop = Rect( 80, 0, 500, 480 );

	//The following for canny edge detec
	Mat dst, detected_edges;
	Mat src, src_gray;
	int edgeThresh = 1;
	int lowThreshold;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	const char* window_name = "Edge Map";

	const int MAX_NUM_OBJECTS = 100;
	//minimum and maximum object area
	const int MIN_OBJECT_AREA = 10 * 10;


};
