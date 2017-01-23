#include "Point.h"
#include "Vector.h"
#include "Ray.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {

	public: 
		//TODO: lots more functionality that needs to be added here. 
	
		Camera() {
			//TODO
		}
		
		//this one returns a ray starting at the camera origin and
		//passing through the given x and y coordinates of the "screen"
		Ray getRayThrough(int x, int y) {
			//TODO
		}

		int getWidth() {
			return height;
		}
		
		int getHeight() {
			return width;
		}
		
		Point getOrigin() {
			return origin;
		}

		void setHeight(int h) { height = h; }
		void setWidth(int w) { width = w; }
		void setOrigin(Point o) { origin = o; }
		void setXAxis(Vector x) { xAxis = x; }
		void setYAxis(Vector y) { yAxis = y; }
		void setZAxis(vector z) { zAxis = z; }
		void setViewAngle(double angle) { viewAngle = angle; }
		void setNearClippingDistance(int dist) { nearClippingDistance = dist; }
		void setFarClippingDistance(int dist) { farClippingDistance = dist; }

	private: 
		int height; 
		int width; 
		Point origin;
		Vector xAxis;
		Vector yAxis;
		Vector zAxis; 
		double viewAngle;
		int nearClippingDistance;
		int farClippingDistance; 
		
};

#endif