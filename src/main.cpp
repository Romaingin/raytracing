#include "raytracer.h"
#include <iostream>

int main () {
	// Init SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	Raytracer raytracer(512, 512);

	// Run
	raytracer.computeImage();

	// Save Image
	raytracer.saveImage("render.bmp");

	// Quit
	IMG_Quit();
	SDL_Quit();

	return 0;
}
