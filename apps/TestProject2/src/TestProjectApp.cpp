#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/ObjLoader.h"
#include "cinder/Rand.h"

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

// Base Particle class

// ParticleRef typedef
typedef std::shared_ptr<class Particle> ParticleRef;

// Particle
class Particle {
public:
	static ParticleRef create( const vec2 &position );
	Particle( const vec2 &position );
	
	void draw();
	void update();
	void push( const ci::vec2 &force );
	
	vec2 getPosition() const { return mPosition; }
	vec2 getVelocity() const { return mVelocity; }
	
	void setPosition( const vec2 &position ) { mPosition = position; }
	void setVelocity( const vec2 &velocity ) { mVelocity = velocity; }
	
protected:
	vec2 mPosition;
	vec2 mVelocity;
};
// Particle Implementation
ParticleRef Particle::create( const vec2 &position )
{
	return make_shared<Particle>( position );
}
Particle::Particle( const vec2 &position )
: mPosition( position ), mVelocity( 0.0f )
{
}
void Particle::draw()
{
	gl::drawSolidCircle( mPosition, 3.0f );
}

void Particle::update()
{
	mPosition += mVelocity;
	// turbulence
	mPosition += Simplex::curlNoise( mPosition * 0.001f ) * 0.5f;
	mVelocity *= 0.98f;
}

void Particle::push( const ci::vec2 &force )
{
	mVelocity += force;
}

class TestProjectApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
	
	void mouseDown( MouseEvent event ) override;
	void mouseUp( MouseEvent event ) override;
	void mouseWheel( MouseEvent event ) override;
	void mouseMove( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void keyDown( KeyEvent event ) override;
	void keyUp( KeyEvent event ) override;
	void resize() override;
	void fileDrop( FileDropEvent event ) override;
	
	void cleanup() override;
	
	virtual void save( cereal::BinaryOutputArchive &ar );
	virtual void load( cereal::BinaryInputArchive &ar );
	
	vector<ParticleRef> mParticles;
};



void TestProjectApp::setup()
{
	// initialize ui
	ui::initialize();
	
	// Create particles
	for( int i = 0; i < 1000; ++i ) {
		vec2 pos = vec2( randFloat( 0, getWindowWidth() ), randFloat( 0, getWindowHeight() ) );
		mParticles.push_back( Particle::create( pos ) );
	}
}

void TestProjectApp::update()
{
	ui::Value( "FPS", (int) getAverageFps() );
	
	for( auto particle : mParticles ) {
		particle->update();
	}
}
void TestProjectApp::draw()
{
	gl::clear();
	
	for( auto particle : mParticles ) {
		particle->draw();
	}
}

void TestProjectApp::mouseDown( MouseEvent event ) 
{
	for( auto particle : mParticles ) {
		vec2 pos = particle->getPosition();
		vec2 diff = pos - vec2( event.getPos() );
		particle->push( -diff * 0.01f );
	}
}
void TestProjectApp::mouseUp( MouseEvent event ) 
{
}
void TestProjectApp::mouseWheel( MouseEvent event ) 
{
}
void TestProjectApp::mouseMove( MouseEvent event ) 
{
}
void TestProjectApp::mouseDrag( MouseEvent event ) 
{
}

void TestProjectApp::keyDown( KeyEvent event ) 
{
}
void TestProjectApp::keyUp( KeyEvent event ) 
{
}
void TestProjectApp::resize() 
{
}
void TestProjectApp::fileDrop( FileDropEvent event ) 
{
}

void TestProjectApp::cleanup() 
{
}

void TestProjectApp::save( cereal::BinaryOutputArchive &ar )
{
}
void TestProjectApp::load( cereal::BinaryInputArchive &ar )
{
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
