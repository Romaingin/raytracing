#include "methods.h"
#include "material.h"
#include <algorithm>

using namespace glm;

color_t tracer (Scene& scene, vec3 ray, vec3 origin, size_t depth) {
	int faceId;
	size_t elementId;
	vec3 intersection;
	vec3 normal;

	intersectionFinder(scene, ray, origin, faceId, elementId, intersection, normal);

	if (faceId < 0) {
		return scene.backgroundColor;
	}

	// Define colors
	color_t shadow = color_t(1.0,1.0,1.0,1.0);
	color_t diffuse;
	color_t reflection = color_t(0.0,0.0,0.0,1.0);
	color_t refraction = color_t(0.0,0.0,0.0,1.0);
	Material mat = scene.elements[elementId]->material;

	// Shadowed ?
	if (shadowMapping(scene, faceId, elementId, intersection)) {
		shadow = color_t(0.0,0.0,0.0,1.0);
	}

	// Diffuse
	float lighting = std::max(0.0f, -dot(normal, scene.sun.getDirection()));
	diffuse = scene.sun.getColor() * lighting * scene.sun.getIntensity() * mat.diffuseColor;

	// Reflection
	if (depth > 0) {
		reflection = reflectionMapping(scene, faceId, elementId, intersection, ray, normal, depth-1);
	}

	// Refraction

	return clamp(diffuse * shadow + mat.reflectivity * reflection);
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

bool shadowMapping (Scene& scene, int faceId, size_t elementId, vec3 origin) {
	float dist;
	vec3 inter_tmp;
	vec3 inter;

	for (size_t el = 0; el < scene.elementNumber; el++) {
		for (Face f : scene.elements[el]->faces) {
			if (f.getId() != faceId && el != elementId && f.isRayThrough(-scene.sun.getDirection(), origin, &dist, &inter_tmp) && dist > 0) {
				return true;
			}
		}
	}

	return false;
}

color_t reflectionMapping (Scene& scene, int faceId, size_t elementId, vec3 origin, vec3 incidentRay, vec3 normal, size_t depth) {
	vec3 reflectedRay = incidentRay - 2 * dot(incidentRay, normal) * normal;
	return tracer(scene, reflectedRay, origin, depth);
}

color_t clamp (color_t color) {
	return color_t( std::max(0.0f,std::min(color.x, 1.0f)),
					std::max(0.0f,std::min(color.y, 1.0f)),
					std::max(0.0f,std::min(color.z, 1.0f)),
					std::max(0.0f,std::min(color.t, 1.0f)));
}
