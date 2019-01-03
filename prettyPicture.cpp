#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "RayTracer.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std; 

int main() {
	
	//set up scene geometry. this will probably be reading things from a file
	RayTracer rayTracer = RayTracer("OutputFile", "ConfigFile");

	rayTracer.trace();

	rayTracer.writeToOutput();

	return 0;
}

