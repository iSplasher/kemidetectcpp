#include "circletracking.h"
#include "colortracking.h"
#include "edgetracking.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


struct Drawable {

	Drawable() {}

	virtual ~Drawable() {};

	virtual void draw() = 0;
};


struct AtomRect : public Drawable {

	AtomRect( double width, double height ) {
		x = 0;
		y = 0;

		w = width / 2;
		h = height;
	}

	virtual ~AtomRect() {};

	void draw() override {
		slRectangleFill( x, y, w, h );

		// tegn skaller
		auto midt = Point( x + w / 2, y + h / 2 );
		auto radius = std::min( w / 2, h / 2 );

		for( int i = 1; i < skaller + 1; i++ ) {
			slCircleOutline( midt.x, midt.y, skaller / i, 10 );
		}
	}

	double x, y, w, h;

	std::vector< Object > neutroner;
	std::vector< Object > protoner;
	std::vector< Object > elektroner;

	int skaller = 4;

};


int main( int argc, char* argv[] ) {
	/*auto track = EdgeTracking();
	track.openImage( "kugle.jpg" );
	track.getObjects();

	namedWindow( "Display", CV_WINDOW_AUTOSIZE );
	imshow( "Display", track.getImage() );

	track.showDebugWindows();

	waitKey( 0 );*/
	int width = 800, height = 600;

	sf::RenderWindow window( sf::VideoMode( width, height ), "SFML window" );

	sf::Font font;
	if( !font.loadFromFile( "font/roboto.ttf" ) )
		return EXIT_FAILURE;
	sf::Text text( "Hello SFML", font, 50 );

	AtomRect atom( width, height );

	while( window.isOpen() ) {
		// Process events
		sf::Event event;
		while( window.pollEvent( event ) ) {
			// Close window: exit
			if( event.type == sf::Event::Closed )
				window.close();
		}
		// Clear screen
		window.clear( sf::Color::White );
		// Draw the sprite
		window.draw( sprite );
		// Draw the string
		window.draw( text );
		// Update the window
		window.display();
	}

	return 0;
}
