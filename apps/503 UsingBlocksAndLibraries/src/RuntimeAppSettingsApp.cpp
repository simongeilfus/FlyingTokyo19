#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "CinderImGui.h"
#include "Simplex.h"
#include "runtime_app.h"

using namespace ci;
using namespace ci::app;
using namespace std;

struct Circle {
	vec2 position;
	float size;
};

class RuntimeAppSettingsApp : public App {
  public:
	void setup() override;
	void draw() override;
	
	vector<Circle> mCircles;
};

void RuntimeAppSettingsApp::setup()
{
	ui::initialize();
	
	for( int i = 0; i < getWindowSize().x; i += 20 ) {
		for( int j = 0; j < getWindowSize().y; j += 20 ) {
			mCircles.push_back( { vec2( 10 ) + vec2( i, j ), 10.0f } );
		}
	}
}

void RuntimeAppSettingsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	
	ui::ScopedWindow window( "RuntimeApp" );
	ui::Text( "Hello!" );
	ui::Value( "Fps", (int) getAverageFps() );
	
	for( auto &circle : mCircles ) {
		gl::drawSolidCircle( circle.position, circle.size * ( Simplex::noise( vec3( circle.position * 0.1f, getElapsedSeconds() * 0.75f ) ) * 0.5f + 0.5f ), 64 );
	}
}

CINDER_RUNTIME_APP( RuntimeAppSettingsApp, RendererGl( RendererGl::Options().msaa(8) ), App::SettingsFn()
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
