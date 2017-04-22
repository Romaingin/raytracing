#include "antialiaser.h"
#include <algorithm>
#include <iostream>

Antialiaser::Antialiaser (int level) {
	_level = level;
	std::cout << "Antialiasing Level : " << _level << '\n';

	// Set scattering profile and weights
	float norm = 0;
	size_t count = 0;
	if (_level <= 0) {
		_pixelScattering.push_back(glm::vec2(0.5, 0.5));
		_pixelScatteringWeigths.push_back(1.0);
		norm += 1.0;
		count += 1;
	} else if (_level <= 1) {
		_pixelScattering.push_back(glm::vec2(0, 0));
		_pixelScattering.push_back(glm::vec2(0, 1));
		_pixelScattering.push_back(glm::vec2(1, 0));
		_pixelScattering.push_back(glm::vec2(1, 1));
		_pixelScatteringWeigths.push_back(0.2);
		_pixelScatteringWeigths.push_back(0.2);
		_pixelScatteringWeigths.push_back(0.2);
		_pixelScatteringWeigths.push_back(0.2);
		norm += 0.2 * 4;
		count += 4;
	} else if (_level <= 2) {
		_pixelScattering.push_back(glm::vec2(0.5, 0.25));
		_pixelScattering.push_back(glm::vec2(0.5, 0.75));
		_pixelScattering.push_back(glm::vec2(0.25, 0.5));
		_pixelScattering.push_back(glm::vec2(0.75, 0.5));
		_pixelScatteringWeigths.push_back(0.5);
		_pixelScatteringWeigths.push_back(0.5);
		_pixelScatteringWeigths.push_back(0.5);
		_pixelScatteringWeigths.push_back(0.5);
		norm += 0.5 * 4;
		count += 4;
	} else {
		std::cout << "Wrong Antialiasing level" << '\n';
		exit(0);
	}

	// Normalise weights
	for (size_t i = 0; i < count; i++) {
		_pixelScatteringWeigths[i] /= norm;
	}
}

void Antialiaser::setSampleValue (color_t newColor) {
	_partialColor += _pixelScatteringWeigths[_currentIndex] * newColor;
	_currentIndex += 1;
}

void Antialiaser::resetPixelValue () {
	_partialColor = color_t(0,0,0,0);
	_currentIndex = 0;
}
