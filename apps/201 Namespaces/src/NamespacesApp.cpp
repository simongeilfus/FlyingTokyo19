#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// Namespaces are just a useful way to group section of code
namespace FlyingTokyo {
	
// We can safely declare anything here
// without having to worry about name conflicts.
// It allows us to pick the most natural names for
// anything we write.
class vec3 {
public: 
	float x, y, z;
};

class SomeEmptyClass {
public:
};

}

// We can also make aliases or shortcuts for namespace
// Keeping the real name of the namespace with the most 
// natural name and still get a short convenient version.
namespace ft = FlyingTokyo;

// We wouldn't be able to do the same here
// as Cinder has the a class with the same name
// #define WONT_COMPILE
#ifdef WONT_COMPILE
class vec3 {
public: 
	float x, y, z;
};

// If we try to build this, the compiler won't be able
// to choose between ci::vec3 and ft::vec3.
vec3 staticVector;
#endif

class NamespacesApp : public App {
public:
	NamespacesApp()
	{
		// We can now access both classes safely
		ci::vec3 cinderVector;
		ft::vec3 flyingTokyoVector;

		// To make things even shorter we can also use 
		// "using namespace" from anywhere in our code
		// to not have to write the full scoped name.
		// !!! Avoid using that in header files !!!
		using namespace ft;

		// We don't need to put ft:: in front of the type anymore:
		SomeEmptyClass emptyObject;

		// Nothing else to see here, quit the app!
		quit();
	}
};

CINDER_APP( NamespacesApp, RendererGl )
