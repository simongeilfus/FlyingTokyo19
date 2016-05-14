#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AppSettingsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void AppSettingsApp::setup()
{
}

void AppSettingsApp::mouseDown( MouseEvent event )
{
}

void AppSettingsApp::update()
{
}

void AppSettingsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( AppSettingsApp, RendererGl )
