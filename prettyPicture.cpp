#include "Geometry.h"
#include "Ray.h"
#include "Color.h"
#inlcude <cstdlib>
#include <vector> 

using namespace std; 

int main() {
	
	//set up scene geometry. this will probably be reading things from a file
	RayTracer rayTracer = new RayTracer("name of file to read geometry from"); 
	
	rayTracer.setOutputDestination("file", "prettyPicture.png");
	
	rayTracer.trace(); 
}

