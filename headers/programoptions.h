#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

#include <glm/glm.hpp>

struct ProgramOptions {
	int image_width;	// Width of the picture
	int image_height;	// Height of the picture
	int target_size;	// Width and height of the areas to render sequentially
	int antialiasing;	// Level of antialiasing
};

typedef glm::vec4 color_t;

#endif
