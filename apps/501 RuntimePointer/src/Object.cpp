#include "Object.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/scoped.h"

using namespace std;
using namespace ci;

Object::Object()
{
	mPlane = gl::Batch::create( geom::Plane().subdivisions( ivec2( 10 ) ).size( vec2( 10 ) ) >> geom::Lines(), gl::getStockShader( gl::ShaderDef().color() ) );
	mTeapot = gl::Batch::create( geom::Teapot(), gl::getStockShader( gl::ShaderDef().lambert() ) );
}
void Object::draw()
{
	gl::ScopedDepth scopedDepth( true );
	mPlane->draw();
	mTeapot->draw();
}