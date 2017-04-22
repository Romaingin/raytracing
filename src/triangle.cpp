#include "triangle.h"
#include <glm/gtx/norm.hpp>

using namespace glm;

Triangle::Triangle (vec3 A, vec3 B, vec3 C) {
	_A = A;
	_B = B;
	_C = C;

	// Compute edges
	_U = _B - _A;
	_V = _C - _A;
	_W = _B - _C;

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

// > isRayThrough
//		Check if a ray v coming from a certain point P passes within the triangle
bool Triangle::isRayThrough(vec3 ray, vec3 P) {
	if (!_isDefined) return false;

	// Get the intersection with the triangle plan
	float t = (_d - dot(_n, P)) / dot(_n, ray);
	vec3 Inter = P + t * ray;

	// Get the result
	vec3 x = Inter - _A;
	vec3 y = Inter - _B;
	vec3 z = Inter - _C;
	vec3 u = _B - _A;
	vec3 v = _C - _B;
	vec3 w = _A - _C;

	vec3 n1 = cross(u, x);
	vec3 n2 = cross(v, y);
	vec3 n3 = cross(w, z);

	bool b1 = dot(n1, n2) > 0;
	bool b2 = dot(n2, n3) > 0;

	return b1 && b2; //  && dot(_V, B) <= 0
}
