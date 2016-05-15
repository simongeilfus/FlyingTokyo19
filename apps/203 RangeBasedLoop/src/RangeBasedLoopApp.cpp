#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RangeBasedLoopApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void RangeBasedLoopApp::setup()
{
}

void RangeBasedLoopApp::mouseDown( MouseEvent event )
{
}

void RangeBasedLoopApp::update()
{
}

void RangeBasedLoopApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( RangeBasedLoopApp, RendererGl )
