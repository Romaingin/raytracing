#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <string>

struct Material {
	std::string nom;
	glm::vec4 specularColor;
	glm::vec4 diffuseColor;
	float diffusivity;
	float transmittivity;
	float reflectivity;
	float reflectionIndex;
	bool castShadow;
};

#endif
