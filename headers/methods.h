#ifndef METHODS_H
#define METHODS_H

#include "scene.h"
#include <glm/glm.hpp>

// > tracer
//		Main methods that calls all the raytracing functions sequencially
//		Return the color of the given ray's pixel
glm::vec4 tracer (Scene& scene, glm::vec3 ray);

// > intersectionFinder
//		Perform test over all elements in the scene to determin
//		if the ray passes through a face
void intersectionFinder (Scene& scene, glm::vec3 ray, int& faceId, glm::vec3& intersection, glm::vec3& normal);

// > shadowMapping
//		Perform test over all elements in the scene to determin
//		if a given point is shadowed by a face
bool shadowMapping (Scene& scene, int faceId, glm::vec3 point);

#endif
