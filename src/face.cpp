#include "face.h"
#include <glm/gtx/norm.hpp>

using namespace glm;

// No normal specified
Face::Face (int id, vec3 A, vec3 B, vec3 C) {
	_id = id;
	_A = A;
	_B = B;
	_C = C;

	// Compute edges
	_U = _B - _A;
	_V = _C - _A;
	_W = _C - _B;

	// Compute the normal (check if not null)
	vec3 dir = cross(_U, _V);
	_isDefined = true;
	if (length2(dir) > 0.0f) {
		_n = normalize(dir);
	} else {
		_n = vec3(0,0,0);
		_isDefined = false;
	}

	_d = dot(_n, _A);
}

// Normal specified
Face::Face (int id, vec3 A, vec3 B, vec3 C, vec3 N) {
	_id = id;
	_A = A;
	_B = B;
	_C = C;

	// Compute edges
	_U = _B - _A;
	_V = _C - _A;
	_W = _C - _B;

	// Compute the normal (check if not null)
	_n = N;
	_isDefined = true;
	if (length2(N) > 0.0f) {
	} else {
		_isDefined = false;
	}

	_d = dot(_n, _A);
}

// > isRayThrough
//		Check if a ray v coming from a certain point P passes within the triangle
bool Face::isRayThrough(vec3 ray, vec3 P, float *dist, vec3 *inter) {
	if (!_isDefined) return false;

	// Get the intersection with the triangle plan
	float t = (_d - dot(_n, P)) / dot(_n, ray);
	vec3 Inter = P + t * ray;

	// Get the result
	vec3 n1 = cross( _U, Inter - _A);
	vec3 n2 = cross( _W, Inter - _B);
	vec3 n3 = cross(-_V, Inter - _C);

	bool b1 = dot(n1, n2) >= 0;
	bool b2 = dot(n2, n3) >= 0;

	*dist = t;
	*inter = Inter;
	return b1 && b2;
}
