#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "sun.h"
#include "face.h"
#include "element.h"
#include <vector>
#include <glm/glm.hpp>

struct Scene {
	Camera camera;
	Sun sun;
	size_t elementNumber;
	std::vector<Element*> elements;
	glm::vec4 backgroundColor;

	Scene(glm::vec3 position, glm::vec3 target, float fov, float aspect, glm::vec3 sunDir) :
		camera {position, target, fov, aspect}, sun {sunDir}
	{ elementNumber = 0; }
};

#endif
