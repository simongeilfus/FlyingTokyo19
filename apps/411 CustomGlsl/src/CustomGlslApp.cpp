#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CustomGlslApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void CustomGlslApp::setup()
{
}

void CustomGlslApp::mouseDown( MouseEvent event )
{
}

void CustomGlslApp::update()
{
}

void CustomGlslApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( CustomGlslApp, RendererGl )
