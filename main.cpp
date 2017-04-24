#include "circletracking.h"
#include "colortracking.h"
#include "edgetracking.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>  

struct Skalle;


enum Type {
	Elektron,
	Neutron,
	Proton
};


struct Drawable {

	Drawable() {}

	virtual ~Drawable() {};

	virtual void draw( sf::RenderWindow& render ) = 0;
};


struct Element : public Drawable {


	Element( Type type, Point2d pos, Skalle* parent ) {
		this->pos = pos;

		this->type = type;
		switch( type ) {
			case Elektron:
				outline = sf::Color( 255, 0, 0 );
				size = 3;
				letter.setString( "E" );
				break;
			case Neutron:
				outline = sf::Color( 0, 0, 0 );
				size = 5;
				letter.setString( "N" );
				break;
			case Proton:
				outline = sf::Color( 56, 235, 60 );
				size = 5;
				letter.setString( "P" );
				break;
			default: ;
		}

		circle = sf::CircleShape( size );
		letter.setCharacterSize( 12 );
		letter.setFillColor( outline );
	}

	void draw( sf::RenderWindow& render ) override {
		circle.setPosition( pos.x, pos.y );
		letter.setPosition( pos.x + size / 2, pos.y + size / 2 );

		render.draw( letter );
		render.draw( circle );
	}

	Type type;
	Point2d pos;
	sf::Color outline = sf::Color( 98, 98, 98 );
	int size;
	sf::CircleShape circle;
	sf::Text letter;
};


struct Skalle : public Drawable {

	Skalle( Point2d* centrum, int radius, sf::Color background ) {

		this->background = background;
		this->radius = radius;
		this->centrum = centrum;
		circle = sf::CircleShape( radius, 50 );
	}

	void addElektron( Point2d pos ) {
		elektroner.push_back( Element( Elektron, pos, this ) );
	}

	void removeElektron( Point2d pos ) {
		elektroner.pop_back();
	}

	int electronCount() const { return elektroner.size(); }

	void clear() {
		elektroner.clear();
	}

	void draw( sf::RenderWindow& render ) override {
		circle.setFillColor( background );
		circle.setOutlineColor( outline );
		circle.setOutlineThickness( outline_thickness );
		circle.setPosition( centrum->x - radius - ( outline_thickness / 2 ),
		                    centrum->y - radius - ( outline_thickness / 2 ) );
		render.draw( circle );

		// tegn elektroner

		for( auto& e : elektroner ) {
			e.draw( render );
		}

	}

	std::vector< Element > elektroner;
	int nummer = 0;
	Point2d* centrum;
	int radius;
	sf::Color outline = sf::Color( 98, 98, 98 );
	int outline_thickness = 8;
	sf::Color background;
	sf::CircleShape circle;
};


struct AtomDetail : public Drawable {
	AtomDetail( int x, int y, int width, int height, sf::Font& font ) {
		pos.x = x;
		pos.y = y;
		size.x = width;
		size.y = height;

		rect = sf::RectangleShape( sf::Vector2f( width, height ) );

		setElementCount( 0, Proton );
		setElementCount( 0, Neutron );
		setElementCount( 0, Elektron );

		elektrontext.setFont( font );
		protontext.setFont( font );
		neutrontext.setFont( font );

	}

	void setElementCount( int antal, Type type ) {
		switch( type ) {

			case Elektron:
				elektroner = antal;
				elektrontext.setString( std::to_string( antal ) + " elektroner" );
				break;
			case Neutron:
				protoner = antal;
				protontext.setString( std::to_string( antal ) + " protoner" );
				break;
			case Proton:
				neutroner = antal;
				neutrontext.setString( std::to_string( antal ) + " neutroner" );
				break;
			default: ;
		}
	};

	void draw( sf::RenderWindow& render ) override {
		rect.setFillColor( background );
		rect.setOutlineColor(background2);
		rect.setPosition( pos.x, pos.y );
		render.draw( rect );

		auto y = size.y / 3;
		auto xmargin = 10;
		auto ymargin = 10;
		protontext.setPosition( pos.x + xmargin, pos.y + ymargin );
		neutrontext.setPosition( pos.x + xmargin, pos.y + y + ymargin );
		elektrontext.setPosition( pos.x + xmargin, pos.y + y * 2 + ymargin );
		protontext.setFillColor( foreground );
		neutrontext.setFillColor( foreground2 );
		elektrontext.setFillColor( foreground3 );
		neutrontext.setCharacterSize( font_size );
		protontext.setCharacterSize( font_size );
		elektrontext.setCharacterSize( font_size );

		//render.draw( protontext );
		//render.draw( neutrontext );
		//render.draw( elektrontext );

	}

	sf::Text elektrontext;
	sf::Text neutrontext;
	sf::Text protontext;
	int elektroner = 0;
	int protoner = 0;
	int neutroner = 0;
	Point2d pos;
	Point2d size;
	sf::RectangleShape rect;
	sf::Color background = sf::Color( 206, 206, 206 );
	sf::Color background2 = sf::Color(151, 151, 151);
	sf::Color foreground = sf::Color( 3, 218, 114 );
	sf::Color foreground2 = sf::Color( 46, 46, 46 );
	sf::Color foreground3 = sf::Color( 246, 3, 43 );
	unsigned font_size = 30;
};


struct AtomInfo : public Drawable {
	AtomInfo( int x, int y, int width, int height, sf::Font& font ) {
		pos.x = x;
		pos.y = y;
		size.x = width;
		size.y = height;

		rect = sf::RectangleShape( sf::Vector2f( width, height ) );
	}

	void draw( sf::RenderWindow& render ) override {
		rect.setFillColor( background );
		rect.setPosition( pos.x, pos.y );
		render.draw( rect );

		int margin = 5;

		// Atom Rect
		atomrect.setFillColor( background2 );
		atomrect.setSize( sf::Vector2f( margin * 2 - size.x, 100 ) );
		atomrect.setPosition( pos.x + margin, pos.y + margin );
	}

	Point2d pos;
	Point2d size;
	sf::RectangleShape rect;
	sf::RectangleShape atomrect;
	sf::RectangleShape inforect;
	sf::Color background = sf::Color( 206, 206, 206 );
	sf::Color background2 = sf::Color( 151, 151, 151 );
};


struct AtomCircle : public Drawable {

	AtomCircle( double pos_x, double pos_y, double r, int skaller_antal = 3 ) {
		x = pos_x;
		y = pos_y;
		radius = r;

		auto x = skaller_antal;
		do {
			auto s = Skalle( &centrum, ( radius / 3 ) * x, background );
			skaller.push_back( s );
			x -= 1;
		} while( x );
	}

	virtual ~AtomCircle() {};

	void clear() {
		for( auto& s : skaller ) {
			s.clear();
		}
	}

	void draw( sf::RenderWindow& render ) override {

		centrum.x = x + radius;
		centrum.y = y + radius;

		// tegn skaller
		for( auto& s : skaller ) {
			s.draw( render );
		}

		// tegn protoner og neutroner
		for( auto& p : protoner ) {
			p.draw( render );
		}

		for( auto& n : neutroner ) {
			n.draw( render );
		}

	}

	double x, y, radius;

	std::vector< Element > neutroner;
	std::vector< Element > protoner;
	Point2d centrum;
	sf::Color background = sf::Color( 206, 206, 206 );

	std::vector< Skalle > skaller;

};


int main( int argc, char* argv[] ) {

	int width = 640, height = 480;

	Mat sourceFeed;
	Mat cameraFeed;

	Rect crop(0, 0, 640, 480);

	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	const auto CAMERA_ID = 1;

	capture.open(CAMERA_ID);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	waitKey(1000);

	auto track = EdgeTracking();
	//track.showDebugWindows();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window( sf::VideoMode( width, height ), "KemiDetect", sf::Style::Default, settings );

	sf::Font font;
	if( !font.loadFromFile( "font/roboto.ttf" ) )
		return EXIT_FAILURE;

	int margin = 0;
	auto radius = ( ( width ) / 4 );
	auto atomcirle_x = 20;
	auto atomcirle_y = ( height / 3 );
	AtomCircle atomcircle( atomcirle_x, atomcirle_y * 2 - radius - 10, radius );
	int x = atomcircle.x + radius * 2;
	int y = 10;
	AtomInfo atominfo( y, 0, radius * 2+y, atomcirle_y * 1 - y * 3, font );
	AtomDetail atomdetail( x + 10, margin, width - x - margin, height - margin*2, font );

	while( window.isOpen() ) {

		//store image to matrix
		capture.read(sourceFeed);
		if (!sourceFeed.data)
		{
			return -1;
		}
		cameraFeed = sourceFeed(crop);
		track.applyImage(cameraFeed);
		auto objects = track.getObjects();
		namedWindow("Display", CV_WINDOW_AUTOSIZE);
		imshow("Display", track.getImage());

		// Process events
		sf::Event event;
		while( window.pollEvent( event ) ) {
			// Close window: exit
			if( event.type == sf::Event::Closed )
				window.close();
		}
		window.clear( sf::Color::White );

		atominfo.draw( window );
		atomcircle.draw( window );
		atomdetail.draw( window );

		window.display();
	}

	return 0;
}
