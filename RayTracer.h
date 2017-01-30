#include "Ray.h"
#include "Geometry.h"
#include "Color.h"
#include "Camera.h"
#include "Output.h"
#include "Utils.h"
#include <cmath>
#include <string> 
#include <Vector>
#include <vector>

#ifndef RAYTRACER_H
#define RAYTRACER_H

int MAX_REFLECTION_DEPTH = 0;


class RayTracer {


	public:
	
		RayTracer(string outputFilePath, string configDataFilePath) {
            int height, width;
            readConfigData(configDataFilePath, height, width, &camera);
			output = Output(outputFilePath, height, width);
		}
		
		//does all the logic for the tracer and sends the result to the 
		//currently set output destination; 
		void trace() {
			//for each pixel
			for(int i = 0; i < camera.getWidth(); i++) {
				for(int j = 0; j < camera.getHeight(); j++) {
					//create a ray
					Ray ray = camera.getRayThrough(i, j); 
					//set color for that pixel to findColorForRay()
					Color pixelColor = findColorForRay(ray, geometry, 0);
					//output the color to the appropriate destination. 
					output.setColor(i, j, pixelColor);
				}
			}
		}

        void writeToOutput() {
            output.writeImage();
        }

	private: 

		Camera camera;
		vector<Geometry*> geometry; 
		Color backgroundColor;
		Output output; 
		

		Color findColorForRay(Ray ray, vector<Geometry*> sceneGeometry, int depth) {

			/*
            vector<Geometry*> intersectingObjects = vector<Geometry*>();
			Geometry* obj;
			Point intersectionPoint = NULL;

			for(int i = 0; i < sceneGeometry.size(); i++) {
				obj = sceneGeometry[i]; 
				intersectionPoint = obj->intersectsAt(ray);
				if(intersectionPoint != NULL) {
					intersectingObjects.push_back(obj);
				}
			}
			
			obj = Utilities::findClosestObject(camera.getOrigin(), intersectingObjects);
			intersectionPoint = obj->intersectsAt(ray);

			if(obj != NULL) {
				if(obj->isReflective() && depth < MAX_REFLECTION_DEPTH) {
					Ray reflectedRay = obj->findReflectedRay(ray);
					return findColorForRay(reflectedRay, sceneGeometry, depth+1); // + some other numbers based on an equation I don't have right now;
				} else {
					return obj->getColorAt(intersectionPoint);
				}
			}
			*/
			return backgroundColor;
		}

        void readConfigData(string filePath, int& height, int& width, Camera* camera) {
            ifstream in = ifstream(filePath);
            (*camera) = Camera();
            in >> height >> width;
            camera->setHeight(height);
            camera->setWidth(width);

            double x, y, z;
			double cameraRotationAngle;
            in >> x >> y >> z;
            camera->setOrigin(Point(x,y,z));
            in >> x >> y >> z;
            in >> cameraRotationAngle;
			camera->setAxes(Vector(x,y,z), cameraRotationAngle);
            double angle;
            in >> angle;
            camera->setViewAngle(angle);
            int nearClippingDistance, farClippingDistance;
            in >> nearClippingDistance >> farClippingDistance;
            camera->setNearClippingDistance(nearClippingDistance);
            camera->setFarClippingDistance(farClippingDistance);

            int r, g, b;
            in >> r >> g >> b;
            backgroundColor = Color(r, g, b);

        }
};

#endif