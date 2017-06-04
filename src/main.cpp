#include "raytracer.h"
#include "programoptions.h"
#include <iostream>

int main () {
	// Init SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	// OPTIONS of the program
	ProgramOptions po = {
		512,	// Width of the picture
		512,	// Height of the picture
		32,		// Width and height of the areas to render sequentially
		2,		// Level of antialiasing
		4		// Max recursion depth
	};

	std::cout << "Image resolution : " << po.image_width << "x" << po.image_height << '\n';
	std::cout << "Antialiasing Level : " << po.antialiasing << '\n';


	Raytracer raytracer(po);

	// Run
	raytracer.computeImage();

	// Save Image
	raytracer.saveImage("render.bmp");

	// Quit
	IMG_Quit();
	SDL_Quit();

	return 0;
}
