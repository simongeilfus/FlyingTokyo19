#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class NamespacesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void NamespacesApp::setup()
{
}

void NamespacesApp::mouseDown( MouseEvent event )
{
}

void NamespacesApp::update()
{
}

void NamespacesApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( NamespacesApp, RendererGl )
