#include "Point_X.h"
#include "Vector_X.h"
#include "Ray.h"
#include "Matrix_MxN.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {

	public:
	
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

		int getWidth() { return height; }
		int getHeight() { return width; }
		Point getOrigin() {	return origin; }

		void setHeight(int h) { height = h; update(); }
		void setWidth(int w) { width = w; update(); }
		void setOrigin(Point o) { origin = o; update(); }
        //the zAxis is aligned along "view", the xAxis is
        //orthogonal to Z and in angle "theta" (theta is given in radians)
        void setAxes(Vector view, double theta) {
            zAxis = view*-1;
			zAxis.normalize();
			yAxis = zAxis.cross(Vector(std::cos(theta),std::sin(theta),0));
			yAxis.normalize();
			xAxis = yAxis.cross(zAxis);
			xAxis.normalize();
            update();
        }

		void setAxes(Vector x, Vector y, Vector z, double theta) {
			zAxis = z;
			zAxis.normalize();
			xAxis = x;
			xAxis.normalize();
			yAxis = y;
			yAxis.normalize();
			viewAngle = theta;
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
		double viewAngle; //in radians
		int nearClippingDistance;
		int farClippingDistance;
        //four corners of the "screen"
        Point topLeft;
        Point topRight;
        Point bottomLeft;
        Point bottomRight;


        void update() {
			double X = nearClippingDistance*tan(viewAngle/2.0)*2.0;
			double Y = ((double) height/(double) width) * X;
			topLeft = transformPoint(Point(X/-2, Y/2, 0));
			topRight = transformPoint(Point(X/2, Y/2, 0));
			bottomLeft = transformPoint(Point(X/-2, Y/-2, 0));
			bottomRight = transformPoint(Point(X/2, Y/-2, 0));
        }

		Point transformPoint(Point p) {
			Point result = Point(3);
			Matrix translation;
			return result;
		}
};

#endif