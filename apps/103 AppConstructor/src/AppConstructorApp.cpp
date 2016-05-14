#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AppConstructorApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void AppConstructorApp::setup()
{
}

void AppConstructorApp::mouseDown( MouseEvent event )
{
}

void AppConstructorApp::update()
{
}

void AppConstructorApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( AppConstructorApp, RendererGl )
