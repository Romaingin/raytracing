#ifndef SUN_H
#define SUN_H

#include <glm/glm.hpp>

class Sun {
private:
	glm::vec3 _dir;
	float _intensity;
	glm::vec4 _color;
public:
	glm::vec3 getDirection () {
		return _dir;
	}
	glm::vec4 getColor () {
		return _color;
	}
	void setColor (glm::vec4 new_color) {
		_color = new_color;
	}
	float getIntensity () {
		return _intensity;
	}
	void setIntensity (float new_intensity) {
		_intensity = new_intensity;
	}

	Sun (glm::vec3 dir) {
		_dir = glm::normalize(dir);
		_intensity = 1.0;
		_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	}
	~Sun () {}
};

#endif
