#include "object.h"

Object::Object() {
	//set values for default constructor
	setType( "Object" );
	setColor( cv::Scalar( 0, 0, 0 ) );
	skalle = -1;
}

Object::Object( std::string name ) {

	setType( name );

	if( name == "green" ) {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin( cv::Scalar( 34, 132, 58 ) );
		setHSVmax( cv::Scalar( 256, 170, 256 ) );

		//BGR value fo r Green:
		setColor( cv::Scalar( 0, 255, 0 ) );

	}
	if( name == "black" ) {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin( cv::Scalar( 0, 0, 0 ) );
		setHSVmax( cv::Scalar( 39, 160, 20 ) );

		//BGR value for Black:
		setColor( cv::Scalar( 0, 0, 0 ) );

	}
	if( name == "red" ) {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin( cv::Scalar( 0, 100, 0 ) );
		setHSVmax( cv::Scalar( 12, 256, 256 ) );

		//BGR value for Red:
		setColor( cv::Scalar( 0, 0, 255 ) );

	}

	if( name == "yellow" ) {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin( cv::Scalar( 0, 100, 135 ) );
		setHSVmax( cv::Scalar( 40, 256, 256 ) );

		//BGR value for Red:
		setColor( cv::Scalar( 0, 255, 255 ) );

	}

}

Object::~Object( void ) {}

int Object::getXPos() const {

	return Object::xPos;

}

void Object::setXPos( int x ) {

	Object::xPos = x;

}

int Object::getYPos() const {

	return Object::yPos;

}

void Object::setYPos( int y ) {

	Object::yPos = y;

}

cv::Scalar Object::getHSVmin() const {

	return Object::HSVmin;

}

cv::Scalar Object::getHSVmax() const {

	return Object::HSVmax;
}

void Object::setHSVmin( cv::Scalar min ) {

	Object::HSVmin = min;
}


void Object::setHSVmax( cv::Scalar max ) {

	Object::HSVmax = max;
}
