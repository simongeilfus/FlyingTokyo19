#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/ObjLoader.h"
#include "cinder/Rand.h"

#include "Simplex.h"

#include "RuntimeUtils.h"

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
	static ParticleRef create();
	static ParticleRef create( const ci::vec2 &position );
	
	Particle(){}
	Particle( const ci::vec2 &position );
	
	void draw();
	void update();
	void push( const ci::vec2 &force );
	
	float getSize() const { return mSize; }
	ci::vec2 getPosition() const { return mPosition; }
	ci::vec2 getVelocity() const { return mVelocity; }
	
	void setSize( float size ) { mSize = size; }
	void setPosition( const ci::vec2 &position ) { mPosition = position; }
	void setVelocity( const ci::vec2 &velocity ) { mVelocity = velocity; }
	
protected:
	float		mSize;
	ci::vec2	mPosition;
	ci::vec2	mVelocity;
};
// Particle Implementation
ParticleRef Particle::create()
{
	return make_shared<Particle>();
}
ParticleRef Particle::create( const vec2 &position )
{
	return make_shared<Particle>( position );
}
Particle::Particle( const vec2 &position )
: mPosition( position ), mVelocity( 0.0f ), mSize( randFloat( 5.0f, 25.0f ) )
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


// ParticleRenderer
typedef std::shared_ptr<class ParticlesRenderer> ParticlesRendererRef;

class ParticlesRenderer {
public:
	static ParticlesRendererRef create( size_t maxParticles );
	ParticlesRenderer( size_t maxParticles );
	
	void render( const std::vector<ParticleRef> &particles );
	
protected:
	gl::BatchRef mBatch;
};

ParticlesRendererRef ParticlesRenderer::create( size_t maxParticles )
{
	return make_shared<ParticlesRenderer>( maxParticles );
}

ParticlesRenderer::ParticlesRenderer( size_t maxParticles )
{
	// Create an empty VBO
	auto positionsLayout = geom::BufferLayout();
	positionsLayout.append( geom::POSITION, 3, 0, 0 );
	auto positionsVbo = gl::Vbo::create( GL_ARRAY_BUFFER, maxParticles * sizeof( vec3 ), nullptr, GL_STREAM_DRAW );
	auto vboMesh	= gl::VboMesh::create( maxParticles, GL_POINTS, { { positionsLayout, positionsVbo } } );
	
	// Create a shader
	auto glslProg	= gl::GlslProg::create( gl::GlslProg::Format().vertex( loadAsset( "shader.vert" ) ).fragment( loadAsset( "shader.frag" ) ) );
	// Create a Batch with the VboMesh and the Shader
	mBatch = gl::Batch::create( vboMesh, glslProg );
}

void ParticlesRenderer::render( const std::vector<ParticleRef> &particles )
{
	// Update the VboMesh
	auto vboMesh = mBatch->getVboMesh();
	auto mappedPosition = vboMesh->mapAttrib3f( geom::POSITION, true );
	for( const auto &particle : particles ) {
		*mappedPosition = vec3( particle->getPosition(), particle->getSize() );
		++mappedPosition;
	}
	mappedPosition.unmap();
	
	// Render the batch
	mBatch->draw( 0, particles.size() );
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
	
	ParticlesRendererRef mParticlesRenderer;
	vector<ParticleRef> mParticles;
};



void TestProjectApp::setup()
{	
	// initialize ui
	ui::initialize();
	
	// Create particles
	size_t numParticles = 10000;
	mParticlesRenderer = ParticlesRenderer::create( numParticles );
	for( int i = 0; i < numParticles; ++i ) {
		vec2 pos = vec2( randFloat( 0, getWindowWidth() ), randFloat( 0, getWindowHeight() ) );
		mParticles.push_back( Particle::create( pos ) );
	}
	
	gl::enable( GL_PROGRAM_POINT_SIZE );
}

void TestProjectApp::update()
{
	ui::Value( "FPS", (int) getAverageFps() );
	
	for( const auto &particle : mParticles ) {
		particle->update();
	}
}
void TestProjectApp::draw()
{
	gl::clear();
	
	mParticlesRenderer->render( mParticles );
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

//CINDER_APP( TestProjectApp, RendererGl( RendererGl::Options().msaa( 8 ) ) )
CINDER_RUNTIME_APP( TestProjectApp, RendererGl, []( App::Settings *settings ) {
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
