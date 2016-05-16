#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "runtime_app.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TestProjectApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void TestProjectApp::setup()
{
	
}

void TestProjectApp::mouseDown( MouseEvent event )
{
}

void TestProjectApp::update()
{
}

void TestProjectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_RUNTIME_APP( TestProjectApp, RendererGl )
