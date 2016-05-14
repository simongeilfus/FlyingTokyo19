#include "cinder/app/RendererGl.h"
#include "ofApp.h"

int main( int argc, char* argv[] )
{
	ci::app::RendererRef renderer( new ci::app::RendererGl() );
	ci::app::AppMac::main<ofApp>( renderer, "ofApp", argc, argv );
	return 0;
}
