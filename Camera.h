#include "Point.h"
#include "Vector.h"

#IFNDEF CAMERA_H
#DEFINE CAMERA_H

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

#ENDIF