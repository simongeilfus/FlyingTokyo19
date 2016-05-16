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
	
	// COPY THIS
	gl::GlslProgRef		mGlslProg;
	// !
	
	
	CameraPersp			mCamera;
	CameraUi			mCameraUi;
};

void TestProjectApp::setup()
{
	getWindow()->setAlwaysOnTop();
	
	// Initialize the Camera and its UI
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 50.0f, 0.1f, 1000.0f );
	mCamera.lookAt( vec3(5,2,0), vec3(0) );
	mCameraUi = CameraUi( &mCamera, getWindow(), -1 );
	
	// initialize ui
	ui::initialize();
	
	// COPY THIS!
	// watch and create shader
	wd::unwatchAll();
	wd::watch( "fog.*", [this]( const fs::path &path ) {
		try {
			mGlslProg = gl::GlslProg::create( gl::GlslProg::Format().vertex( loadAsset( "fog.vert" ) ).fragment( loadAsset( "fog.frag" ) ) );
		}
		catch( gl::GlslProgExc exc ) {
			console() << exc.what() << endl;
		}
	} );
	// !
	
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
	
	
	//mScene.push_back( gl::Batch::create( buildings, gl::getStockShader( gl::ShaderDef().lambert() ) ) );
	//mScene.push_back( gl::Batch::create( geom::Sphere(), gl::getStockShader( gl::ShaderDef().lambert() ) ) );
	
	// COPY THIS!
	// Add the skybox
	mScene.push_back( gl::Batch::create( geom::Sphere().radius( 100.0f ), gl::getStockShader( gl::ShaderDef().lambert() ) ) );
	//!
	
	// Add trees
	geom::SourceMods tree;
	for( int i = 0; i < 500; ++i ) {
		auto treePos	= vec3( randFloat( -5.0f, 5.0f ), 0.0f, randFloat( -5.0f, 5.0f ) );
		treePos.y		= noise( vec2( treePos.x, treePos.z ) );
		auto treeRand	= randFloat();
		auto treeScale	= 0.1f;
		auto treeHeight = randFloat( 1, 4 ) * treeScale;
		tree &= geom::Sphere().subdivisions( 1 ).radius( 1.5f * treeScale )
		>> geom::AttribFn<vec3, vec3>( geom::POSITION, geom::POSITION, [this,treeScale,treeRand,treeHeight]( vec3 v ) {
			auto n = v * 4.5f + vec3( treeRand );
			return v + mPerlin.dnoise( n.x, n.y, n.z ) * 0.25f * treeScale * vec3( 2.0f, 1.0f, 2.0f );
		} )
		>> geom::Translate( vec3( treePos.x, treePos.y + treeHeight * 0.25f + treeHeight, treePos.z ) );
		
		tree &= geom::Cone().height( treeHeight ).base( 0.3f * treeScale ).subdivisionsHeight( 1 ).subdivisionsAxis( 5 ).apex( 0.2f * treeScale )
		>> geom::Transform( glm::translate( vec3( treePos.x, treePos.y, treePos.z ) ) * glm::rotate( randFloat( 0.0f, 0.2f ), vec3( 1, 0, 0 ) ) );
	}
	mScene.push_back( gl::Batch::create( tree, gl::getStockShader( gl::ShaderDef().lambert() ) ) );
}

void TestProjectApp::update()
{
	ui::Value( "FPS", (int) getAverageFps() );
	
	bool isAlwaysOnTop = getWindow()->isAlwaysOnTop();
	if( ui::Checkbox( "Always on top", &isAlwaysOnTop ) ) {
		getWindow()->setAlwaysOnTop( isAlwaysOnTop );
	}
	
	// COPY THIS!
	// update shader settings
	{
		ui::ScopedWindow window( "Shading" );
		static vec3        uFogColor = vec3( 0.25, 0.29, 0.47 );
		static float       uFogDensity = 10.0f;
		static vec3        uSunColor = vec3( 1.0f, 0.77, 0.60 );
		static vec3        uSunDirection = vec3( 0, 0.3f, -1.0f );
		static float       uSunDispertion = 0.2f;
		static float       uSunIntensity = 0.16f;
		static vec3        uInscatteringCoeffs = vec3( 0.0f );
		
		ui::ColorEdit3( "Fog Color", &uFogColor[0] );
		ui::ColorEdit3( "Sun Color", &uSunColor[0] );
		ui::ColorEdit3( "Inscattering", &uInscatteringCoeffs[0] );
		ui::DragFloat( "FogDensity", &uFogDensity, 0.1f, 0.0f, 100.0f );
		ui::DragFloat( "SunDispertion", &uSunDispertion, 0.01f, 0.00001f, 10.0f );
		ui::DragFloat( "SunIntensity", &uSunIntensity, 0.01f, 0.000001f, 2.0f );
		
		mGlslProg->uniform( "uFogColor", uFogColor );
		mGlslProg->uniform( "uFogDensity", uFogDensity );
		mGlslProg->uniform( "uSunColor", uSunColor );
		mGlslProg->uniform( "uSunDirection", uSunDirection );
		mGlslProg->uniform( "uSunDispertion", uSunDispertion );
		mGlslProg->uniform( "uSunIntensity", uSunIntensity );
		mGlslProg->uniform( "uInscatteringCoeffs", uInscatteringCoeffs );
	}
	//!
}
void TestProjectApp::draw()
{
	
	gl::clear( Color::black() );
	
	gl::ScopedMatrices scopedMatrices;
	gl::ScopedDepth	scopedDepth( true );
	gl::setMatrices( mCamera );
	
	for( const auto &sceneObject : mScene ) {
		// COPY THIS!
		if( mGlslProg && sceneObject->getGlslProg() != mGlslProg ) {
			sceneObject->replaceGlslProg( mGlslProg );
		}
		//!
		
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
