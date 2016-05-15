#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SmartPointersApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void SmartPointersApp::setup()
{
}

void SmartPointersApp::mouseDown( MouseEvent event )
{
}

void SmartPointersApp::update()
{
}

void SmartPointersApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( SmartPointersApp, RendererGl )
