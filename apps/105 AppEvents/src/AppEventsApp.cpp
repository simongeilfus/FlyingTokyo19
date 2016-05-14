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
