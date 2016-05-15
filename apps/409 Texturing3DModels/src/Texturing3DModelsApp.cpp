#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Texturing3DModelsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void Texturing3DModelsApp::setup()
{
}

void Texturing3DModelsApp::mouseDown( MouseEvent event )
{
}

void Texturing3DModelsApp::update()
{
}

void Texturing3DModelsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( Texturing3DModelsApp, RendererGl )
