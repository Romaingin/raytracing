#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Face {
private:
	// Id
	int _id;

	// Points
	glm::vec3 _A;
	glm::vec3 _B;
	glm::vec3 _C;

	// Edges
	glm::vec3 _U;
	glm::vec3 _V;
	glm::vec3 _W;

	// Normal
	glm::vec3 _n;
	bool _isDefined;
	float _d;

public:
	glm::vec3 getNormal() { return _n; }
	bool isRayThrough(glm::vec3, glm::vec3, float*, glm::vec3*);
	int getId() {
		return _id;
	}

	Face (int, glm::vec3, glm::vec3, glm::vec3);
	Face (int, glm::vec3, glm::vec3, glm::vec3, glm::vec3);
	~Face () {}
};

#endif
