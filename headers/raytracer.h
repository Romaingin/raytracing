#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "camera.h"
#include "face.h"
#include "scene.h"
#include "programoptions.h"
#include "antialiaser.h"
#include "sun.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdint>
#include <vector>

class Raytracer {
private:
	// Layout
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Texture* _image;
	SDL_Texture* _box;
	SDL_PixelFormat* _format;
	Antialiaser _antialiaser;

	// Options
	ProgramOptions po;

	// Scene
	Scene _scene;

	// Methods
	void setPixel(SDL_Surface *, int, int, uint32_t);
public:
	void computeImage ();
	void saveImage (const char *);
	void traceZone (int, int);

	Raytracer (ProgramOptions&);
	~Raytracer ();
};

#endif
