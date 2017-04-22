#include "objloader.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <string.h>
#include <sstream>
#include <string>
#include <fstream>

// > objLoader
//		Load a particular obj file into an array of faces
bool objLoader(const char *path, std::vector<Face>& outFaces) {
	std::ifstream in(path, std::ios::in);
	if (!in) {
		std::cerr << "Cannot open " << path << std::endl;
		return false;
	}

	std::vector<glm::vec3> outVertices;
	std::vector<glm::vec3> outNormals;
	std::string line;
	while (std::getline(in, line)) {
		// What type of info ?
		std::string type = line.substr(0,2);
		if (type == "v ") {
			std::istringstream v(line.substr(2));
			float x, y, z;
			v>>x; v>>y; v>>z;
			outVertices.push_back(glm::vec3(x, y, z));
		} else if (type == "vn") {
			std::istringstream v(line.substr(2));
			float x, y, z;
			v>>x; v>>y; v>>z;
			outNormals.push_back(glm::vec3(x, y, z));
		} else if (type == "f ") {
			unsigned int vertexIndex[3], normalIndex[3];
			const char* chh = line.c_str();
			sscanf (chh, "f %d//%d %d//%d %d//%d\n",&vertexIndex[0], &normalIndex[0],
													&vertexIndex[1], &normalIndex[1],
													&vertexIndex[2], &normalIndex[2] );

			// Create a new face
			outFaces.push_back(Face(outVertices[vertexIndex[0]-1],
									outVertices[vertexIndex[1]-1],
									outVertices[vertexIndex[2]-1],
									outNormals[normalIndex[0]-1]));
		}
	}

	return true;
}
