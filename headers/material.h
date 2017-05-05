#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <string>

struct Materiau {
	std::string nom;
	glm::vec4 specularColor;
	glm::vec4 diffuseColor;
	glm::vec4 selfillumColor;
	float shinestrength;
	float shineness;
	float transmittivity;
	float reflectivity;
	bool permanent;
};

#endif
