#include "methods.h"

using namespace glm;

vec4 tracer (Scene& scene, vec3 ray) {
	vec4 color;
	int faceId;
	vec3 intersection;
	vec3 normal;

	intersectionFinder(scene, ray, faceId, intersection, normal);

	if (faceId >= 0) {
		if (shadowMapping(scene, faceId, intersection)) {
			color = vec4(0,0,0,255);
		} else {
			float lighting = std::max(0.0f, -dot(normal, scene.sun.getDirection()));
			color = scene.sun.getColor() * lighting * scene.sun.getIntensity();
		}
	}

	return color;
}

void intersectionFinder (Scene& scene, vec3 ray, int& faceId, vec3& intersection, vec3& normal) {
	faceId = -1;
	vec3 inter_tmp;
	float minDist = 1e99;
	float dist = minDist;

	for (Face f : scene.elements[0]->faces) {
		if (f.isRayThrough(ray, scene.camera.getPosition(), &dist, &inter_tmp) && dist < minDist) {
			minDist = dist;
			faceId = f.getId();
			intersection = inter_tmp;
			normal = f.getNormal();
		}
	}
}

bool shadowMapping (Scene& scene, int faceId, vec3 point) {
	float dist;
	vec3 inter_tmp;
	vec3 inter;

	for (Face f : scene.elements[0]->faces) {
		if (f.getId() != faceId && f.isRayThrough(-scene.sun.getDirection(), point, &dist, &inter_tmp) && dist > 0) {
			return true;
		}
	}

	return false;
}
