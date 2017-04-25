#include "edgetracking.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <random>

struct Skalle;

bool debug = true;


enum Type {
	Elektron,
	Neutron,
	Proton
};


std::map< int, std::string > grundstoffer = {
			{ 1, "(1) Hydrogen" },
			{ 2, "(2) Helium" },
			{ 3, "(3) Lithium" },
			{ 4, "(4) Beryllium" },
			{ 5, "(5) Bor" },
			{ 6, "(6) Carbon" },
			{ 7, "(7) Nitrogen" },
			{ 8, "(8) Oxygen" },
			{ 9, "(9) Fluor" },
			{ 10, "(10) Neon" },
			{ 11, "(11) Natrium" },
			{ 12, "(12) Magnesium" },
			{ 13, "(13) Aluminium" },
			{ 14, "(14) Silicium" },
			{ 15, "(15) Fosfor" },
			{ 16, "(16) Svovl" },
			{ 17, "(17) Klor" },
			{ 18, "(18) Argon" },
			{ 19, "(19) Kalium" },
			{ 20, "(20) Calcium" },
	};


struct Drawable {

	Drawable() {}

	virtual ~Drawable() {};

	virtual void draw( sf::RenderWindow& render ) = 0;
};


struct Element : public Drawable {


	Element( Type type, cv::Point2d pos ) {
		this->pos = pos;

		this->type = type;
		switch( type ) {
			case Elektron:
				outline = sf::Color( 255, 0, 0 );
				size = 2;
				letter.setString( "E" );
				break;
			case Neutron:
				outline = sf::Color( 0, 0, 0 );
				size = 4;
				letter.setString( "N" );
				break;
			case Proton:
				outline = sf::Color( 56, 235, 60 );
				size = 4;
				letter.setString( "P" );
				break;
			default: ;
		}

		circle = sf::CircleShape( size );
		letter.setCharacterSize( 12 );
		letter.setFillColor( outline );
		circle.setFillColor( sf::Color::White );
		circle.setOutlineColor( outline );
		circle.setOutlineThickness( 2 );
	}

	void draw( sf::RenderWindow& render ) override {
		circle.setPosition( pos.x - size, pos.y - size );
		letter.setPosition( pos.x + size / 2, pos.y + size / 2 );

		render.draw( letter );
		render.draw( circle );
	}

	Type type;
	cv::Point2d pos;
	sf::Color outline = sf::Color( 98, 98, 98 );
	int size;
	sf::CircleShape circle;
	sf::Text letter;
};


struct Skalle : public Drawable {

	Skalle( cv::Point2d* centrum, int radius, int antal_elementer, sf::Color background ) {
		this->antal_elementer = antal_elementer;
		this->background = background;
		this->radius = radius;
		this->centrum = centrum;
		circle = sf::CircleShape( radius, 50 );
	}

	void addElektron( int antal = 1 ) {
		while( antal && antal > 0 ) {
			if( elektronCount() < antal_elementer ) {
				elektroner.push_back( Element( Elektron, cv::Point2d() ) );
			}
			antal--;
		}
		updateElektroner();
	}

	void removeElektron() {
		if( elektronCount() > 0 ) {
			elektroner.pop_back();
		}
		updateElektroner();
	}

	void updateElektroner() {
		auto rad = CV_2PI / elektroner.size();

		int x = 1;
		for( auto& e : elektroner ) {
			e.pos.x = centrum->x + ( radius * std::cos( rad * x ) );
			e.pos.y = centrum->y + ( radius * std::sin( rad * x ) );
			x++;
		}
	}

	int elektronCount() const { return elektroner.size(); }

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
	int antal_elementer = 0;
	cv::Point2d* centrum;
	int radius;
	sf::Color outline = sf::Color( 98, 98, 98 );
	double outline_thickness = 0.5;
	sf::Color background;
	sf::CircleShape circle;
};


struct AtomDetail : public Drawable {
	AtomDetail( int x, int y, int width, int height, sf::Font& font ) {
		pos.x = x;
		pos.y = y;
		size.x = width;
		size.y = height;
		this->font = &font;

		rect = sf::RectangleShape( sf::Vector2f( width, height ) );

		attrs = {
					{ "Tilstandsform", getText( "Tilstandsform: Ukendt" ) },
					{ "Massefylde", getText( "Massefylde: Ukendt" ) },
					{ "Smeltepunkt", getText( "Smeltepunkt: Ukendt" ) },
					{ "Kogepunkt", getText( "Kogepunkt: Ukendt" ) },
					{ "Ladning", getText( "Ladning: Ukendt" ) },
					{ "Isotop", getText( "Isotop: Ukendt" ) }
			};

		sf::Image im;
		im.create( img_size.x, img_size.y );
		img.loadFromImage( im );
		img_sprite.setTexture( img );

		setDefault();
	}

	void setDefault() {
		for( auto& m : attrs ) {
			m.second.setString( m.first + ": " + "Ukendt" );
		}
	}

	void setImage( std::string path ) {
		sf::Image im;
		im.loadFromFile( path );
		img.loadFromImage( im );
		img_sprite.setTexture( img, true );
		auto bounds = img_sprite.getLocalBounds();
		auto scale = std::min( img_size.x / bounds.width, img_size.y / bounds.height );
		img_sprite.setScale( scale, scale );
	}

	void setTextElement( std::string key, std::string txt ) {
		attrs[ key ].setString( key + ": " + txt );
	}

	void draw( sf::RenderWindow& render ) override {
		rect.setFillColor( background );
		rect.setPosition( pos.x, pos.y );
		render.draw( rect );

		auto y = size.y / 3;
		auto xmargin = 10;
		auto ymargin = 30;

		auto s = img_sprite.getGlobalBounds();

		img_sprite.setPosition( pos.x + ( size.x / 2 ) - s.width / 2, 10 );

		render.draw( img_sprite );

		auto x = ymargin;
		for( auto& key : attrs ) {
			auto& txt = key.second;
			txt.setPosition( pos.x + xmargin, pos.y + s.height + x );
			txt.setFillColor( foreground );
			x += ymargin;
			render.draw( txt );
		}

	}

	sf::Texture img;
	sf::Sprite img_sprite;
	cv::Point2d img_size = { 270, 200 };

	std::map< std::string, sf::Text > attrs;

	sf::Font* font;
	cv::Point2d pos;
	cv::Point2d size;
	sf::RectangleShape rect;
	sf::Color background = sf::Color( 98, 98, 98 );
	sf::Color foreground = sf::Color( 3, 218, 114 );
	unsigned font_size = 20;

private:

	sf::Text getText( std::string t ) const {
		sf::Text txt;
		txt.setString( t );
		txt.setFont( *font );
		txt.setCharacterSize( font_size );
		return txt;
	}

};


struct AtomInfo : public Drawable {
	AtomInfo( int x, int y, int width, int height, sf::Font& font ) {
		pos.x = x;
		pos.y = y;
		size.x = width;
		size.y = height;

		rect = sf::RectangleShape( sf::Vector2f( width, height ) );
		atom_text = getText( "Navn: Ukendt", font );
		atom_numb = getText( "Nummer: 2", font );
		atom_elek = getText( "Elektroner: 2", font );
		atom_prot = getText( "Protoner: 5", font );
		atom_neut = getText( "Neutroner: 10", font );
		setDefault();
	}

	void setDefault() {
		atom_text.setString( "Navn: Ukendt" );
		atom_numb.setString( "Nummer: Ukendt" );
	}

	void setAtom( std::string txt, std::string nummer ) {
		atom_text.setString( "Navn: " + txt );
		atom_numb.setString( "Nummer: " + nummer );
	}

	void setElementCount( int antal, Type type ) {
		switch( type ) {

			case Elektron:
				atom_elek.setString( "Elektroner: " + std::to_string( antal ) );
				break;
			case Neutron:
				atom_neut.setString( "Neutroner: " + std::to_string( antal ) );
				break;
			case Proton:
				atom_prot.setString( "Protoner: " + std::to_string( antal ) );
				break;
			default: ;
		}
	};

	void draw( sf::RenderWindow& render ) override {
		//rect.setFillColor(background);
		rect.setOutlineThickness( 2 );
		rect.setOutlineColor( background );
		rect.setPosition( pos.x, pos.y );
		render.draw( rect );

		int margin = 2;
		int xmargin = 10;
		int margin2 = 0;

		auto txt_height = atom_text.getLocalBounds().height + 10;
		atom_text.setPosition( pos.x + xmargin, pos.y + margin );
		atom_prot.setPosition( pos.x + xmargin, pos.y + margin + margin2 + txt_height );
		atom_neut.setPosition( pos.x + xmargin, pos.y + margin + margin2 + txt_height * 2 );
		atom_elek.setPosition( pos.x + xmargin, pos.y + margin + margin2 + txt_height * 3 );

		render.draw( atom_prot );
		render.draw( atom_neut );
		render.draw( atom_elek );
		render.draw( atom_text );
	}

	cv::Point2d pos;
	cv::Point2d size;
	sf::RectangleShape rect;
	sf::Text atom_numb;
	sf::Text atom_text;
	sf::Text atom_elek;
	sf::Text atom_neut;
	sf::Text atom_prot;
	sf::Color background = sf::Color( 98, 98, 98 );
	sf::Color foreground = sf::Color( 98, 98, 98 );

private:

	sf::Text getText( std::string t, sf::Font& font ) const {
		sf::Text txt;
		txt.setString( t );
		txt.setFont( font );
		txt.setCharacterSize( 28 );
		txt.setFillColor( foreground );
		return txt;
	}
};


struct AtomCircle : public Drawable {

	AtomCircle( double pos_x, double pos_y, double r ) {
		x = pos_x;
		y = pos_y;
		radius = r;
		centrum.x = x + radius;
		centrum.y = y + radius;

		auto skaller_antal = 4;
		skaller.push_back( Skalle( &centrum, ( radius / skaller_antal ) * 4, 32, background ) );
		skaller.push_back( Skalle( &centrum, ( radius / skaller_antal ) * 3, 18, background ) );
		skaller.push_back( Skalle( &centrum, ( radius / skaller_antal ) * 2, 8, background ) );
		skaller.push_back( Skalle( &centrum, ( radius / skaller_antal ) * 1, 2, background ) );

		setElementCount( Proton, 20 );
		setElementCount( Neutron, 20 );
	}

	virtual ~AtomCircle() {};

	void clear() {
		for( auto& s : skaller ) {
			s.clear();
		}
	}

	void setElementCount( Type type, int antal, int skalle = 0 ) {
		switch( type ) {

			case Elektron:
				skaller[ skalle ].clear();
				skaller[ skalle ].addElektron( antal );
				break;
			case Neutron:
				addElements( type, neutroner, antal );
				break;
			case Proton:
				addElements( type, protoner, antal );
				break;
			default: ;
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
	cv::Point2d centrum;
	sf::Color background = sf::Color::White;

	std::vector< Skalle > skaller;

	std::mt19937 rng;

private:

	double getRandom( double min, double max ) {
		std::uniform_real_distribution< double > dist( min, max );
		rng.seed( std::random_device{}() );
		return dist( rng );
	}

	void addElements( Type type, std::vector< Element >& obj, int antal ) {
		obj.clear();

		while( antal ) {
			auto real_radius = radius / skaller.size() - 15;

			auto a = getRandom( 0, 1 );
			auto b = getRandom( 0, 1 );

			if( b < a ) {
				auto x = a;
				a = b;
				b = x;
			}

			Point2d p;
			p.x = b * real_radius * std::cos( CV_2PI * a / b ) + centrum.x;
			p.y = b * real_radius * std::sin( CV_2PI * a / b ) + centrum.y;
			obj.push_back( Element( type, p ) );
			antal--;
		}
	}

};


void setGrundstof( int nummer, AtomDetail& atomdetail, AtomInfo& atominfo ) {
	std::string root = "grundstoffer";
	auto folder = root + "/" + grundstoffer[ nummer ];
	auto txtfile = folder + "/" + "egenskaber.txt";
	auto billede = folder + "/" + "billede.jpg";

	atomdetail.setImage( billede );

	std::string line;
	ifstream file( txtfile );
	auto delimeter = ": ";
	if( file.is_open() ) {
		while( getline( file, line ) ) {
			auto key = line.substr( 0, line.find( delimeter ) );
			auto value = line.substr( line.find( delimeter ) + 1 );

			if( key == "Navn" ) {
				atominfo.setAtom( value, std::to_string( nummer ) );
			} else {
				atomdetail.setTextElement( key, value );
			}

			if( debug )
				std::cout << key << ":" << value << std::endl;
		}
		file.close();
	}

}


int main( int argc, char* argv[] ) {

	auto interval = 5;

	int width = 640, height = 480;

	cv::Mat sourceFeed;
	cv::Mat cameraFeed;

	cv::Rect crop( 0, 0, 640, 480 );

	//video capture object to acquire webcam feed
	cv::VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	const auto CAMERA_ID = 1;

	capture.open( CAMERA_ID );
	//set height and width of capture frame
	capture.set( CV_CAP_PROP_FRAME_WIDTH, width );
	capture.set( CV_CAP_PROP_FRAME_HEIGHT, height );
	cv::waitKey( 1000 );

	auto track = EdgeTracking();
	//track.showDebugWindows();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window( sf::VideoMode( width, height ), "KemiDetect", sf::Style::Default, settings );

	sf::Font font;
	if( !font.loadFromFile( "font/roboto.ttf" ) ) {
		std::cerr << "Could not load font: 'font/roboto.tff'" << std::endl;
		return EXIT_FAILURE;
	}

	int margin = 0;
	auto radius = ( ( width ) / 4 );
	auto atomcirle_x = 20;
	auto atomcirle_y = ( height / 3 );
	AtomCircle atomcircle( atomcirle_x, atomcirle_y * 2 - radius - 10, radius );
	int x = atomcircle.x + radius * 2;
	int y = 10;
	AtomInfo atominfo( y, 0, radius * 2 + y, atomcirle_y * 1 - y * 3, font );
	AtomDetail atomdetail( x + 10, margin, width - x - margin, height - margin * 2, font );
	std::vector< Object > objects;
	auto camera = false;
	capture.read( sourceFeed );
	if( sourceFeed.data ) {
		camera = true;
		cv::namedWindow( "Display", CV_WINDOW_AUTOSIZE );
	}

	while( window.isOpen() ) {
		track.resetDebugWindows();

		//capture.read( sourceFeed );

		if( debug && !camera ) {
			sourceFeed = imread( "kugle.jpg" );
		}

		if( sourceFeed.data ) {
			cameraFeed = sourceFeed; // sourceFeed( crop );
			track.applyImage( cameraFeed );
			objects = track.getObjects();

			auto elektroner = 0;
			auto protoner = -2;
			auto neutroner = 0;

			auto skalle1 = 0;
			auto skalle2 = 0;
			auto skalle3 = 0;
			auto skalle4 = 0;

			for( auto& o : objects ) {
				auto t = o.getType();

				if( t == "red" ) {
					elektroner++;

					switch( o.skalle ) {
						case 1:
							skalle1++;
							break;
						case 2:
							skalle2++;
							break;
						case 3:
							skalle3++;
							break;
						case 4:
							skalle4++;
							break;
					}

				} else if( t == "black" ) {
					neutroner++;
				} else if( t == "green" ) {
					protoner++;
				}
			}

			atomcircle.setElementCount( Elektron, skalle1, 3 );
			atomcircle.setElementCount( Elektron, skalle2 + 10, 2 );
			atomcircle.setElementCount( Elektron, skalle3, 1 );
			atomcircle.setElementCount( Elektron, skalle4, 0 );

			atomcircle.setElementCount( Proton, protoner );
			atomcircle.setElementCount( Neutron, neutroner );
			atominfo.setElementCount( protoner, Proton );
			atominfo.setElementCount( elektroner, Elektron );
			atominfo.setElementCount( neutroner, Neutron );

			// ladning
			if( elektroner > protoner ) {
				atomdetail.setTextElement( "Ladning", "Positiv" );
			} else if( elektroner < protoner ) {
				atomdetail.setTextElement( "Ladning", "Negativ" );
			} else {
				atomdetail.setTextElement( "Ladning", "Neutral" );
			}

			// isotop

			if( neutroner != protoner ) {
				atomdetail.setTextElement( "Isotop", "Ja" );
			} else {
				atomdetail.setTextElement( "Isotop", "Nej" );
			}

			if( protoner > 0 && protoner < 21 ) {
				setGrundstof( protoner, atomdetail, atominfo );
			} else {
				atomdetail.setDefault();
				atominfo.setDefault();
			}

			if( debug )
				cv::imshow( "Display", track.getImage() );
		}

		// Process events
		sf::Event ev;
		while( window.pollEvent( ev ) ) {
			// Close window: exit
			if( ev.type == sf::Event::Closed )
				window.close();
		}
		window.clear( sf::Color::White );

		atominfo.draw( window );
		atomcircle.draw( window );
		atomdetail.draw( window );

		window.display();
		if( debug )
			track.showDebugWindows();

		if( !debug )
			sf::sleep( sf::milliseconds( interval * 1000 ) );
		else
			waitKey( 1 );
	}

	return 0;
}
