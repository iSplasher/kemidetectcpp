#pragma once

#include "tracking.h"


class ColorTracking : public Tracking {
public:

	ColorTracking() {}

	virtual ~ColorTracking() {}

	std::vector< Object >& getObjects() override;

	void addColorRange( Scalar from, Scalar to ) { color_ranges.push_back( ColorRange( from, to ) ); }

	void drawObjects();

private:

	using ColorRange = std::pair< Scalar, Scalar >;

	std::vector< cv::Vec3f > process( ColorRange );

	vector< vector< Point > > contours;
	vector< Vec4i > hierarchy;

	vector< ColorRange > color_ranges;
	std::vector< Object > objects;

	std::vector< Mat > temp_imgs;

};
