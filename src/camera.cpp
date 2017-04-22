#include "camera.h"
#include "glm/ext.hpp"
#include <iostream>

Camera::Camera (glm::vec3 position, glm::vec3 target, float fov, float aspect) {
	_position = position;
	_aimVect = target - position;
	_fov = fov;
	_aspect = aspect;

	// Compute dirs
	float w = tan(M_PI * _fov / 360.0);
	_X = w / _aspect * glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), _aimVect));
	_Y = -w * glm::normalize(glm::cross(_X, _aimVect));
}

// > getRay
//		Given a postion on the screen, return the coresponding ray coming
//		from the camera and passing through that position
glm::vec3 Camera::getRay (float x, float y) {
	return glm::normalize(_aimVect - x * _X - y * _Y);
}
