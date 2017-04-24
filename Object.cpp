#include "Object.h"

Object::Object() {
	//set values for default constructor
	setType( "Object" );
	setColor( Scalar( 0, 0, 0 ) );

}

Object::Object( string name ) {

	setType( name );

	if( name == "green" ) {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin( Scalar( 34, 50, 50 ) );
		setHSVmax( Scalar( 102, 179, 200 ) );

		//BGR value fo r Green:
		setColor( Scalar( 0, 255, 0 ) );

	}
	if( name == "black" ) {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin( Scalar( 0, 0, 0 ) );
		setHSVmax( Scalar( 170, 256, 60 ) );

		//BGR value for Black:
		setColor( Scalar( 0, 0, 0 ) );

	}
	if( name == "red" ) {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin( Scalar( 0, 21, 0 ) );
		setHSVmax( Scalar( 18, 255, 255 ) );

		//BGR value for Red:
		setColor( Scalar( 0, 0, 255 ) );

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

Scalar Object::getHSVmin() const {

	return Object::HSVmin;

}

Scalar Object::getHSVmax() const {

	return Object::HSVmax;
}

void Object::setHSVmin( Scalar min ) {

	Object::HSVmin = min;
}


void Object::setHSVmax( Scalar max ) {

	Object::HSVmax = max;
}
