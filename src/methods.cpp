#include "methods.h"
#include "material.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace glm;

color_t tracer (Scene& scene, vec3 ray, vec3 origin, float n, size_t depth) {
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
	color_t res = color_t(0.0,0.0,0.0,1.0);
	color_t reflection = color_t(0.0,0.0,0.0,1.0);
	color_t refraction = color_t(0.0,0.0,0.0,1.0);
	Material mat = scene.elements[elementId]->material;

	// Shadowed ?
	if (shadowMapping(scene, faceId, elementId, intersection)) {
		shadow = color_t(0.2,0.2,0.2,1.0);
	}

	// Diffuse
	float lighting = std::max(0.0f, -dot(normal, scene.sun.getDirection()));
	if (!mat.castShadow) { lighting = 1.0; return scene.backgroundColor; }
	diffuse = scene.sun.getColor() * lighting * scene.sun.getIntensity() * mat.diffuseColor;

	if (depth > 0) {
		// Refraction
		if (mat.transmittivity > 0.0) {
			refraction = refractionMapping(scene, faceId, elementId,
											intersection, ray, normal,
											n,
											depth-1);

			if (mat.reflectivity == 0.0) {
				return clamp(shadow * diffuse + mat.transmittivity * refraction);
			}
		}

		// Reflection
		if (mat.reflectivity > 0.0) {
			reflection = reflectionMapping(scene, faceId, elementId,
											intersection, ray, normal,
											n,
											depth-1);

			if (mat.refractionIndex == 0.0) {
				return clamp(shadow * diffuse + mat.reflectivity * reflection);
			}
		}

		// Fresnel effect ?
		float kr;
		if (mat.transmittivity > 0.0 && mat.reflectivity > 0.0) {
			// Compute the Fresnel number
			kr = fresnel(scene, faceId, elementId, ray, normal, n);
		} else {
			kr = 0.0f;
		}

		res = mat.reflectivity * (kr * reflection + (1.0f-kr) * refraction);
		return clamp(shadow * diffuse + mat.diffuseColor * res);

	} else {
		// No more depth
		return clamp(shadow * diffuse);
	}
}

void intersectionFinder (Scene& scene, vec3 ray, vec3 origin, int& faceId, size_t& elementId, vec3& intersection, vec3& normal) {
	faceId = -1;
	vec3 inter_tmp;
	float minDist = 1e99;
	float dist = minDist;

	for (size_t el = 0; el < scene.elementNumber; el++) {
		for (Face f : scene.elements[el]->faces) {
			if (f.isRayThrough(ray, origin, &dist, &inter_tmp) && dist < minDist && dist > 0.01) {
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
			if (!(f.getId() == faceId && el == elementId) &&
				f.isRayThrough(-scene.sun.getDirection(), origin, &dist, &inter_tmp) &&
				dist > 0.01 && scene.elements[el]->material.castShadow) {
				return true;
			}
		}
	}

	return false;
}

color_t reflectionMapping (Scene& scene, int, size_t, vec3 origin, vec3 incidentRay, vec3 normal, float n1, size_t depth) {
	vec3 reflectedRay = incidentRay - 2 * dot(incidentRay, normal) * normal;
	return tracer(scene, reflectedRay, origin, n1, depth);
}

color_t refractionMapping (Scene& scene, int, size_t elementId, vec3 origin, vec3 incidentRay, vec3 normal, float n1, size_t depth) {
	// Material properties
	float n2;
	if (dot(incidentRay, normal) < 0) {
		n2 = scene.elements[elementId]->material.refractionIndex;
	} else {
		n2 = 1.0;
	}
	float n1dn2 = n1 / n2;

	// Angles
	float cosi = -dot(normalize(incidentRay), normal);
	float k = 1.0f - n1dn2 * n1dn2 * (1.0f - cosi * cosi);
	if (k < 0) {
		return color_t(0.0,0.0,0.0,0.0);
	} else {
		vec3 refractedRay = n1dn2 * incidentRay + (n1dn2 * cosi - sqrt(k)) * normal;
		return tracer(scene, refractedRay, origin, n2, depth);
	}
}

float fresnel(Scene& scene, int, size_t elementId, vec3 incidentRay, vec3 normal, float n) {
	float cosi = std::max(-1.0f,std::min(dot(incidentRay, normal), 1.0f));
	float etai = n;
	float etat = scene.elements[elementId]->material.refractionIndex;
	float kr;

	if (cosi > 0.0f) { std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * std::sqrt(std::max(0.f, 1.0f - cosi * cosi));
	// Total internal reflection
	if (sint >= 1.0f) {
		kr = 1.0f;
	} else {
		float cost = std::sqrt(std::max(0.0f, 1.0f - sint * sint));
		cosi = std::abs(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2.0f;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
	return kr;
}

color_t clamp (color_t color) {
	return color_t( std::max(0.0f,std::min(color.x, 1.0f)),
					std::max(0.0f,std::min(color.y, 1.0f)),
					std::max(0.0f,std::min(color.z, 1.0f)),
					std::max(0.0f,std::min(color.t, 1.0f)));
}
