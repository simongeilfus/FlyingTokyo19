# FlyingTokyo19

####1. Short introduction to Cinder

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
####2. Cinder App Structure