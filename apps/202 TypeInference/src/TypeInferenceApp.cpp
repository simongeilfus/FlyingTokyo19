#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TypeInferenceApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void TypeInferenceApp::setup()
{
}

void TypeInferenceApp::mouseDown( MouseEvent event )
{
}

void TypeInferenceApp::update()
{
}

void TypeInferenceApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( TypeInferenceApp, RendererGl )
