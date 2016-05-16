#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

#include "runtime_ptr.h"
#include "Object.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RuntimePointerBasicsApp : public App {
public:
	void setup() override;
	void draw() override;
	
	CameraPersp		mCamera;
	CameraUi		mCameraUi;
	
	runtime_ptr<Object> mPtr;
};

void RuntimePointerBasicsApp::setup()
{
/*
 // By default runtime_ptr will try to find the source files automatically but
 // you can also manually specify them or provide more interpreter options like this :
#ifndef DISABLE_RUNTIME_COMPILED_PTR
	// We're passing a Options().cinder() as we're using Cinder in the class code
	runtime_class<Object>::initialize( "Object.cpp", runtime_class<Object>::Options().cinder() );
#endif
 */
	
	// create the runtime pointer
	mPtr = make_runtime<Object>();
	
	// create a camera and a camera ui
	mCamera	= CameraPersp( getWindowWidth(), getWindowHeight(), 60, 0.1, 1000 ).calcFraming( Sphere( vec3(0), 2 ) );
	mCameraUi = CameraUi( &mCamera, getWindow(), -1 );
}

void RuntimePointerBasicsApp::draw()
{
	gl::clear( Color::gray( 0.05f ) );
	gl::setMatrices( mCamera );
	
	// the pointer could become invalid so
	// it's a good idea to wrap it in a "if"
	if( mPtr ) {
		mPtr->draw();
	}
}

CINDER_APP( RuntimePointerBasicsApp, RendererGl )
