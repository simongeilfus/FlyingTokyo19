# FlyingTokyo19

####1. Short introduction to Cinder

One of the first thing

#####1.1 From openFrameworks to Cinder.
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

If you forget about cosmetics and don't get too much into details both Cinder and oF are C++ frameworks and they work the same in terms of general app structure. They both have a main entry point and a `App` class from which all your apps inherit.  
  
You can see a (cinder app here organised using oF approach to structuring the source code)[/101 oFAppStructure/src]. Both are extremely similar when organising the code that way :

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

#####1.2 Cinder App Structure.
#####1.3 App constructor, destructor cleanup and shutdown.
#####1.4 Events and Object-oriented design.
#####1.5 App Settings / prepareSetting and C++11 Lambdas.
#####1.6 Extra flexibility with signals
#####1.7 Multiple Windows

___
####2. Cinder App Structure