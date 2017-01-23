#include "Geometry.h"
#include "Ray.h"
#include "Color.h"
#include "RayTracer.h"
#include <vector> 

using namespace std; 

int main() {
	
	//set up scene geometry. this will probably be reading things from a file
	RayTracer rayTracer = RayTracer("OutputFile", "ConfigFile");
	
	rayTracer.trace();

	rayTracer.writeToOutput();

	return 0;
}

