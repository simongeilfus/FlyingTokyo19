#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HelperFunctionsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void HelperFunctionsApp::setup()
{
}

void HelperFunctionsApp::mouseDown( MouseEvent event )
{
}

void HelperFunctionsApp::update()
{
}

void HelperFunctionsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( HelperFunctionsApp, RendererGl )
