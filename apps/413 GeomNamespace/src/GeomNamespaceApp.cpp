#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GeomNamespaceApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void GeomNamespaceApp::setup()
{
}

void GeomNamespaceApp::mouseDown( MouseEvent event )
{
}

void GeomNamespaceApp::update()
{
}

void GeomNamespaceApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( GeomNamespaceApp, RendererGl )
