#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class OpenGLStatesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void OpenGLStatesApp::setup()
{
}

void OpenGLStatesApp::mouseDown( MouseEvent event )
{
}

void OpenGLStatesApp::update()
{
}

void OpenGLStatesApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( OpenGLStatesApp, RendererGl )
