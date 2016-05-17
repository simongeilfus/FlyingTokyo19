#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RangeBasedLoop : public App {
public:
	RangeBasedLoop()
	{
		// Range-For allows you to iterate through the "range" of a container. 
		// It relies on type inference as well and uses the auto keyword we've seen previously.
		vector<int> vectorOfIntegers = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
		for( auto i : vectorOfIntegers ) {
			console() << i << endl;
		}

		// Basically any standard container that has a begin() and a end() can be used in a Range-For.
		// Which means that any vector, array, map, deque can be iterated that way.
		map<string,int> mapOfIntegers = { { "ichi", 1 }, { "ni", 2 }, { "san", 3 } };
		for( auto i : mapOfIntegers ) {
			console() << i.first << " " << i.second << endl;
		}

		// Nothing else to see here, quit the app!
		quit();	
	}

};

CINDER_APP( RangeBasedLoop, RendererGl )
