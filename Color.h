#include <string>

#IFNDEF COLOR_H
#DEFINE COLOR_H

public class Color {

	public:
	//TODO: lots of things
	
		Color(int r, int g, int b, double alpha) {
			this.r = r;
			this.g = g;
			this.b = b;
			this.alpha = alpha;
		}
		
		Color(const Color& c) {
			r = c.r;
			g = c.g;
			b = c.b;
			alpha = c.alpha;
		}
		
		// returns a string of the format "r g b"
		string getFormattedColorString()
		{
			return to_string(r) + " " + to_string(g) + " " + to_string(b);
		}
		
		Color getColorApplyingAlpha(Color background) {
			return new Color((int) (alpha*r + ((1-alpha)*background.getR())),
									 (int) (alpha*g + ((1-alpha)*background.getG())),
									 (int) (alpha*b + ((1-alpha)*background.getB())),
									0);
		}
		
		int getR() { return r; }
		int getG() { return g; }
		int getB() { return b; }
		double getAlpha() { return alpha; }
	
	private: 
		int r, g, b;
		double alpha;

};

#ENDIF