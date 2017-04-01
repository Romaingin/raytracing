#include "raytracer.h"
#include <iostream>

int main (int argc, char const *argv[]) {
	// Init SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	Raytracer* raytracer = new Raytracer(512, 512);

	// Run
	raytracer->computeImage();

	// Save Image
	raytracer->saveImage("render.bmp");

	// Quit
	delete raytracer;
	IMG_Quit();
	SDL_Quit();

	return 0;
}
