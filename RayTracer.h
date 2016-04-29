#include "Ray.h"
#include "Geometry.h"
#include "Color.h"
#include "Camera.h"
#include "Output.h"
#include "Utils.h"
#include <cmath>
#include <string> 
#include <Vector>

#IFNDEF RAYTRACER_H
#DEFINE RAYTRACER_H

int MAX_REFLECTION_DEPTH = 0;


public class RayTracer {


	public: 
	
		RayTracer() {
			//TODO
		}
	
		RayTracer(string filePath) {
			configDataFromFile(filePath);
		}
		
		void setOutputDestination(string filePath) {
			if(output != null) {
				output.dispose();
			}
			output = new Output(filePath);
		}
		
		//does all the logic for the tracer and sends the result to the 
		//currently set output destination; 
		void trace() {
			//for each pixel
			for(int i = 0; i < camera.getWidth(); i++) {
				for(int j = 0; j < camera.GetHeight(); j++) {
					//create a ray
					Ray ray = camera.getRayThrough(i, j); 
					//set color for that pixel to findColorForRay()
					Color pixelColor = findColorForRay(ray, geometry, 0);
					//output the color to the appropriate destination. 
					output.setColor(i, j, pixelColor);
				}
			}
		}

	private: 

		Camera camera;
		vector<Geometry*> geometry; 
		Color backgroundColor;
		Output output; 
		

		Color findColorForRay(Ray ray, vector<Geometry*> sceneGeometry, int depth) {
			
			vector<Geometry*> intersectingObjects; 
			Geometry* obj;
			
			for(int i = 0; i < sceneGeometry.size(); i++) {
				obj = sceneGeometry[i]; 
				Point intersectionPoint = obj->intersectsAt(ray); 
				if(intersectionPoint != null) {
					intersectingObjects.pushBack(obj); 
				}
			}
			
			obj = Utilities.findClosestObject(camera.origin, intersectingObjects); 
			
			if(obj != null) {
				if(obj->isReflective() && depth < MAX_REFLECTION_DEPTH) {
					Ray reflectedRay = obj->findReflectedRay(ray);
					return findColorForRay(reflectedRay, sceneGeometry, depth+1) // + some other numbers based on an equation I don't have right now;
				} else {
					return obj->getColorAt(intersectionPoint);
				}
			}
			
			return backgroundColor;
		}
		
		void configDataFromFile(string filePath) {
			//TODO
			//read in values in some sort of pre-defined manner...
		}
};

#ENDIF