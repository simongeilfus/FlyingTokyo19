#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

// uncomment this to disable runtime compilation
// #define DISABLE_RUNTIME_COMPILED_APP
#include "runtime_app.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RuntimeApp : public App {
  public:
	void setup() override;
	void draw() override;
	
	gl::BatchRef	mPlane, mTeapot;
	CameraPersp		mCamera;
	CameraUi		mCameraUi;
};

void RuntimeApp::setup()
{
	mPlane		= gl::Batch::create( geom::Plane().subdivisions( ivec2( 10 ) ) >> geom::Lines(), gl::getStockShader( gl::ShaderDef().color() ) );
	mTeapot		= gl::Batch::create( geom::Teapot() >> geom::Scale( vec3( 0.35f ) ), gl::getStockShader( gl::ShaderDef().lambert() ) );
	mCamera		= CameraPersp( getWindowWidth(), getWindowHeight(), 60, 0.1, 1000 );
	mCameraUi	= CameraUi( &mCamera, getWindow(), -1 );
	mCamera.lookAt( vec3( 1.0f, 1.0f, 1.0f ), vec3( 0.0f ) );
}

void RuntimeApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::setMatrices( mCamera );
	gl::ScopedDepth enableDepth( true );
	
	mPlane->draw();
	gl::rotate( getElapsedSeconds(), vec3( 0.0f, 1.0f, 0.0f ) );
	mTeapot->draw();
}

CINDER_RUNTIME_APP( RuntimeApp, RendererGl )
