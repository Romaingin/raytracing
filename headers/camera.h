#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 _position;
	glm::vec3 _Y;
	glm::vec3 _X;
	glm::vec3 _aimVect;
	float _fov;
	float _aspect;

public:
	glm::vec3 getPosition() { return _position; };
	glm::vec3 getAimVect() { return _aimVect; };
	glm::vec3 getRay(float, float);

	Camera (glm::vec3, glm::vec3, float, float);
	~Camera () {}
};

#endif
