#ifndef ELEMENT_H
#define ELEMENT_H

#include "face.h"
#include "material.h"
#include <vector>

struct Element {
	std::vector<Face> faces;
	Material material;
};

#endif
