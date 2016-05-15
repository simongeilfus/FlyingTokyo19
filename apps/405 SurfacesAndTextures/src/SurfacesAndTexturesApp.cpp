#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SurfacesAndTexturesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void SurfacesAndTexturesApp::setup()
{
}

void SurfacesAndTexturesApp::mouseDown( MouseEvent event )
{
}

void SurfacesAndTexturesApp::update()
{
}

void SurfacesAndTexturesApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( SurfacesAndTexturesApp, RendererGl )
