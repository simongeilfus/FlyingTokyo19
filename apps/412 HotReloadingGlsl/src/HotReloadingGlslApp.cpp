#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HotReloadingGlslApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void HotReloadingGlslApp::setup()
{
}

void HotReloadingGlslApp::mouseDown( MouseEvent event )
{
}

void HotReloadingGlslApp::update()
{
}

void HotReloadingGlslApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( HotReloadingGlslApp, RendererGl )
