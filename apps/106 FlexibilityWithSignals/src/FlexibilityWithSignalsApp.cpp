#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FlexibilityWithSignalsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void FlexibilityWithSignalsApp::setup()
{
}

void FlexibilityWithSignalsApp::mouseDown( MouseEvent event )
{
}

void FlexibilityWithSignalsApp::update()
{
}

void FlexibilityWithSignalsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( FlexibilityWithSignalsApp, RendererGl )
