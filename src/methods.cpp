#include "methods.h"

using namespace glm;

color_t tracer (Scene& scene, vec3 ray, vec3 origin) {
	color_t color = scene.backgroundColor;
	int faceId;
	size_t elementId;
	vec3 intersection;
	vec3 normal;

	intersectionFinder(scene, ray, origin, faceId, elementId, intersection, normal);

	if (faceId >= 0) {
		if (shadowMapping(scene, faceId, elementId, intersection)) {
			color = vec4(0.0,0.0,0.0,1.0);
		} else {
			float lighting = std::max(0.0f, -dot(normal, scene.sun.getDirection()));
			color = scene.sun.getColor() * lighting * scene.sun.getIntensity() * scene.elements[elementId]->material.diffuseColor;
		}
	}

	return color;
}

void intersectionFinder (Scene& scene, vec3 ray, vec3 origin, int& faceId, size_t& elementId, vec3& intersection, vec3& normal) {
	faceId = -1;
	vec3 inter_tmp;
	float minDist = 1e99;
	float dist = minDist;

	for (size_t el = 0; el < scene.elementNumber; el++) {
		for (Face f : scene.elements[el]->faces) {
			if (f.isRayThrough(ray, origin, &dist, &inter_tmp) && dist < minDist && dist > 0) {
				minDist = dist;
				faceId = f.getId();
				elementId = el;
				intersection = inter_tmp;
				normal = f.getNormal();
			}
		}
	}
}

bool shadowMapping (Scene& scene, int faceId, size_t elementId, vec3 point) {
	float dist;
	vec3 inter_tmp;
	vec3 inter;

	for (size_t el = 0; el < scene.elementNumber; el++) {
		for (Face f : scene.elements[el]->faces) {
			if (f.getId() != faceId && el != elementId && f.isRayThrough(-scene.sun.getDirection(), point, &dist, &inter_tmp) && dist > 0) {
				return true;
			}
		}
	}

	return false;
}

color_t reflectionMapping (Scene& scene, int faceId, size_t elementId, vec3 point, vec3 incidentRay) {

}
