#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AppEventsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void AppEventsApp::setup()
{
}

void AppEventsApp::mouseDown( MouseEvent event )
{
}

void AppEventsApp::update()
{
}

void AppEventsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( AppEventsApp, RendererGl )
