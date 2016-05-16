#pragma once

#include "cinder/gl/Batch.h"

class Object {
public:
	Object();
	
	// methods that needs to be modified at runtime have be virtual
	virtual void draw();
	
protected:
	ci::gl::BatchRef mPlane, mTeapot;
};