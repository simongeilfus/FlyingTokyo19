#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "CinderImGui.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImGuiApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void ImGuiApp::setup()
{
	ui::initialize();
}

void ImGuiApp::mouseDown( MouseEvent event )
{
}

void ImGuiApp::update()
{
}

void ImGuiApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( ImGuiApp, RendererGl )
