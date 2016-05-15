#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HotReloadingImagesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void HotReloadingImagesApp::setup()
{
}

void HotReloadingImagesApp::mouseDown( MouseEvent event )
{
}

void HotReloadingImagesApp::update()
{
}

void HotReloadingImagesApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( HotReloadingImagesApp, RendererGl )
