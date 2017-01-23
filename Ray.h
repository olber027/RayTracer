#include "Vector.h"
#include "Point.h"
#include <cstdlib>

using namespace std;

#ifndef RAY_H
#define RAY_H

class Ray {
	public:
		Ray(Point start, Vector dir) : startPoint(start), direction(dir) {}
		Ray() : Ray(Point(), Vector()) {}
		
		Point getStartPoint() {
			return startPoint;
		}
		
		Vector getDirection() {
			return direction;
		}
		
	private: 
		Point startPoint;
		Vector direction;

};

#endif