#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VertexBatchApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void VertexBatchApp::setup()
{
}

void VertexBatchApp::mouseDown( MouseEvent event )
{
}

void VertexBatchApp::update()
{
}

void VertexBatchApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( VertexBatchApp, RendererGl )
