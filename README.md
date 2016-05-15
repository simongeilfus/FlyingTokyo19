# FlyingTokyo 19 : An Introduction to Cinder, Hot-Reloading and Runtime-Compiled C++

First thing first; Please clone this repository, start the install script and go grab yourself a cup of coffee (this is going to clone, build and install Cinder, Llvm, Clang, Cling and other smaller piece of code ... it is going to take a while!) :  
```shell
git clone https://github.com/simongeilfus/FlyingTokyo19.git
cd FlyingTokyo19
sh install.sh
```

[https://www.dropbox.com/s/cm84m5halg54p5p/lib.zip?dl=0](https://www.dropbox.com/s/cm84m5halg54p5p/lib.zip?dl=0)
```
unzip and copy to cinder/blocks/Cinder-Runtime/
```

___

### Table of contents
1. [Short introduction to Cinder](#1-short-introduction-to-cinder)
  1. [From openFrameworks to Cinder.](#11-from-openframeworks-to-cinder)
  2. [Cinder App Structure.](#12-cinder-app-structure)
  3. [App constructor, destructor and cleanup method.](#13-app-constructor-destructor-and-cleanup-method)
  4. [App Settings.](#14-app-settings)
  5. [Events.](#15-events)
  6. [Extra flexibility with signals.](#16-extra-flexibility-with-signals)
  7. [Multiple Windows.](#17-multiple-windows)
  8. [Object Oriented Design.](#18-object-oriented-design)
2. [Modern C++ and Cinder](#2-modern-c-and-cinder)
  1. [Namespaces.](#21-namespaces)
  2. [Auto keyword and type inference.](#22-auto-keyword-and-type-inference)
  3. [Range-based loops.](#23-range-based-loops)
  4. [Const-correctness and parameter passing.](#24-const-correctness-and-parameter-g)
  5. [Override keyword.](#25-override-keyword)
  6. [Lambdas, std::function and std::bind.](#26-lambdas-stdfunction-and-stdbind)
  7. [Smart Pointers and Cinder's "create pattern".](#27-smart-pointers-and-cinders-e-pattern)
3. [User Interface](#3-user-interface)
  1. [Cinder's Params.](#31-cinders-params)
  2. [Immediate mode UI.](#32-immediate-mode-ui)
4. [Graphics](#4-graphics)
  1. [Helpers functions and the gl::namespace.](#41-helpers-functions-and-the-glnamespace)
  2. [Vertex Batch and TriMesh.](#42-vertex-batch-and-trimesh)
  3. [Batch.](#43-batch)
  4. [States and Scoped Objects.](#44-states-and-scoped-objects)
  5. [Images, Surfaces and gl::Textures.](#45-images-surfaces-and-gltextures)
  6. [Hot-Reloading Images.](#46-hot-reloading-images)
  7. [Stock Shaders.](#47-stock-shaders)
  8. [Importing 3D Models.](#48-importing-3d-models)
  9. [Texturing 3D Models.](#49-texturing-3d-models)
  10. [Hot-Reloading Model and Textures.](#410-hot-reloading-model-and-textures)
  11. [Custom Glsl Program.](#411-custom-glsl-program)
  12. [Hot-Reloading Glsl Programs.](#412-hot-reloading-glsl-programs)
5. [Runtime-Compiled C++](#5-runtime-compiled-c)

___

###1. Short introduction to Cinder

> [Cinder is a C++ library for programming with aesthetic intent - the sort of development often called creative coding. This includes domains like graphics, audio, video, and computational geometry. Cinder is cross-platform, with official support for OS X, Windows, iOS, and WinRT.](https://libcinder.org/about)

> [Cinder is production-proven, powerful enough to be the primary tool for professionals, but still suitable for learning and experimentation.](https://libcinder.org/about)

#####1.1. [From openFrameworks to Cinder.](apps/101 oFAppStructure/src)
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
  
You can see [here a cinder app organised using oF approach to structuring the source code](apps/101 oFAppStructure/src). Both are extremely similar when organising the code that way :

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



#####1.2. [Cinder App Structure.](apps/102 CinderAppStructure/src/CinderAppStructure.cpp)
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

#####1.3. [App constructor, destructor and cleanup method.](apps/103 AppConstructor/src/AppConstructorApp.cpp)
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


#####1.4. [App Settings.](apps/104 AppSettings/src/AppSettingsApp.cpp)
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

This is a bit less compact and elegant but it does exactly the same with a static function: 
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

#####1.5. [Events.](apps/105 AppEvents/src/AppEventsApp.cpp)
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

#####1.6. [Extra flexibility with signals.](apps/106 FlexibilityWithSignals/src/FlexibilityWithSignalsApp.cpp)
Cinder offers another level of flexibility in how you deal with the app events thanks to its use of "signals". IMO Cinder's signal implementation is based on the best available out there. It is fast, reliable and well designed.  
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

#####1.7. [Multiple Windows.](apps/107 MultipleWindow/src/MultipleWindowApp.cpp)
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

#####1.8. Object Oriented Design.   
Another big difference worth mentioning between Cinder and other libraries such as Processing or openFrameworks is the Object Oriented approach in the design of the library. As we've seen previously seen Cinder is indeed providing a long list of classes that are used everywhere in the library.  

So when Processing's `background` or openFrameworks `ofBackground` methods accepts a series of`floats` describing the different channels of the clear color; Cinder's equivalent `gl::clear` accepts a `Color` or `ColorA` objects.  

The same can be said of Processing `rect( x, y, w, h )` and openFrameworks `ofDrawRectangle( x, y, w, h )` where Cinder version would look like `gl::drawSolidRect( Rectf( x1, y1, x2, y2 ) )`. (I think openFrameworks's `ofDrawRectangle(const ofRectangle &r)` has only been added in recent versions).  

This approach becomes obvious when you look at Cinder's graphic API and even more when looking at its OpenGL abstraction layer.  


___
###2. Modern C++ and Cinder
C++11 (and C++14) has brought a great number of very nice new features to the standard and Cinder makes extensive use of them. This section will focus on how Cinder might differ from other frameworks in the way it uses C++. This sections will also give a short overview of some of the principles introduced recently in the standard.    

I would strongly advise to have a look to [David Wicks / sansumbrella.com](http://sansumbrella.com/) amazing ["A Soso Tour of Cpp"](https://github.com/sosolimited/Cpp-Handbook/blob/master/tour-of-cpp.md) if you want a more complete introduction on the subject.  

#####2.1. [Namespaces.](apps/)
Namespaces can't really be called a modern feature but as we have seen previously Cinder relies a lot on them. Namespaces are just a convenient way to group sections of code. It allows us to stick to most logical names for classes and functions without caring too much about name conflicts.
```c++
namespace MyLibrary {
	struct vec2 {
		float x, y;
	};
}
```
From there you can access the `vec2` type by writting `MyLibrary::vec2` or by adding a `using namespace MyLibrary`. As `vec2` might be a common choice of name in other libraries, this ensures that we won't have any issues using them in the same project.    
**A good habit is to never add `using namespace` in a header file and leave them exclusively to the .cpp files.**


#####2.2. [Auto keyword and type inference.](apps/)
C++ is a strongly typed language meaning that, unlike dynamic languages like JavaScript, you have to give a type to the variable you create (hence the abscence of a `var` keywords like in JS). The introduction of type inference slighly changes that while retaining the safety of a strongly typed language.  

Type inference is the ability of the compiler to automatically deduce the types of your variables.  
```c++
auto someNumber 		= 123.456f;	// the compiler will see this as a float
auto someOtherNumber 	= 789.012;	// the compiler will see this as a double
auto anEmptyRectangle	= Rectf();	// the compiler will see this as a Rectf
```
It sometimes makes the code more readable and in other case saves you from writting very long types. Let's say that we have a map of texture format that we want to iterate:    
```c++
// C++11 allows us to write what we used to write like this:
std::map<string,gl::Texture2d::Format>::iterator it = mTextures.begin();
// In a much shorter way
auto it = mTextures.begin();
```
#####2.3. [Range-based loops.](apps/)
Another really nice new feature in C++11 are Range-Based loops. A Range-For allows you to iterate through the "range" of a container. Basically any standard container that has a `begin()` and a `end()` can be used in a Range-For. It relies on type inference as well and uses the `auto` keyword we've seen previously.  
```c++
vector<float> numbers;
for( auto number : numbers ) {
	console() << number << endl;
}
```
Again it is especially usefull when using types with a long name: 
```c++
// This long and hard to read for loop
for( std::map<string,gl::Texture2d::Format>::iterator it = mTextures.begin(); it != mTextures.end(); ++it ) {
}
// could be written as:
for( auto texture : mTextures ) {
}
```

#####2.4. [Const-correctness and parameter passing.](apps/)

We could write a book about const-correctness and parameter passing in C++ but in very short this is just a good habit to take. It will make your code more readable, self-documented, safer and sometimes more efficient. `Const` basically allows to state and make it clear to yourself and others when something should not be changed or modified. This is something you will see a lot in Cinder.  

Internet is full of articles on this subject but probably the easiest thing to remember is the 4 following points : 

- Pass an argument by value when it is a **built-in type or a small object** (Passing by value makes a *copy of the object*) : 
```c++
void firstFunction( int number, bool boolean );
```
- Pass an argument by reference when you want the argument to be **read-write**:
```c++
void secondFunction( Rectf &rectangle );
```
- Pass an argument by const reference when you want the argument to be **read-only** (Read-only also ensure that your data *won't be unecessarely copied* when calling the function).
```c++
void thirdFunction( const vector<gl::Texture> &textures );
```

Only a quick glance at those 3 functions arguments is enough to know what the functions will do with the objects you pass to them.  

- When writting a class's method that doesn't modify the content of the class mark it as 'const'.
```c++
class MyClass {
public:
	string getName() const;	
};
```

#####2.5. [Override keyword.](apps/)

The `override` keyword was introduce recently in c++ and using it is another good habit to take. We've seen it used in most apps snippets above and its main purpose is to ensure that you make less mistakes when overriding methods. Adding this keyword after a function clearly states that your intent is to override an existing method of the base class. If the method doesn't exist in the base class, you'll get a nice and clear compile-time error.    

```c++
class CinderApp : public App {
public:
	void setup() override;
	void cleaanup() override; // compile-time error ! (cleaanup doesn't exist in App).
};
```


#####2.6. [Lambdas, std::function and std::bind.](apps/)
Lambdas and `std::function` are great additions to the standard. We've seen them used previously in the App Settings and Signals sections. A `std::function` is a standard way of representing a reference to a function. They are used to easily pass around callbacks and functions. Unlike traditional function pointers, `std::function` is short, simple and easy to remember. Just pass the signature of the function as the parameter of the template :   

```c++
void registerCallback( const function<void()> &callback );
void registerMouseEvent( const function<bool(MouseEvent event)> &event );
```
Lambdas is going just a step further and allows us to write an anonymous function as we would write any variable.  
```c++
auto divideByTen = []( float &i ) {
	i /= 10.0f;
};
console() << divideByTen( 100.0f ) << endl; // output 10.0f
```

Syntax of lambda functions is not really complicated but introduce some unusual combination of characters :
```
[ capture-list ] ( params ) { body }
[ capture-list ] ( params ) -> ret { body } 
```

The capture-list comes from the fact that lambdas have their own private scope and are not aware of the context they are written in or what was declared before them. For that reason there is different approaches to passing objects to a lambda scope, as there is different approaches to passing arguments to a function.  
	
	- [] captures nothing 
	- [this] captures the this pointer by value
	- [a,&b] where a is captured by value and b is captured by reference.
	- [=] captures all automatic variables odr-used in the body of the lambda by value
	- [&] captures all automatic variables odr-used in the body of the lambda by reference

`std::bind` simply allows (among other things) to bind together an object and a member function in an easy and standard way.
```c++
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void registerCallback( const function<void()> &callback );

class CinderApp : public App {
public:
    CinderApp();
    void callBack() {}
};

CinderApp::CinderApp()
{
	auto callback = bind( &CinderApp::callback, this );
	registerCallback( callback );
}
CINDER_APP( CinderApp, RendererGl )
```

#####2.7. [Smart Pointers and Cinder's "create pattern".](apps/)
Cinder relies **a lot** on `shared_ptr`. Unlike other languages C++ doesn't have any *Garbage Collection* system; You have to take care of how you create objects, how do you reserve memory and how do you release it when it's not needed anymore. Fortunately for us C++11 introduced a set of tools to make this much more easy.

**unique_ptr** : The most basic one is the `unique_ptr` that you can see as the usual pointer but where the memory will be automatically released when the object goes out of scope.

```c++
{
	auto ptr = unique_ptr<Type>( new Type() );
	...
} // ptr destructor is called and memory is released
```

C++14 complete this new class by providing the `make_unique` method:
```c++
{
	auto ptr = make_unique<Type>();
	...
} // ptr destructor is called and memory is released
```

**shared_ptr** : Another tool and probably the most commonly used one is the `shared_ptr` class. `shared_ptrs` are used for objects that are going to be shared a lot across the application (hence *shared* in the name). Unlike a `unique_ptr`, a `shared_ptr` doesn't get released when it goes out of scope but when all the references of the object are destroyed. Or when the number of references to the object reach zero.   
```c++
// When passed to this function the shared_ptr is copied and the number of reference to the object increase, but just for the duration of this function.
void function( const shared_ptr<Object> &obj ) // obj.refcount++
{
}  // obj.refcount--

void anotherFunction() {
	// We create a shared_ptr of the Object type; it's the first reference
	// to the object so its reference count is equal to one.
	auto ptr = make_shared<Object>(); // ptr.refcount = 1
	{
		// we make a copy of it
		auto ptr2 = ptr; // ptr.refcount = 2	
	} // ptr2 went out of scope and its destructor is called. ptr.refcount goes back to 1

	// We pass ptr to the other function making a copy of it
	// that get straight away destroyed. Leaving the refcount at one.
	function( ptr ); // ptr.refcount = 1
} 
// ptr destructor is called and ptr.refcount finally reach zero.
// At that point the actual Object destructor is called and the memory is finally released.
```
You can see them being used in a lot of places in Cinder in the form of typedefs. Any Type that ends with a *Ref* in its name is actually a `shared_ptr` (the Ref suffix is short for reference counted object); `WindowRef`, `DisplayRef`, `gl::Texture2dRef`, `gl::VboRef`, etc...    

You'll usually find in the header where the Object is defined a typedef and most of the time a static method in the class to allow to easily instanciate the `shared_ptr`:  
```c++
typedef shared_ptr<class Object> ObjectRef;
class Object {
public:
	static ObjectRef create();
};
```

**weak_ptr** : `weak_ptrs` are a bit less common, and are used in parallel of a `shared_ptr` to hold a *weak reference* to it. A `weak_ptr` doesn't contribute to the *refcount* of its `shared_ptr`. It is most of the time used to break dependency cycles (ex. let's say you have a tree where each node has a `shared_ptr` to each of their children. If the children also *own* the reference to their parent with a `shared_ptr`, you'll create a cycle and nothing will ever be released. Slightly more difficult to explain or understand but their use is very specific and much more rare than the two other kinds of smart pointers).

#####2.8. [Method chaining, Format and Options.](apps/)
Method chaining has nothing new or modern but is worth mentioning as it is used in a lot of places as well in Cinder. Method chaining is some sort of syntactic sugar that allows to call a series of method in a single expression or line. It can usually been seen in Cinder with setters of certain small classes. Most `Options` or `Format` classes has this syntactic sugar.  
```c++
// We've seen it previously used with the `Window::Format` class, where :
auto windowFormat0 = Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 40 ).title( "WindowTitle" );

// Replaces the longer : 
auto windowFormat1 = Window::Format();
windowFormat1.setSize( ivec2( 256 ) );
windowFormat1.setPosition( ivec2( 0, 40 ) );
windowFormat1.setTitle( "WindowTitle" );

// And can of course be used as the argument of a function :
createWindow( windowFormat1 );
createWindow( Window::Format().size( ivec2( 256 ) ).pos( ivec2( 0, 40 ).title( "WindowTitle" ) );
``` 

It is usually implemented simply by having short versions of setters methods that instead of being of the `void` type return a reference to the object itself :
```c++
class Options {
public :
	void setName( const string &name ) { mName = name; }
	void setPosition( const vec2 &pos ) { mPosition = pos; }
	void setRadius( float radius ) { mRadius = radius; }

	Options& name( const string &name ) { mName = name; return *this; }
	Options& position( const vec2 &pos ) { mPosition = pos; return *this; }
	Options& radius( float radius ) { mRadius = radius; return *this; }
};

auto option = Options().name( "Pastrami" ).position( vec2( 10.0f ) ).radius( 1.25f );

```
___
###3. User Interface
The very first actual user of User Interfaces is the developer. For that reason and because in a lot of cases you need a way to interact with the app and be able to test thing out; an easy to use UI library is a must.  

#####3.1. [Cinder's Params.](#31-cinders-params)
Cinder wraps [**anttweakbar**](http://anttweakbar.sourceforge.net/doc/), a small OpenGL library that despites it weird look, does the job pretty well. It is easy to use and provides all sort of small widgets to tweak values in your application. It is definitely not designed to create the final user interface but perfectly fit the role of a developer tool.   
```c++
mParams = params::InterfaceGl::create( "Params", ivec2( 200, 120 ) );
mParams->addParam( "Color", &mSomeColor );
mParams->addParam( "Number", &mSomeFloat ).min( 0.0f ).max( 10.0f );
mParams->addParam( "Orientation", &mSomeQuaternion );
```

#####3.2. [Immediate mode UI.](#32-immediate-mode-ui)
[Dear ImGui](https://github.com/ocornut/imgui) is another small library written by Omar Cornut with more or less the same goal. Providing developers a fast and easy way of creating small tools and testing interfaces. It is very flexible, has a huge number of widgets and is definitely both much more complete and better looking than *anttweakbar*.  

The main difference with *anttweakbar* is in the fact that *Dear ImGui* is designed as a **Immediate Mode User Interface** library. Instead of initializing your UI when the application starts, you write your UI code in the main loop, repeating UI states every frame, like you would do when rendering graphics. Think of it as a some sort of equivalent to a Immediate Mode Graphic API. It seems a bit weird at first but it's extremly powerfull and flexible.  

```c++
void CinderApp::setup()
{
	// initialize Dear ImGui
	ui::initialize();
}
void CinderApp::update()
{
	// add a slider
 	ui::SliderFloat( "Slider", &mSomeFloat );
 	// and a button
 	if( ui::Button( "Button" ) ) {
 		console() << "Button has been clicked!" << endl;
 	}
}
```
One really nice thing with a state-less UI, is the fact that it can really easily reflect the state of your app without any extra-work. Let say that you have a vector of Objects that can be created, deleted or modified: 

```c++
void CinderApp::update()
{
	// Iterate over the current list of objects
 	for( auto object : mObjects ) {
		ui::Text( object->getName() );
		auto color = object->getColor();
		if( ui::ColorEdit4( "Color", &color[0] ) ) {
			object->setColor( color );
		}
		auto pos = object->getPosition();
		if( ui::DragFloat3( "Position", &pos[0] ) ) {
			object->setPosition( pos );
		}
	}
}
```  
___
###4. Graphics

Cinder now has some excellent documentation and a few great guides to get you started. [The OpenGL guide](https://libcinder.org/docs/guides/opengl/) is definitely the place to start if you need some introduction to OpenGL in Cinder.

Since Cinder 0.9 the default version of OpenGL is 3.2 Core Profile on desktop and OpenGL ES 2.0 and 3.0 on mobile. Recent versions of OpenGL have changed drastically while more or less abandoning the [fixed function pipeline](https://www.opengl.org/wiki/Fixed_Function_Pipeline) and the old immediate-mode rendering. In two sentences; all the `glBegin`/`glVertex`/`glEnd` we (wrongly) conceived as convenient before are gone and the (ugly) shading pipeline we had easily access to using `glEnable(GL_LIGHTING)` or `glLightfv(GL_LIGHT0, GL_AMBIENT, ambient )` is gone as well.  

Hopefully Cinder provides a great API and a set of tools to help us understand those changes without having to write a single line of OpenGL.

#####4.1. [Helpers functions and the gl::namespace.](/)

The gl namespace can be split into two series of files, functions and classes. The first one is a series of free-standing functions that wrap opengl functions ([`cinder/gl/wrapper.h`](https://github.com/cinder/Cinder/blob/master/include/cinder/gl/wrapper.h)) or that add short tools that allows to quickly sketch graphics ([`cinder/gl/draw.h`](https://github.com/cinder/Cinder/blob/master/include/cinder/gl/draw.h)). The later is not intended to be performant but is really usefull because it allows us to rapidly write prototypes or debug graphics without the need to use `VBOS`, `VAOS` and `Glsl Programs`.  

You'll find a series of 2D convenience functions like `drawLine, drawSolidRect, drawSolidRoundedRect, drawSolidCircle, drawSolidEllipse, drawStrokedRect, drawStrokedRect, drawStrokedRoundedRect, drawStrokedCircle, drawStrokedCircle, drawStrokedEllipse, drawString, drawStringCentered, drawStringRight, ...` and 3d convenience functions like  `drawCube, drawColorCube, drawStrokedCube, drawStrokedCube, drawSphere, drawSphere, drawBillboard, drawFrustum, drawCoordinateFrame, drawVector, drawLine,...`.

The second type of tools you'll find in the `gl namespace` is a series of classes that abstract OpenGL functionalities. They go from basic things like `Buffers` and `Queries` to higher level objects like `VboMesh`, `TextureFont` or `Batch`.

#####4.2. [Vertex Batch and TriMesh.](/)

Immediate mode rendering (`glBegin`, `glVertex`, `glEnd`) was a mistake in the design of older version of OpenGL and it gave us all bad habits in terms of how to communicate with a GPU. That said, there is still some small occasions where this kind of tools can be usefull when sketching things out or debugging. For this reason Cinder provides a small classes that mimics the way it used to work.  
```c++
auto vertBatch = gl::VertBatch::create();
vertBatch->begin( GL_POINTS );
for( auto p : mPoints ) {
	vertBatch->vertex( p );
}
vertBatch->draw();  
```

Computer graphics can usually be summarized to a group of polygons or triangles. This is the most common representation of any type of graphic on a computer and Cinder's provides several tools that allow to work with triangles and polygons. One of the most commonly used is the `TriMesh` class. A `TriMesh` can represent a 2D or 3D shape with its properties like its colors, texture coordinates, normals, etc ...

```c++
auto trimesh = TriMesh();
trimesh.appendPosition( p0 );
trimesh.appendPosition( p1 );
trimesh.appendPosition( p2 );
trimesh.appendTriangle( 0, 1, 2 );
```

#####4.3. [Batch.](/)

Today graphics in OpenGL are governed by `Vertex Buffer Objects`, `Glsl Programs` and `Vertex Arrays`. The first one describe a list of vertices, its properties (like colors or texture coordinates) and how they are connected to form faces and polygons and the second one describes how the faces of the first are transformed and shaded. It replace the old fixed-function pipeline where a Glsl Program now has to be bound all the time for anything to get rendered. Simply put, the last one allows to group things together in a way that the Graphic Card understand. 

Cinder provides an easy interface that wraps and takes care of all the above called a `gl::Batch`.

```c++
auto batch = gl::Batch::create( trimesh, gl::getStockShader( gl::ShaderDef().color() ) );
batch->draw();
```

#####4.4. [States and Scoped Objects.](/)
#####4.5. [Images, Surfaces and gl::Textures.](/)
#####4.6. [Hot-Reloading Images.](/)
#####4.7. [Stock Shaders.](/)
#####4.8. [Importing 3D Models.](/)
#####4.9. [Texturing 3D Models.](/)
#####4.10. [Hot-Reloading Model and Textures.](/)
#####4.11. [Custom Glsl Program.](/)
#####4.12. [Hot-Reloading Glsl Programs.](/)
#####4.13. [Geom Namespace.](/)
#####4.14. [Vbo and VboMesh.](/)


___
###5. Runtime-Compiled C++