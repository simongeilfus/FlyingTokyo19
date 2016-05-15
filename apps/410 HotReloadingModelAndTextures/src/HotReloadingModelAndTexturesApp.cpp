#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HotReloadingModelAndTexturesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void HotReloadingModelAndTexturesApp::setup()
{
}

void HotReloadingModelAndTexturesApp::mouseDown( MouseEvent event )
{
}

void HotReloadingModelAndTexturesApp::update()
{
}

void HotReloadingModelAndTexturesApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( HotReloadingModelAndTexturesApp, RendererGl )
