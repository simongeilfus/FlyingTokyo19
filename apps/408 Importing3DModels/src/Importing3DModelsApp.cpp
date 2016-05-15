#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Importing3DModelsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void Importing3DModelsApp::setup()
{
}

void Importing3DModelsApp::mouseDown( MouseEvent event )
{
}

void Importing3DModelsApp::update()
{
}

void Importing3DModelsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( Importing3DModelsApp, RendererGl )
