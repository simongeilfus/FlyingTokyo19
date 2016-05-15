#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LambdasAndFunctionsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void LambdasAndFunctionsApp::setup()
{
}

void LambdasAndFunctionsApp::mouseDown( MouseEvent event )
{
}

void LambdasAndFunctionsApp::update()
{
}

void LambdasAndFunctionsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( LambdasAndFunctionsApp, RendererGl )
