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

            update();
		}
		
		//this one returns a ray starting at the camera origin and
		//passing through the given x and y coordinates of the "screen"
		Ray getRayThrough(int x, int y) {
			Vector Xvec = (topRight - topLeft)*(x == 0 ? 0 : double(x)/double(width));
            Vector Yvec = (bottomLeft - topLeft)*(y == 0 ? 0 : double(y)/double(height));
            Vector vec = Xvec + Yvec;
            Ray result = Ray(origin, ((topLeft+vec) - origin).getNormalizedVector());
            return result;
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
		void setOrigin(Point o) { origin = o; update();}
        //the zAxis is aligned along "view", the xAxis is
        //orthogonal to Z and in angle "theta"
        void setAxes(Vector view, double theta) {
            zAxis = view*-1;



            update();
        }

		void setViewAngle(double angle) { viewAngle = angle; update(); }
		void setNearClippingDistance(int dist) { nearClippingDistance = dist; update(); }
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
        //four corners of the "screen"
        Point topLeft;
        Point topRight;
        Point bottomLeft;
        Point bottomRight;


        void update() {
            //TODO
        }
};

#endif