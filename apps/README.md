# FlyingTokyo19

####1. Short introduction to Cinder

One of the first thing

#####1.1 [From openFrameworks to Cinder.](/101 oFAppStructure/src)
If we were to compare oF and Cinder in terms of app structure, one of the main difference we could note is in the way the source file(s) are organised.  

Openframeworks use the really common `main.cpp` file to write the entry of the application and next to it usually splits the main application source into a `ofApp.h` header and a `ofApp.cpp` implementation file.  

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
  
You can see a [here cinder app organised using oF approach to structuring the source code](/101 oFAppStructure/). Both are extremely similar when organising the code that way :

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
  
It's not a big deal, and doesn't change much for the user but the first obvious thing is how the `main` function is implemented. Where oF made it much simpler (and avoiding unecessary questions like "why does the main function returns an int or has those weird arguments") Cinder is sticking to the standard way of writing a `main` function.
  
Here's what Bjarne Stroustrups and the ISOCPP say about this:  
http://www.stroustrup.com/bs_faq2.html#void-main  
https://isocpp.org/wiki/faq/newbie#main-returns-int  

**All that said we don't really have to care about this as both libraries ship with a project generator that takes care of generating this structure for us.**  

#####1.2 [Cinder App Structure.](/102 CinderAppStructure/)
Cinder has an even simpler approach to structuring the source files and the app structure as all the above is usually merged into one single file. IMO this allows faster prototyping as you don't need to go back and forth between files to write a simple application.  

At some point when the application grow bigger I sometimes split it into a header and implementation file, but I'm usually happy with the base structure. It also makes sharing snippet and small test cases much easier as they can live in a single gist page.  
  
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

#####1.3 [App constructor, destructor and cleanup method.](/103 AppConstructor/)
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

#####1.4 [Events.](/104 AppEvents/)
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

#####1.5 [App Settings.](/105 AppSettings/)
#####1.6 [Extra flexibility with signals](/101 oFAppStructure/)
#####1.7 [Multiple Windows](/101 oFAppStructure/)

___
####2. Cinder App Structure