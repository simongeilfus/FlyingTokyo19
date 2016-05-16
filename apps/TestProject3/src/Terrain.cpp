#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/ObjLoader.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

#include "Simplex.h"

#include <cereal/archives/binary.hpp>
#include "CinderCereal.h"
#define RUNTIME_APP_CEREALIZATION

#include "Watchdog.h"
#include "CinderImGui.h"
#include "runtime_app.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TestProjectApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void resize() override;
	
	virtual void save( cereal::BinaryOutputArchive &ar );
	virtual void load( cereal::BinaryInputArchive &ar );

	
	float				mPerlinScale;
	float				mPerlinStrength;
	Perlin				mPerlin;
	
	vector<gl::BatchRef> mScene;
	
	
	CameraPersp			mCamera;
	CameraUi			mCameraUi;
};

void TestProjectApp::setup()
{
	getWindow()->setAlwaysOnTop();
	
	// Initialize the Camera and its UI
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 50.0f, 0.1f, 100.0f );
	mCamera.lookAt( vec3(5,2,0), vec3(0) );
	mCameraUi = CameraUi( &mCamera, getWindow(), -1 );
	
	// initialize ui
	ui::initialize();
	
	// noise helper
	auto noise = [this]( const vec2 &pos ) {
		return mPerlin.fBm( pos * mPerlinScale ) * mPerlinStrength;
	};
	
	// terrain
	mPerlin.setOctaves( 4 );
	mPerlinScale		= 0.1f;
	mPerlinStrength		= 2.0f;
	auto terrain		= TriMesh( geom::Plane().subdivisions( ivec2( 100 ) ).size( vec2( 10 ) ), TriMesh::Format().positions().normals() );
	auto numVertices	= terrain.getNumVertices();
	auto positions		= terrain.getPositions<3>();
	for( size_t i = 0; i < numVertices; i++ ) {
		positions[i].y	= noise( vec2( positions[i].x, positions[i].z ) );
	}
	terrain.recalculateNormals();
	
	mScene.push_back( gl::Batch::create( terrain, gl::getStockShader( gl::ShaderDef().lambert() ) ) );
	
	// buildings
	geom::SourceMods buildings;
	
	for( int i = 0; i < 200; ++i ) {
		auto buildingPos	= vec3( randFloat( -5.0f, 5.0f ), 0.0f, randFloat( -5.0f, 5.0f ) );
		auto buildingSize	= vec3( randFloat( 0.05f, 0.2f ), randFloat( 0.2f, 0.4f ), randFloat( 0.05f, 0.2f ) );
		auto building		= geom::Cube().size( buildingSize )
			>> geom::Translate( vec3( buildingPos.x, noise( vec2( buildingPos.x, buildingPos.z ) ) + buildingSize.y * 0.5f - 0.05f, buildingPos.z ) );
		buildings &= building;
	}
	
	
	mScene.push_back( gl::Batch::create( buildings, gl::getStockShader( gl::ShaderDef().lambert() ) ) );
	mScene.push_back( gl::Batch::create( geom::Sphere(), gl::getStockShader( gl::ShaderDef().lambert() ) ) );
}

void TestProjectApp::update()
{
	ui::Value( "FPS", (int) getAverageFps() );
	
	bool isAlwaysOnTop = getWindow()->isAlwaysOnTop();
	if( ui::Checkbox( "Always on top", &isAlwaysOnTop ) ) {
		getWindow()->setAlwaysOnTop( isAlwaysOnTop );
	}
}
void TestProjectApp::draw()
{
	
	gl::clear( Color::black() );
	
	gl::ScopedMatrices scopedMatrices;
	gl::ScopedDepth	scopedDepth( true );
	gl::setMatrices( mCamera );
	
	for( const auto &sceneObject : mScene ) {
		sceneObject->draw();
	}
}

void TestProjectApp::resize()
{
	mCamera.setAspectRatio( getWindowAspectRatio() );
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

CINDER_RUNTIME_APP( TestProjectApp, RendererGl( RendererGl::Options().msaa( 8 ) ), []( App::Settings *settings ) {
	//settings->setAlwaysOnTop();
}
#ifndef DISABLE_RUNTIME_COMPILATION
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
