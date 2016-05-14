#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MultipleWindowApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void MultipleWindowApp::setup()
{
}

void MultipleWindowApp::mouseDown( MouseEvent event )
{
}

void MultipleWindowApp::update()
{
}

void MultipleWindowApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( MultipleWindowApp, RendererGl )
