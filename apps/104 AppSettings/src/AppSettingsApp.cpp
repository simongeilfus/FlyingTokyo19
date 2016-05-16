#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AppSettings : public App {
  public:
	void setup() override;
	void draw() override { gl::clear(); }
};

void AppSettings::setup()
{
	// You can set a few options here but as
	// the app has already started at that point,
	// you might want to use the next option
	setWindowPos( ivec2( 20, 300 ) );
}

// This is the prefered way of changing the app settings as it will
// allow the app to actually starts with the settings that you want.
CINDER_APP( AppSettings, RendererGl, []( App::Settings *settings ) {
	settings->setWindowSize( ivec2( 200, 60 ) );
	settings->setTitle( "App Settings" );
	settings->setResizable( false );
	
	auto displays = Display::getDisplays();
	if( displays.size() > 1 ) {
		settings->setDisplay( displays[1] );
	}
	
} )

/*
// A bit less compact and elegant but it does exactly 
// the same but with a static function:
void mySettings( App::Settings *settings )
{
	settings->setWindowSize( ivec2( 200, 60 ) );
	settings->setTitle( "App Settings" );
	settings->setResizable( false );
}

CINDER_APP( AppSettings, RendererGl, mySettings )
*/
