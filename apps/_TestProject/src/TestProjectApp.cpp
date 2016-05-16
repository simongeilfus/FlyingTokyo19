#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/ObjLoader.h"
#include "cinder/Rand.h"
#include "cinder/ip/

#include "Watchdog.h"

#include "runtime_app.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TestProjectApp : public App {
  public:
	void setup() override;
	void draw() override;
	
	gl::GlslProgRef		mGlslProg;
	gl::BatchRef		mBatch, mPlane;
	CameraPersp			mCamera;
	CameraUi			mCameraUi;
	gl::Texture2dRef	mTexture;
};

void TestProjectApp::setup()
{
	// Initialize the Camera and its UI
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 50.0f, 0.1f, 100.0f );
	mCamera.lookAt( vec3(5,2,0), vec3(0) );
	mCameraUi = CameraUi( &mCamera, getWindow() );
	
	// Load shader
	geom::SourceMods cubes;
	for( int i = 0; i < 500; i++ ){
		cubes = cubes & ( geom::Teapot()
				  >> geom::Translate( vec3( randFloat( -1.0f, 1.0f ), randFloat( -1.0f, 1.0f ), randFloat( -1.0f, 1.0f ) ) * 10.0f )
				  >> geom::Scale( vec3( randFloat( 0.1f, 1.0f ) ) )
				  );
	}
	
	// create batch
	mBatch = gl::Batch::create( cubes, gl::getStockShader( gl::ShaderDef().lambert() ) );
	
	mPlane = gl::Batch::create( geom::Plane().subdivisions( ivec2( 64 ) ).size( vec2( 100.0f ) ) >> geom::Lines(), gl::getStockShader( gl::ShaderDef().lambert() ) );
	
	mTexture = gl::Texture2d::create( loadImage( loadAsset( "iceland.jpg" ) ) );
}

void TestProjectApp::draw()
{
	
	gl::clear( Color::gray( 0.5f ) );
	
	gl::ScopedMatrices scopedMatrices;
	gl::ScopedDepth	scopedDepth( true );
	gl::setMatrices( mCamera );
	
	gl::ScopedTextureBind scopedTex0( mTexture );
	gl::ScopedColor scopedColor( Color( 1,0,0 ) );
	
	mBatch->draw();
	mPlane->draw();
	
}

CINDER_RUNTIME_APP( TestProjectApp, RendererGl, []( ) )
