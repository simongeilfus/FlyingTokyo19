#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MultipleWindowApp : public App {
public:
	MultipleWindowApp();
	void mouseDown( MouseEvent event ) override;
	void drawWindow0() { gl::clear( mColorPalette[1] ); }
	void drawWindow1() { gl::clear( mColorPalette[2] ); }
	void drawWindow2() { gl::clear( mColorPalette[3] ); }
	
	Color mColorPalette[5] = { Color::hex( 0xB2403F ), Color::hex( 0xFFFE8D ), Color::hex( 0xFF7573 ), Color::hex( 0x4896CC ), Color::hex( 0x4887B2 ) };
};
MultipleWindowApp::MultipleWindowApp()
{
	// Connect the windows to their respective signals
	getWindowIndex( 0 )->getSignalDraw().connect( bind( &MultipleWindowApp::drawWindow0, this ) );
	getWindowIndex( 1 )->getSignalDraw().connect( bind( &MultipleWindowApp::drawWindow1, this ) );
	getWindowIndex( 2 )->getSignalDraw().connect( bind( &MultipleWindowApp::drawWindow2, this ) );
}
void MultipleWindowApp::mouseDown( MouseEvent event )
{
	// create a new window next to the one that received a mouseDown event
	auto windowPos	= getWindowPos();
	auto window		= createWindow( Window::Format().pos( windowPos + ivec2( 256, 0 ) ).size( ivec2( 256 ) ) );
	// pick a random color in our palette and clear the new window with it
	auto randColor	= mColorPalette[randInt(0, 5)];
	window->getSignalDraw().connect( [randColor](){ gl::clear( randColor ); } );
}

CINDER_APP( MultipleWindowApp, RendererGl, []( App::Settings *settings ) {
	// prepare the app with 3 small windows
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 40 ) ).title( "Window 0" ) );
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 320 ) ).title( "Window 1" ) );
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 576 ) ).title( "Window 2" ) );
})
