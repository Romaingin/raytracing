#ifndef MATERIAU_H
#define MATERIAU_H

#include <glm/glm.hpp>

class Materiau {
	private :
		char _nom;
		glm::vec4 _speculatColor;
		glm::vec4 _diffuseColor;
		glm::vec4 _selfillumColor;
		float shinestrength;
		float shineness;
		float transmittivity;
		float reflectivity;
		bool permanent ;

	public :
	char getName () {
		return _nom;
	}
	glm::vec4 getDiffuseColor () {
		return _diffuseColor;
	}
	glm::vec4 getSelfillumColor () {
		return _selfillumColor;
	}
	float getShinestrenth () {
		return _shinestrength;
	}
	float getShineness () {
		return _shineness;
	}
	float getTransmittivity () {
		return _transmittivity;
	}
	float getReflectivity () {
		return _reflectivity;
	}
	bool getPermanent () {
		return _permanent; 
	}
};

#endif
