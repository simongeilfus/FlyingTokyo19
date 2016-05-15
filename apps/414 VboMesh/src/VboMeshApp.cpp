#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VboMeshApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void VboMeshApp::setup()
{
}

void VboMeshApp::mouseDown( MouseEvent event )
{
}

void VboMeshApp::update()
{
}

void VboMeshApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( VboMeshApp, RendererGl )
