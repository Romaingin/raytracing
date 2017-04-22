#ifndef ANTIALIASER_H
#define ANTIALIASER_H

#include "programoptions.h"
#include <glm/glm.hpp>
#include <vector>

class Antialiaser {
private:
	int _level;
	std::vector<glm::vec2> _pixelScattering;
	std::vector<float> _pixelScatteringWeigths;
	color_t _partialColor;
	short _currentIndex;

public:
	std::vector<glm::vec2> getPixelSamplesRepartition () { return _pixelScattering; }
	void setSampleValue (color_t);
	void resetPixelValue ();
	color_t getPixelValue () { return _partialColor; };

	Antialiaser (int);
	~Antialiaser () {}
};

#endif
