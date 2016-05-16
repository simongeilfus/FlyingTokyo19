#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

#include <cereal/archives/binary.hpp>
#include "CinderCereal.h"
#define RUNTIME_APP_CEREALIZATION

#include "runtime_app.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PreservingStateApp : public App {
  public:
	void setup() override;
	void draw() override;
	
	virtual void save( cereal::BinaryOutputArchive &ar );
	virtual void load( cereal::BinaryInputArchive &ar );
	
	CameraPersp mCamera;
	CameraUi	mCameraUi;
};

void PreservingStateApp::setup()
{
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 50.0f, 0.1f, 1000.0f ).calcFraming( Sphere( vec3(0.0f), 1.0f ) );
	mCameraUi = CameraUi( &mCamera, getWindow() );
}

void PreservingStateApp::draw()
{
	gl::clear( Color::gray( 0.25f ) );
	gl::setMatrices( mCamera );
	
	gl::ScopedDepth scopedDepth( true );
	gl::drawColorCube( vec3(0.0f), vec3(1.0f) );
}

void PreservingStateApp::save( cereal::BinaryOutputArchive &ar )
{
	ar( mCamera.getEyePoint(), mCamera.getOrientation() );
}
void PreservingStateApp::load( cereal::BinaryInputArchive &ar )
{
	vec3 eye;
	quat orientation;
	ar( eye, orientation );
	mCamera.setEyePoint( eye );
	mCamera.setOrientation( orientation );
}

CINDER_RUNTIME_APP( PreservingStateApp, RendererGl, App::SettingsFn()
#ifndef DISABLE_RUNTIME_COMPILED_APP
// The interpreter needs to know about the blocks we are using
,[] ( cling::Interpreter *interpreter ) {
   // We need to find the path starting from the location of the app
   auto appRoot = getAppPath().parent_path().parent_path().parent_path();
   auto cinderRoot = appRoot.parent_path().parent_path() / "cinder";
   auto blocksFolder = cinderRoot / "blocks";
   interpreter->AddIncludePath( ( blocksFolder / "Cinder-Cereal/include" ).string() );
   interpreter->AddIncludePath( ( blocksFolder / "Cinder-Cereal/lib/cereal/include" ).string() );
}
#endif
)
