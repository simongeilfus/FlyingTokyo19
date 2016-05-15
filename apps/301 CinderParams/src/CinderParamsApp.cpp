#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderParamsApp : public App {
  public:
	void setup() override;
	void draw() override;

	Color	mClearColor, mCircleColor;
	vec3	mCirclePosition;
	float	mCircleSize;
	params::InterfaceGlRef mParams;
};

void CinderParamsApp::setup()
{
	// Initialize our variables
	mClearColor		= Color::black();
	mCircleColor	= Color::white();
	mCirclePosition = vec3( getWindowCenter(), 0.0f );
	mCircleSize		= 150.0f;
	
	// Initialize the params interface
	mParams = params::InterfaceGl::create( "Params", ivec2( 200, 120 ) );
	mParams->addParam( "Clear Color", &mClearColor );
	mParams->addParam( "Circle Color", &mCircleColor );
	mParams->addParam( "Circle Position", &mCirclePosition );
	mParams->addParam( "Circle Size", &mCircleSize ).min( 0.0f ).max( (float) getWindowWidth() );
}

void CinderParamsApp::draw()
{
	// Clear the screen and render our circle
	gl::clear( mClearColor );
	gl::color( mCircleColor );
	gl::drawSolidCircle( vec2( mCirclePosition ), mCircleSize );
	
	// Render the UI
	mParams->draw();
}

CINDER_APP( CinderParamsApp, RendererGl( RendererGl::Options().msaa( 8 ) ) )
