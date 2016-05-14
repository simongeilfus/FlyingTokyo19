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
	void draw() override;
	void cleanup() override;
	
	string mSomeMember;
};

AppConstructorApp::AppConstructorApp()
: mSomeMember( "World" ) // the member initializer list is a convenient place to initialize your variables
{
	// This is the first place where code get executed
	// during the app construction.
	console() << "Hello " << mSomeMember << endl;
}
AppConstructorApp::~AppConstructorApp()
{
	// This is the last place where code get executed
	// during the app destruction
	console() << "Goodbye from destructor" << endl;
}
void AppConstructorApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}
void AppConstructorApp::cleanup()
{
	// This is a safe place to release and cleanup
	// things before the actual destruction of the app
	console() << "Cleaning up" << endl;
}

CINDER_APP( AppConstructorApp, RendererGl )
