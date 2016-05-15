#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class StockShadersApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void StockShadersApp::setup()
{
}

void StockShadersApp::mouseDown( MouseEvent event )
{
}

void StockShadersApp::update()
{
}

void StockShadersApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( StockShadersApp, RendererGl )
