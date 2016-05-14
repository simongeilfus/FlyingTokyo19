# FlyingTokyo19

1. Short introduction to Cinder
  1. [From openFrameworks to Cinder.](#11-from-openframeworks-to-cinder)
  2. [Cinder App Structure.](#12-cinder-app-structure)
  3. [App constructor, destructor and cleanup method.](#13-app-constructor-destructor-and-cleanup-method)
  4. [App Settings.](#14-app-settings)
  5. [Events.](#15-events)
  6. [Extra flexibility with signals](#16-extra-flexibility-with-signals)
  7. [Multiple Windows](#17-multiple-windows)

___

###1. Short introduction to Cinder

One of the first thing

#####1.1. [From openFrameworks to Cinder.](101 oFAppStructure/src)
If we were to compare oF and Cinder in terms of app structure, one of the main difference we could note is the way the source file(s) are organised.  

Openframeworks use the really common approach of having a `main.cpp` file to write the entry of the application and next to it a `ofApp.h` header and a `ofApp.cpp` implementation file for the main application source code.  

`main.cpp`
```c++
#include "ofApp.h"
int main()
{
    ofSetupOpenGL( 1024,768, OF_WINDOW );
    ofRunApp( new ofApp() );
}
```

`ofApp.h`
```c++
#pragma once

#include "ofMain.h"
class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
};
```
`ofApp.cpp`
```c++
#include "ofApp.h"
void ofApp::setup()
{
}
void ofApp::update()
{
}
void ofApp::draw()
{
}
```

If you forget about cosmetics and don't get too much into details both Cinder and oF work the same in terms of general app structure. They both have a main entry point and a `App` class from which all apps inherit.  
  
You can see a [here cinder app organised using oF approach to structuring the source code](101 oFAppStructure/src). Both are extremely similar when organising the code that way :

`main.cpp`
```c++
#include "cinder/app/RendererGl.h"
#include "ofApp.h"
int main( int argc, char* argv[] )
{
	ci::app::RendererRef renderer( new ci::app::RendererGl() );
	ci::app::AppMac::main<ofApp>( renderer, "ofApp", argc, argv );
	return 0;
}

```
`ofApp.h`
```c++
#pragma once
#include "cinder/app/App.h"
class ofApp : public ci::app::App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
	void mouseDown( ci::app::MouseEvent event ) override;
};
```
`ofApp.cpp`
```c++
#include "ofApp.h"
void ofApp::setup()
{
}
void ofApp::update()
{
}
void ofApp::draw()
{
}
```

Even if the two versions are really similar in a way, there's one striking difference and it is definitely more of a design choice than a value difference. openFrameworks seems to be designed to be simple and easy to read for non-programmers while Cinder is written using a really standard and modern form of C++.  
  
It's not a big deal, and doesn't change much for the user but another obvious thing is how the `main` function is implemented. Where oF made it much simpler (and probably avoiding unecessary questions like "why does the main function returns an int or why does it have those weird arguments") Cinder is sticking to the standard way of writing a `main` function. Not a big deal but much more standard compliant.  
  
Here's what Bjarne Stroustrups and the ISOCPP say about this; Stroustrups is actually much more strict about it:
> "The definition `void main() { /* ... */ }` is not and never has been C++, nor has it even been C."  

http://www.stroustrup.com/bs_faq2.html#void-main  
https://isocpp.org/wiki/faq/newbie#main-returns-int  

**All that said I would say that we don't really have to care about this as both libraries ship with a handy project generator that takes care of generating this structure for us.**
___

#####1.2. [Cinder App Structure.](102 CinderAppStructure/src/CinderAppStructure.cpp)
Cinder has a really simple approach to structuring the source files and the app structure. The code mentionned above is usually merged into one single file. IMO this allows faster prototyping as you don't need to go back and forth between files to write a simple application.  

It keeps things simple and centralized and makes sharing code snippets and small test cases much easier as they can live in a single gist page (for example most snippet here can be copy and pasted in any project and will work straight away).  

At some point when the application grows bigger I sometimes split it into a header and implementation file, but I'm usually happy with the base structure.
  
The `main` function is wrapped into a handy `CINDER_APP` macro that expands to the proper version depending on the platform the app is built on. This reduces the code above to this short one for the whole app:  
```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
};

void CinderApp::setup()
{
}
void CinderApp::update()
{
}
void CinderApp::draw()
{
}

CINDER_APP( CinderApp, RendererGl )
```
___

#####1.3. [App constructor, destructor and cleanup method.](103 AppConstructor/src/AppConstructorApp.cpp)
Cinder is quite flexible in terms of how you structure your app and how the different component of the app get initialized and cleaned up. Not overriding the `setup` function and using a constructor instead is totally fine:  
```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AppConstructorApp : public App {
public:
	AppConstructorApp();
	string mSomeMember;
};

AppConstructorApp::AppConstructorApp()
: mSomeMember( "World" ) // the member initializer list is a convenient place to initialize your variables
{
	// This is the first place where code get executed
	// during the app construction.
}
CINDER_APP( AppConstructorApp, RendererGl )
```

You can also use a destructor the same way or override the `cleanup` method if you need to release resources before the app destruction: 
```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AppConstructorApp : public App {
public:
	AppConstructorApp();
	~AppConstructorApp();
	void cleanup() override;
	string mSomeMember;
};

AppConstructorApp::AppConstructorApp()
: mSomeMember( "World" ) // the member initializer list is a convenient place to initialize your variables
{
	// This is the first place where code get executed
	// during the app construction.
}
AppConstructorApp::~AppConstructorApp()
{
	// This is the last place where code get executed
	// during the app destruction
}
void AppConstructorApp::cleanup()
{
	// This is a safe place to release and cleanup
	// things before the actual destruction of the app
}
CINDER_APP( AppConstructorApp, RendererGl )
```
___

#####1.4. [App Settings.](104 AppSettings/src/AppSettingsApp.cpp)
Cinder also provides a series of functions to setup the app the way you want. Change the window position, set it fullscreen, add a window title, etc... It is fine to set this up in the app constructor or setup method:   
```c++
void CinderApp::setup()
{
	setWindowPos( ivec2( 20, 300 ) );
	setWindowSize( ivec2( 200, 20 ) );
}
```
But most of the time you want to access more specific settings, want those settings to be set before the app actually starts (and avoid any unwantend flickering) or simply want to do it the proper/cleaner way. The `CINDER_APP` macro accepts a third argument that is the prepareSettings function. It can be a free-standing/static function or a lambda.  
```c++
CINDER_APP( AppSettings, RendererGl, []( App::Settings *settings ) {
	settings->setWindowSize( ivec2( 200, 20 ) );
	settings->setTitle( "App Settings" );
	settings->setResizable( false );
})
```

This is a bit less compact and elegant but it does exactly the same but with a static function: 
```c++
void mySettings( App::Settings *settings )
{
	settings->setWindowSize( ivec2( 200, 60 ) );
	settings->setTitle( "App Settings" );
	settings->setResizable( false );
}
CINDER_APP( AppSettings, RendererGl, mySettings )
```

The settings function could also be used for more specific/advanced settings. For example you could decide to start the app on the secondary display if more than one display is detected:  
```c++
CINDER_APP( AppSettings, RendererGl, []( App::Settings *settings ) {
	auto displays = Display::getDisplays();
	settings->setDisplay( displays.size() > 1 ? displays[1] : displays[0] );
})
```

The `CINDER_APP` macro also provides a way to specify options for the **OpenGL Renderer**. Such as the desired version, the amount of antialiasing or the presence of a stencil buffer for example :  
```c++
// this will create a renderer with a multisample anti aliasing of 16 and a stencil buffer
CINDER_APP( AppSettings, RendererGl( RendererGl::Options().msaa( 16 ).stencil() )
```  
#####1.5. [Events.](105 AppEvents/src/AppEventsApp.cpp)
Cinder's [AppBase class](https://github.com/cinder/Cinder/blob/master/include/cinder/app/AppBase.h) provides a series of method you can override to receive the base events of your app and the app's window events. It is the easiest way of subscribing to most events in your app :
```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AppEventsApp : public App {
public:
	//! Performs any application setup after the Renderer has been initialized.
	void setup() override {}
	//! Performs any once-per-loop computation.
	void update() override {}
	//! Performs any rendering once-per-loop or in response to OS-prompted requests for refreshes.
	void draw() override { gl::clear(); }
	
	//! Receives mouse-down events.
	void mouseDown( MouseEvent event ) override {}
	//! Receives mouse-up events.
	void mouseUp( MouseEvent event ) override {}
	//! Receives mouse-wheel events.
	void mouseWheel( MouseEvent event ) override {}
	//! Receives mouse-move events.
	void mouseMove( MouseEvent event ) override {}
	//! Receives mouse-drag events.
	void mouseDrag( MouseEvent event ) override {}
	
	//! Responds to the beginning of a multitouch sequence
	void touchesBegan( TouchEvent event ) override {}
	//! Responds to movement (drags) during a multitouch sequence
	void touchesMoved( TouchEvent event ) override {}
	//! Responds to the end of a multitouch sequence
	void touchesEnded( TouchEvent event ) override {}
	
	//! Receives key-down events.
	void keyDown( KeyEvent event ) override {}
	//! Receives key-up events.
	void keyUp( KeyEvent event ) override {}
	//! Receives window resize events.
	void resize() override {}
	//! Receives file-drop events.
	void fileDrop( FileDropEvent event ) override {}
	
	//! Cleanups any resources before app destruction
	void cleanup() override {}
};

CINDER_APP( AppEventsApp, RendererGl )
```
___

#####1.6. [Extra flexibility with signals](106 FlexibilityWithSignals/src/FlexibilityWithSignalsApp.cpp)
Cinder offers another level of flexibility in how you deal with the app events thanks to its use of "signals". IMO Cinder's signal implementation is based on the best available out there.  
It allows you to structure things exactly the way you want : 
```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FlexibilityWithSignals : public App {
public:
	FlexibilityWithSignals()
	{
		// Connect to the update signal to output
		// the FPS to the window title.
		getSignalUpdate().connect( [this](){
			getWindow()->setTitle( to_string( (int) getAverageFps() ) + " fps" );
		} );
	}
	
};
CINDER_APP( FlexibilityWithSignals, RendererGl )
```

And more importantly allows to give other classes the ability to listen to specific events. Which clearly simplify their use and makes the user code shorter and nicer.   
```c++
// For example, giving a reference to the window to a CameraUI object 
// will let the constructor connect with any signal it would need to 
// take care of the UI. Saving you to write mCameraUi->mouseDown, mCameraUi->mouseDrag, etc ...
mCameraUi = CameraUi( &mCamera, getWindow() );
```

We could from anywhere in the app get a reference to the App or to the Window and use any of its signals. Here's a very short example of a simple button class :
```c++
class Button {
public:
	Button( const string &name, const Rectf &rect );
	void draw();
protected:
	string mName;
	Rectf mBounds;	
};

Button::Button( const string &name, const Rectf &rect )
: mName( name ), mBounds( rect )
{
	// subscribe to mouse down event of the app window
	app::getWindow()->getSignalMouseDown().connect( [this]( app::MouseEvent event ) {
		// Check if the mouse is inside the bounds of this button
		if( mBounds.contains( event.getPos() ) ) {
			app::console() << "Button " << mName << " clicked!" << std::endl;		
		}		
	});
}
void Button::draw()
{
	gl::drawSolidRect( mBounds );
	gl::drawStringCentered( mName, mBounds.getCenter() );
}

```
___
#####1.7. [Multiple Windows](107 MultipleWindow/src/MultipleWindowApp.cpp)
Adding more than one window to your app works the same way. You can use the `createWindow` shortcut from anywhere in your code, or do it in the prepareSettings function with `App::Settings::prepareWindow` :

```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MultipleWindowApp : public App {
public:
	void mouseDown( MouseEvent event ) override;
	void draw() override { gl::clear( Color( 0, 0, 0 ) ); }
};

void MultipleWindowApp::mouseDown( MouseEvent event )
{
	// create a new window next to the one that
	// received a mouseDown event
	auto windowPos = getWindowPos();
	createWindow( Window::Format().pos( windowPos + ivec2( 256, 0 ) ).size( ivec2( 256 ) ) );
}

CINDER_APP( MultipleWindowApp, RendererGl, []( App::Settings *settings ) {
	// prepare the app with 3 small windows
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 40 ) ).title( "Window 0" ) );
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 320 ) ).title( "Window 1" ) );
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 576 ) ).title( "Window 2" ) );
})
```

The use of `signals` become much more obvious in a multi-window situation. You could probably keep the usual `draw` method and test which window the method is currently drawing; like so : 
```c++
void CinderApp::draw()
{
	if( getWindow() == getWindowIndex( 0 ) ) {
		// draw the first window
	}
	else if( getWindow() == getWindowIndex( 1 ) ) {
		// draw the second window
	}
}
```

But `signals` are made to make that situation easier and more elegant, and it is definitely much cleaner to write it like this :
```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MultipleWindowApp : public App {
public:
	MultipleWindowApp();
	void drawWindow0() { gl::clear( Color::black() ); }
	void drawWindow1() { gl::clear( Color::white() ); }
};
MultipleWindowApp::MultipleWindowApp()
{
	// Connect the windows to their respective signals
	getWindowIndex( 0 )->getSignalDraw().connect( bind( &MultipleWindowApp::drawWindow0, this ) );
	getWindowIndex( 1 )->getSignalDraw().connect( bind( &MultipleWindowApp::drawWindow1, this ) );
}

CINDER_APP( MultipleWindowApp, RendererGl, []( App::Settings *settings ) {
	// prepare the app with 2 small windows
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 40 ) ).title( "Window 0" ) );
	settings->prepareWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 320 ) ).title( "Window 1" ) );
})
```

___
#####1.8. Object Oriented Design   
Another big difference worth mentioning between Cinder and other libraries such as Processing or openFrameworks is the Object Oriented approach in the design of the library. As we've seen previously seen Cinder is indeed providing a long list of classes that are used everywhere in the library.  

So when Processing's `background` or openFrameworks `ofBackground` methods accepts a series of`floats` describing the different channels of the clear color; Cinder's equivalent `gl::clear` accepts a `Color` or `ColorA` objects.  

The same can be said of Processing `rect( x, y, w, h )` and openFrameworks `ofDrawRectangle( x, y, w, h )` where Cinder version would look like `gl::drawSolidRect( Rectf( x1, y1, x2, y2 ) )`. (I think openFrameworks's `ofDrawRectangle(const ofRectangle &r)` has only been added in recent versions).  

This approach becomes obvious when you look at Cinder's graphic API and even more when looking at its OpenGL abstraction layer.  


___
###2. C++11
C++11 (and C++14) has brought a great number of very nice new features to the standard and Cinder makes extensive use of of them.  

#####2.1. [Auto keyword and type inference.](/)
C++ is a strongly typed language meaning that, unlike dynamic languages like JavaScript, you have to give a type to the variable you create (hence the abscence of a `var` keywords like in JS). The introduction of type inference slighly changes that while retaining the safety of a strongly typed language.  

Type inference is the ability of the compiler to automatically deduce the types of your variables.  
```c++
auto someFloat 			= 123.456f;
auto anEmptyRectangle	= Rectf();
```
It sometimes makes the code more readable and in other case saves you from writting very long types. Let's say that we have a map of texture format that we want to iterate:    
```c++
std::map<string,gl::Texture2d::Format>::iterator it = mTextures.begin();
```
C++11 allows to write the same like this:  
```c++
auto it = mTextures.begin();
```

___
###3. Graphics

Cinder now has excellent documentation and a few great guide to get you started. [The OpenGL guide](https://libcinder.org/docs/guides/opengl/) is definitely the place to start if you need some introduction to OpenGL.

Since Cinder 0.9 the default version of OpenGL is 3.2 Core Profile on desktop and OpenGL ES 2.0 and 3.0 on mobile. Recent versions of OpenGL have changed drastically while more or less abandoning the [fixed function pipeline](https://www.opengl.org/wiki/Fixed_Function_Pipeline) and the old immediate-mode rendering.

Hopefully Cinder provides a great API and a set of tools to help us understand those changes without having to write a single line of OpenGL.

#####3.1. [Helpers functions and the gl::namespace.](/)

```c++
//! Draws the VboMesh \a mesh. Consider a gl::Batch as a faster alternative. Optionally specify a \a first vertex index and a \a count of vertices.
void draw( const VboMeshRef &mesh, GLint first = 0, GLsizei count = -1 );
//! Draws a Texture2d \a texture, fitting it to \a dstRect. Ignores currently bound shader.
void draw( const Texture2dRef &texture, const Rectf &dstRect );
//! Draws a subregion \a srcArea of a Texture (expressed as upper-left origin pixels).
void draw( const Texture2dRef &texture, const Area &srcArea, const Rectf &dstRect );
void draw( const Texture2dRef &texture, const vec2 &dstOffset = vec2() );
void draw( const PolyLine2 &polyLine );
void draw( const PolyLine3 &polyLine );
//! Draws a Path2d \a pathd using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc
void draw( const Path2d &path, float approximationScale = 1.0f );
//! Draws a Shaped2d \a shaped using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc
void draw( const Shape2d &shape, float approximationScale = 1.0f );
//! Draws a TriMesh \a mesh at the origin. Currently only uses position and index information.
void draw( const TriMesh &mesh );
//! Draws a geom::Source \a source at the origin.
void draw( const geom::Source &source );

//! Draws a CubeMapTex \a texture inside \a rect with an equirectangular projection. If \a lod is non-default then a specific mip-level is drawn. Typical aspect ratio should be 2:1.
void drawEquirectangular( const gl::TextureCubeMapRef &texture, const Rectf &r, float lod = -1 );
//! Draws a CubeMapTex \a texture as a horizontal cross, fit inside \a rect. If \a lod is non-default then a specific mip-level is drawn. Typical aspect ratio should be 4:3.
void drawHorizontalCross( const gl::TextureCubeMapRef &texture, const Rectf &rect, float lod = -1 );
//! Draws a CubeMapTex \a texture as a vertical cross, fit inside \a rect. If \a lod is non-default then a specific mip-level is drawn. Typical aspect ratio should be 3:4.
void drawVerticalCross( const gl::TextureCubeMapRef &texture, const Rectf &rect, float lod = -1 );

//! Draws a solid (filled) Path2d \a path using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc. Performance warning: This routine tesselates the polygon into triangles. Consider using Triangulator directly.
void drawSolid( const Path2d &path2d, float approximationScale = 1.0f );
//! Draws a solid (filled) Shape2d \a shape using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc. Performance warning: This routine tesselates the polygon into triangles. Consider using Triangulator directly.
void drawSolid( const Shape2d &shape, float approximationScale = 1.0f );
void drawSolid( const PolyLine2 &polyLine );

//! Renders a solid cube centered at \a center of size \a size. Normals and created texture coordinates are generated.
void drawCube( const vec3 &center, const vec3 &size );
//! Renders a solid cube centered at \a center of size \a size. Each face is assigned a unique color.
void drawColorCube( const vec3 &center, const vec3 &size );
//! Renders a stroked cube centered at \a center of size \a size.
void drawStrokedCube( const vec3 &center, const vec3 &size );
//! Renders a stroked cube using \a box as the guide for center and size.
inline void drawStrokedCube( const ci::AxisAlignedBox &box ) { drawStrokedCube( box.getCenter(), box.getSize() ); }
//! Renders a solid \a sphere, subdivided on both longitude and latitude into \a subdivisions.
void drawSphere( const Sphere &sphere, int subdivisions = -1 );
//! Renders a solid sphere at \a center of radius \a radius, subdivided on both longitude and latitude into \a subdivisions.
void drawSphere( const vec3 &center, float radius, int subdivisions = -1 );
//! Draws a textured quad of size \a scale that is aligned with the vectors \a bbRight and \a bbUp at \a pos, rotated by \a rotationRadians around the vector orthogonal to \a bbRight and \a bbUp.
void drawBillboard( const vec3 &pos, const vec2 &scale, float rotationRadians, const vec3 &bbRight, const vec3 &bbUp, const Rectf &texCoords = Rectf( 0, 0, 1, 1 ) );
//! Renders a stroked representation of \a cam
void drawFrustum( const Camera &cam );
void drawCoordinateFrame( float axisLength = 1.0f, float headLength = 0.2f, float headRadius = 0.05f );
//! Draws a vector starting at \a start and ending at \a end. An arrowhead is drawn at the end of radius \a headRadius and length \a headLength.
void drawVector( const vec3 &start, const vec3 &end, float headLength = 0.2f, float headRadius = 0.05f );

//! Draws a line between points a and b
void drawLine( const vec3 &a, const vec3 &b );
void drawLine( const vec2 &a, const vec2 &b );

//! Draws \a texture on the XY-plane
void drawSolidRect( const Rectf &r, const vec2 &upperLeftTexCoord = vec2( 0, 1 ), const vec2 &lowerRightTexCoord = vec2( 1, 0 ) );
//! Draws a solid rounded rectangle centered around \a rect, with a corner radius of \a cornerRadius
void drawSolidRoundedRect( const Rectf &r, float cornerRadius, int numSegmentsPerCorner = 0,  const vec2 &upperLeftTexCoord = vec2( 0, 1 ), const vec2 &lowerRightTexCoord = vec2( 1, 0 ) );
//! Draws a filled circle centered around \a center with a radius of \a radius. Default \a numSegments requests a conservative (high-quality but slow) number based on radius.
void drawSolidCircle( const vec2 &center, float radius, int numSegments = -1 );
//! Draws a filled ellipse centered around \a center with an X-axis radius of \a radiusX and a Y-axis radius of \a radiusY. Default \a numSegments requests a conservative (high-quality but slow) number based on radius.
void drawSolidEllipse( const vec2 &center, float radiusX, float radiusY, int numSegments = -1 );

//! Draws a stroked rectangle with dimensions \a rect.
void drawStrokedRect( const Rectf &rect );
//! Draws a stroked rectangle centered around \a rect, with a line width of \a lineWidth
void drawStrokedRect( const Rectf &rect, float lineWidth );
//! Draws a stroked rounded rectangle centered around \a rect, with a corner radius of \a cornerRadius
void drawStrokedRoundedRect( const Rectf &rect, float cornerRadius, int numSegmentsPerCorner = 0 );
//! Draws a stroked circle centered around \a center with a radius of \a radius. Default \a numSegments requests a conservative (high-quality but slow) number based on radius.
void drawStrokedCircle( const vec2 &center, float radius, int numSegments = -1 );
//! Draws a stroked circle centered around \a center with a radius of \a radius and a line width of \a lineWidth. Default \a numSegments requests a conservative (high-quality but slow) number based on radius.
void drawStrokedCircle( const vec2 &center, float radius, float lineWidth, int numSegments = -1 );
//! Draws a stroked ellipse centered around \a center with an X-axis radius of \a radiusX and a Y-axis radius of \a radiusY.  Default \a numSegments requests a conservative (high-quality but slow) number based on radius.
void drawStrokedEllipse( const vec2 &center, float radiusX, float radiusY, int numSegments = -1 );

//! Draws a string \a str with its lower left corner located at \a pos. Optional \a font and \a color affect the style.
void drawString( const std::string &str, const vec2 &pos, const ColorA &color = ColorA( 1, 1, 1, 1 ), Font font = Font() );
//! Draws a string \a str with the horizontal center of its baseline located at \a pos. Optional \a font and \a color affect the style
void drawStringCentered( const std::string &str, const vec2 &pos, const ColorA &color = ColorA( 1, 1, 1, 1 ), Font font = Font() );
//! Draws a right-justified string \a str with the center of its  located at \a pos. Optional \a font and \a color affect the style
void drawStringRight( const std::string &str, const vec2 &pos, const ColorA &color = ColorA( 1, 1, 1, 1 ), Font font = Font() );

//! Renders a solid triangle.
void drawSolidTriangle( const vec2 &pt0, const vec2 &pt1, const vec2 &pt2 );
//! Renders a textured triangle.
void drawSolidTriangle( const vec2 &pt0, const vec2 &pt1, const vec2 &pt2, const vec2 &texPt0, const vec2 &texPt1, const vec2 &texPt2 );
//! Renders a textured triangle.
void drawSolidTriangle( const vec2 pts[3], const vec2 texCoord[3] = nullptr );
```