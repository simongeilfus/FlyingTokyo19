#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/System.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TypeInference : public App {
public:
	TypeInference()
	{
		// Type Inference is the ability of the compiler to deduce the types 
		// of our variables. We use the "auto" keyword for this :
		auto variable0 = 1.0f;						// compiler will see that as a float
		auto variable1 = 0.123;						// compiler will see that as a double
		auto variable2 = Rectf();	// compiler will see that as a ci::Rectf
		
		// This is especially helpful when writing variables that have a really long Type name :
		auto shortVersion = mTextureFormats.begin();
		std::map<string,gl::TextureCubeMap::Format>::iterator longVersion = mTextureFormats.begin();
		
		// Print the different variables type names:
		console() << System::demangleTypeName( typeid( decltype( variable0 ) ).name() ) << endl;
		console() << System::demangleTypeName( typeid( decltype( variable1 ) ).name() ) << endl;
		console() << System::demangleTypeName( typeid( decltype( variable2 ) ).name() ) << endl;
		console() << System::demangleTypeName( typeid( decltype( longVersion ) ).name() ) << endl;
		console() << System::demangleTypeName( typeid( decltype( shortVersion ) ).name() ) << endl;
		
		// Nothing else to see here, quit the app!
		quit();
	}

	std::map<string,gl::TextureCubeMap::Format> mTextureFormats;
};

CINDER_APP( TypeInference, RendererGl )
