#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// A really basic button class
class BasicButton {
public:
	BasicButton( const string &name, const Rectf &rect );
	void draw();
protected:
	string	mName;
	Rectf	mBounds;
};

// A small App with just a constructor and a std::vector of BasicButtons
class FlexibilityWithSignals : public App {
public:
	FlexibilityWithSignals()
	{
		// get the app's window and connect
		// a small lambda to its draw signal
		auto window = getWindow();
		window->getSignalDraw().connect( [this]{
			// clear the background
			gl::clear( Color::gray( 0.5f ) );
			// draw the buttons
			for( auto button : mButtons ) button.draw();
		} );
		
		// add another small lambda to the update function
		getSignalUpdate().connect( [this](){
			getWindow()->setTitle( to_string( (int) getAverageFps() ) + " fps" );
		} );
		
		// create two buttons
		mButtons = {
			BasicButton( "Button0", Rectf( vec2( 20, 20 ), vec2( 200, 140 ) ) ),
			BasicButton( "Button1", Rectf( vec2( 20, 160 ), vec2( 200, 280 ) ) ),
		};
	}
	
	vector<BasicButton> mButtons;
};

// The BasicButton class implementation

BasicButton::BasicButton( const std::string &name, const ci::Rectf &rect )
: mName( name ), mBounds( rect )
{
	// subscribe to mouse down event of the app window
	ci::app::getWindow()->getSignalMouseDown().connect( [this]( ci::app::MouseEvent event ) {
		// Check if the mouse is inside the bounds of this button
		if( mBounds.contains( event.getPos() ) ) {
			ci::app::console() << mName << " clicked!" << std::endl;
		}
	});
}
void BasicButton::draw()
{
	gl::drawStrokedRoundedRect( mBounds, 12.5f );
	gl::drawStringCentered( mName, mBounds.getCenter() );
}

CINDER_APP( FlexibilityWithSignals, RendererGl, []( App::Settings *settings ) {
 settings->setWindowSize( ivec2( 220, 300 ) );
})
