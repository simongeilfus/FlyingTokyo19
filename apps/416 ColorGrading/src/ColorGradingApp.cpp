#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ColorGradingApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void ColorGradingApp::setup()
{
}

void ColorGradingApp::mouseDown( MouseEvent event )
{
}

void ColorGradingApp::update()
{
}

void ColorGradingApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( ColorGradingApp, RendererGl )
