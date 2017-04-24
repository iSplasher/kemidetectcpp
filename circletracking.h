#pragma once

#include "tracking.h"


class CircleTracking : public Tracking {
public:

	CircleTracking() {}

	virtual ~CircleTracking() {}

	std::vector< Object >& getObjects() override;

private:

	void process();

	double min = 0, max = 0;
	vector< vector< Point > > contours;
	vector< Vec4i > hierarchy;
	Point maxLoc;

	Mat bw_image;
	Mat dist;
	Mat dist_templ;


};
