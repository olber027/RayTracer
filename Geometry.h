#include "Point.h"
#include "Ray.h"
#include "Color.h"
#include <cmath>


virtual class Geometry {
	public:
		
		//returns true if the given ray intersects the geometry.
		virtual bool intersects(Ray ray);
		
		//returns the point at which the ray intersects the geometry. null if it doesn't intersect
		virtual Point intersectsAt(Ray ray); 
	
		//returns the color at the given point. 
		virtual Color getColorAt(Point point); 
		
		virtual Ray findReflectedRay(Ray ray); 
		
		bool isReflective() {
			if(reflectivity <= 0.0)
				return false;
			return true; 
		}
		
	protected:
	
		Color color;
		
		double reflectivity; 
		
		Geometry() : color(new Color()), reflectivity(0.0) {}
};

class Sphere : public Geometry {
	public: 
	
		Sphere(Point center, double radius, Color color, double reflectivity) {
			this.center = center;
			this.radius = radius;
			this.color = color;
			this.reflectivity = reflectivity; 
		}
		
		Sphere(Point center, double radius) {
			this.center = center; 
			this.radius = radius;
			Geometry();  
		}
	
		bool intersects(Ray ray) {
			Vector rayVec = ray.toVector();
			Vector centerVec = center - ray.startPoint();
			double rayAngle = (rayVec*centerVec)/(rayVec.magnitude()*centerVec.magnitude());
			rayAngle = acos(rayAngle); 
		}
		
		Point intersectsAt(Ray ray) {
			if(!intersects(ray))
				return null;
			
		}

		Color getColorAt(Point point) {
			//TODO
		}

		Ray findReflectedRay(Ray ray) {
			//TODO
		}
		
	private: 
	
		Point center; 
		
		double radius;  
		
};


class Cube : public Geometry {
	public: 
	
		Cube(Point center, double length, Color color, double reflectivity, Vector orientation) {
			this.center = center;
			this.length = length;
			this.color = color;
			this.reflectivity = reflectivity; 
			this.orientation = orientation;
		}
		
		Cube(Point center, double length, Color color, double reflectivity) {
			this.center = center;
			this.length = length;
			this.color = color;
			this.reflectivity = reflectivity; 
			orientation = Vector(0,0,1);
		}
		
		Cube(Point center, double length, Vector orientation) {
			this.center = center; 
			this.length = length;
			this.orientation = orientation;
			Geometry();  
		}
		
		Cube(Point center, double length) {
			this.center = center; 
			this.length = length;
			orientation =  Vector(0,0,1);
			Geometry();  
		}
	
		bool intersects(Ray ray) {
			//TODO
		}
		
		Point intersectsAt(Ray ray) {
			if(!intersects(ray))
				return null;
			//TODO
		}

		Color getColorAt(Point point) {
			//TODO
		}

		Ray findReflectedRay(Ray ray) {
			//TODO
		}
		
	private: 
		//center of the cube. 
		Point center; 
		//side length
		double length;  
		//orientation of the cube. this vector will be pointing at the top face of the cube. 
		Vector orientation; 
};


class Plane : public Geometry {
	public: 
	
		Cube(Point center, double width, double height, Color color, double reflectivity, Vector orientation) {
			this.center = center;
			this.width = width;
			this.height = height; 
			this.color = color;
			this.reflectivity = reflectivity; 
			this.orientation = orientation;
		}
		
		Cube(Point center, double width, double height, Color color, double reflectivity) {
			this.center = center;
			this.width = width;
			this.height = height; 
			this.color = color;
			this.reflectivity = reflectivity; 
			orientation = Vector(0,0,1);
		}
		
		Cube(Point center, double width, double height, Vector orientation) {
			this.center = center; 
			this.width = width;
			this.height = height; 
			this.orientation = orientation;
			Geometry();  
		}
		
		Cube(Point center, double width, double height) {
			this.center = center; 
			this.width = width;
			this.height = height; 
			orientation =  Vector(0,0,1);
			Geometry();  
		}
	
		bool intersects(Ray ray) {
			//TODO
		}
		
		Point intersectsAt(Ray ray) {
			if(!intersects(ray))
				return null;
			//TODO
		}

		Color getColorAt(Point point) {
			//TODO
		}

		Ray findReflectedRay(Ray ray) {
			//TODO
		}
		
	private: 
	
		Point center; 
		double width;
		double height;
		//orientation is a unit vector perpendicular to the plane. 
		Vector orientation;
		
};

