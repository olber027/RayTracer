#include "Color.h"
#include <string>
#include <fstream>

#ifndef OUTPUT_H
#define OUTPUT_H

using namespace std;

enum OutputType {

	ToFile
	
};
//eventually this class will be able to do multiple different types of output. 
//currently, I'm only implementing outputting to a file. 

//PPM file format:
//P6
//<width> <height>
//255
//<r> <g> <b> <r> <g> <b> etc...
//<r> <g> <b> <r> <g> <b> etc...
//<r> <g> <b> <r> <g> <b> etc...
//etc...
class Output {

	public: 
	
		Output(string path, int height, int width) {
			outputType = OutputType::ToFile;
			filePath = path; 
			imgHeight = height;
			imgWidth = width;
			img = new Color*[height];
			for(int i = 0; i < height; i++) {
				img[i] = new Color[width]; 
			}
		}
		
		~Output() {
			for(int i = 0; i < imgHeight; i++) {
				delete img[i];
			}
			delete img;
		}
		
		void setColor(int x, int y, Color color) {
			img[x][y] = color; 
		}
		
		void writeImage() {
			if(outputType == OutputType::ToFile) {
				writeImageToFile();
			}
		}

		void setOutputType(OutputType type) {
			outputType = type;
		}
		
		void writeImageToFile() {
			ofstream out(filePath);

			out << "P6" << endl;
			out << imgWidth << " " << imgHeight << endl;
			out << "255" << endl;
			for(int i = 0; i < imgWidth; i++) {
				for(int j = 0; j < imgHeight; j++) {
					out << img[i][j].getFormattedColorString() << " ";
				}
				out << endl;
			}
		}

		
	private:
	
		OutputType outputType;
		string filePath; 
		Color** img;
		int imgHeight, imgWidth;
};

#endif