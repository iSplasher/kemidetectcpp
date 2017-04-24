#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;


class Object {
public:
	Object();

	~Object(void);

	Object(string name);

	int getXPos() const;

	void setXPos(int x);

	int getYPos() const;

	void setYPos(int y);

	int getRadius() const { return radius; };

	void setRadius(int r) { radius = r; };

	Scalar getHSVmin() const;

	Scalar getHSVmax() const;

	void setHSVmin(Scalar min);

	void setHSVmax(Scalar max);

	string getType() const { return type; }
	void setType(string t) { type = t; }

	Scalar getColor() const {
		return Color;
	}

	void setColor(Scalar c) {

		Color = c;
	}

private:

	int xPos, yPos, radius;
	string type;
	Scalar HSVmin, HSVmax;
	Scalar Color;
};
