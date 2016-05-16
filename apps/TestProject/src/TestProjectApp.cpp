#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/ObjLoader.h"
#include "cinder/Rand.h"
#include "cinder/ip/

#include <cereal/archives/binary.hpp>
#include "CinderCereal.h"
#define RUNTIME_APP_CEREALIZATION

#include "Watchdog.h"
#include "runtime_app.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TestProjectApp : public App {
public:
	void setup() override;
	void draw() override;
	
	virtual void save( cereal::BinaryOutputArchive &ar );
	virtual void load( cereal::BinaryInputArchive &ar );
	
	gl::GlslProgRef		mGlslProg;
	gl::BatchRef		mBatch, mPlane;
	gl::Texture2dRef	mTexture;
	
	CameraPersp			mCamera;
	CameraUi			mCameraUi;
};

void TestProjectApp::setup()
{
	// Initialize the Camera and its UI
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 50.0f, 0.1f, 100.0f );
	mCamera.lookAt( vec3(5,2,0), vec3(0) );
	mCameraUi = CameraUi( &mCamera, getWindow() );
	
	// create batch
	mBatch = gl::Batch::create( geom::Sphere().subdivisions( 64 ), gl::getStockShader( gl::ShaderDef().lambert() ) );
	mPlane = gl::Batch::create( geom::Plane().subdivisions( ivec2( 64 ) ).size( vec2( 100.0f ) ) >> geom::Lines(), gl::getStockShader( gl::ShaderDef().lambert() ) );
	
	// load texture
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

void TestProjectApp::save( cereal::BinaryOutputArchive &ar )
{
	ar( mCamera.getEyePoint(), mCamera.getOrientation() );
}
void TestProjectApp::load( cereal::BinaryInputArchive &ar )
{
	vec3 eye;
	quat orientation;
	ar( eye, orientation );
	mCamera.setEyePoint( eye );
	mCamera.setOrientation( orientation );
}

CINDER_RUNTIME_APP( TestProjectApp, RendererGl, , App::SettingsFn()
#ifndef DISABLE_RUNTIME_COMPILED_APP
// The interpreter needs to know about the blocks we are using
,[] ( cling::Interpreter *interpreter ) {
   // We need to find the path starting from the location of the app
   auto appRoot = getAppPath().parent_path().parent_path().parent_path();
   auto cinderRoot = appRoot.parent_path().parent_path() / "cinder";
   auto blocksFolder = cinderRoot / "blocks";
   
   // Header-only blocks are easily added by just providing the include path
   interpreter->AddIncludePath( ( blocksFolder / "SimplexNoise/include" ).string() );
   interpreter->AddIncludePath( ( blocksFolder / "SpacePartitioning/include" ).string() );
   interpreter->AddIncludePath( ( blocksFolder / "Watchdog/include" ).string() );
   
   // Some blocks requires the main include folder but also the include folder of the library it wraps
   interpreter->AddIncludePath( ( blocksFolder / "Cinder-Cereal/include" ).string() );
   interpreter->AddIncludePath( ( blocksFolder / "Cinder-Cereal/lib/cereal/include" ).string() );
   
   // Some other blocks with .cpp files might be a bit more tricky especially when
   // there's some #defines that need to be processed before the rest like in Cinder-ImGui
   auto imguiFolder = blocksFolder / "Cinder-ImGui";
   interpreter->AddIncludePath( ( imguiFolder / "lib/imgui" ).string() );
   interpreter->AddIncludePath( ( imguiFolder / "include" ).string() );
   interpreter->loadFile( ( imguiFolder / "include/CinderImGui.h" ).string(), false );
   interpreter->enableRawInput();
   interpreter->declare( ci::loadString( ci::loadFile( imguiFolder / "lib/imgui/imgui_draw.cpp" ) ) + "\n" + ci::loadString( ci::loadFile( imguiFolder / "lib/imgui/imgui.cpp" ) ) + "\n" + ci::loadString( ci::loadFile( imguiFolder / "src/CinderImGui.cpp" ) ) );
   interpreter->enableRawInput( false );
}
#endif
)
