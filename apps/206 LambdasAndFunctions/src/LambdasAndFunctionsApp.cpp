#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"

#include <functional>

using namespace ci;
using namespace ci::app;
using namespace std;


// simple free function
void freeFunction() 
{
	console() << "I am a non-member function" << endl;
}

// simple free function with arguments
string freeHello( const string &name ) 
{
	console() << "Hello " << name << endl;
	return "Hello " + name;
}

// class with a member function and a basic callback system
class FlyingTokyo {
public:
	void ft19();
	void registerCallback( const std::function<void()> &callback );
	~FlyingTokyo();
protected:
	std::function<void()> mCallback;
};


class LambdasAndFunctions : public App {
public:
	LambdasAndFunctions()
	{
		console() << "_______________" << "Functions" << endl << endl;

		//________________________________________________________________

		// std::function are general purpose function wrappers. They can be
		// used to store, copy and invoke any type of function.
		//________________________________________________________________

		// Storing free functions in a std::function is as simple as using the operator=
		// just specify the function signature as the template <> argument
		function<void()> func0 = freeFunction;
		function<string(const string&)> func1 = freeHello;

		// you can of course use type inference for functions
		auto func2 = freeFunction;

		// Storing member functions requires an extra step because we 
		// also need to keep a reference to the object that the method
		// belongs to. We use std::bind for that, and simply pass the name
		// of the class, the name of the function and a pointer to the object.
		FlyingTokyo ft;
		auto func3 = bind( &FlyingTokyo::ft19, &ft );

		// You can of course do the same from inside 
		// the class itself by using the "this" keyword
		auto func4 = bind( &LambdasAndFunctions::memberFunction, this );
		// Functions with arguments requires a tiny bit more syntax as you 
		// need to use placeholders for each arguments of the function.
		auto func5 = bind( &LambdasAndFunctions::hello, this, placeholders::_1, placeholders::_2 );
		
		
		// Once the functions are stored you can invoke 
		// them the same way you would with any function
		console() << "func0: "; 
		func0();
		console() << "func1: ";
		func1( "World" );
		console() << "func2: ";
		func2();
		console() << "func3: ";
		func3();
		console() << "func5: ";
		func4();
		console() << "func5: ";
		func5( "Flying", "Tokyo" );

		console() << endl << "_______________" << "Lambdas" << endl << endl;

		//________________________________________________________________

		// Lambdas are closure or unnamed functions. You can declare them
		// inside other functions or pass them as function arguments
		//________________________________________________________________
		

		// this is the basic syntax to write a lambda. Start with [], the capture list
		// then follow with the usual function syntax
		auto lambda0 = []() { app::console() << "Hello lambda!" << endl; };
		lambda0();

		// lambdas have their own scope, so anything living outside of the 
		// lambda scope need to be "captured" to be accessible in the lambda
		auto flying = "Flying";
		auto tokyo = []() { return "Tokyo"; };
		auto addSpaceBetween = []( const string &a, const string &b ) { return a + " " + b; };
		auto lambda1 = [flying,tokyo,addSpaceBetween]() { app::console() << "Hello " << addSpaceBetween( flying, tokyo() ) << endl; };
		lambda1();

		// the scope of "this" can also be passed to the capture list
		auto lambda2 = [this]() {
			// getAppPath() is a member of the App class
			auto appPath = getAppPath();
			// same goes for console() which explains why
			// the two previous call to the console were
			// using the free alias version of it app::console()
			console() << appPath << endl;
		};
		lambda2();

		// the same as with any callable objects, 
		// lambdas can be captured in a std::function
		function<string()> func6 = []() { return string( "Hello lambda!" ); };
		console() << func6() << endl;

		// Usually the type returned by the function is deduced by the compiler
		// but you can also explicitly specify the return type of a lambda by 
		// adding -> Type between the ) and {
		auto lambda3 = []( int a, int b ) -> int { return a + b; };
		console() << "1 + 1 = " << lambda3( 1, 1 ) << endl;

		console() << endl << "_______________" << "Basic Callback System" << endl << endl;

		
		//________________________________________________________________

		// Basic Callback System
		//________________________________________________________________
		FlyingTokyo flyingTokyo;
		flyingTokyo.registerCallback( [this]() {
			console() << "Call me back from FlyingTokyo Destructor" << endl;
		} );

		// Quit the App
		quit();
	}
	
	// simple member function
	void memberFunction() 
	{
		console() << "I am a member function" << endl;
	}
	
	// simple member function with arguments
	string hello( const string &firstName, const string lastName ) 
	{
		console() << "Hello " << firstName << " " << lastName << endl;
		return "Hello " + firstName + " " + lastName;
	}
};

// FlyingTokyo Implementation
void FlyingTokyo::ft19()
{
	console() << "I'm a member of Flying Tokyo class" << endl;
}

void FlyingTokyo::registerCallback( const std::function<void()> &callback )
{
	mCallback = callback;
}
FlyingTokyo::~FlyingTokyo()
{
	if( mCallback ) {
		mCallback();
	}
}

CINDER_APP( LambdasAndFunctions, RendererGl )
