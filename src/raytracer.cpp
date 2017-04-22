#include "raytracer.h"
#include "glm/ext.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <unistd.h>

Raytracer::Raytracer (int width, int height) :
	_screen_width(width), _screen_height(height), _target_size(32),
	_camera(glm::vec3(0,0,1), glm::vec3(0,0,0), 90.0, (float)width / (float)height) {

	// Create SLD window
	_window = SDL_CreateWindow(
		"Drone@Hack",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_screen_width,
		_screen_height,
		0
	);

	// Check that the window was successfully created
	if (_window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}

	// Renderer
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);

	_image = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
								_screen_width, _screen_height);
	_box = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
								_target_size, _target_size);
	_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	// Create a box
	SDL_SetRenderTarget(_renderer, _box);
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(_renderer, 0, 0, _target_size-1, 0);
	SDL_RenderDrawLine(_renderer, 0, 0, 0, _target_size-1);
	SDL_RenderDrawLine(_renderer, _target_size-1, 0, _target_size-1, _target_size-1);
	SDL_RenderDrawLine(_renderer, _target_size-1, _target_size-1, 0, _target_size-1);
	SDL_RenderPresent(_renderer);
	SDL_SetRenderTarget(_renderer, NULL);
}

void Raytracer::computeImage () {
	SDL_Event event;
	bool isRunning = true;
	bool hasFinished = false;

	// Coordinates
	int x = 0;
	int y = 0;
	SDL_Rect box_rect;
	box_rect.w = _target_size;
	box_rect.h = _target_size;

	// Main loop
	while (isRunning) {
		// Events
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}

			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
		}

		if (!hasFinished) {
			// Render a zone
			traceZone(x, y);
			x += _target_size;
			if (x >= _screen_width) {
				x = 0;
				y += _target_size;
			}
			if (y >= _screen_height) {
				hasFinished = false;
			}

			// Draw a box
			box_rect.x = x;
			box_rect.y = y;
		}

		// Render to screen
		SDL_RenderCopy(_renderer, _image, NULL, NULL);
		SDL_RenderCopy(_renderer, _box, NULL, &box_rect);
		SDL_RenderPresent(_renderer);
	}
}

// > saveImage
//		Write the screen content to a image file
void Raytracer::saveImage (const char* file_name) {
	SDL_Surface *sshot = SDL_CreateRGBSurface(0, _screen_width, _screen_height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, file_name);
	SDL_FreeSurface(sshot);
}

// > traceZone
//		Compute a particlar square (X, Y) of the image
void Raytracer::traceZone (int X, int Y) {
	// usleep(3000); // TODO

	// TODO
	Triangle T(glm::vec3(0,0,0),glm::vec3(0.3,0.1,0),glm::vec3(-0.1,0.2,0));

	void *tmp;
	Uint32 *pixels;
	int pitch;

	// Lock
	SDL_LockTexture(_image, NULL, &tmp, &pitch);
	pixels = (Uint32 *)tmp;

	int sx = std::min(_target_size, _screen_width - X);
	int sy = std::min(_target_size, _screen_height - Y);
	for (int i = 0; i < sx; i++) {
		for (int j = 0; j < sy; j++) {
			// Get the ray coming from the camera
			float x = (float)(X + i) / (float)_screen_width - 0.5;
			float y = (float)(Y + j) / (float)_screen_height - 0.5;
			glm::vec3 ray = _camera.getRay(x, y);

			// Test if in triangle
			Uint8 s = 255 * T.isRayThrough(ray, _camera.getPosition());

			pixels[(Y + j) * _screen_width + (X + i)] = SDL_MapRGBA(_format, s, s, s, 255);
			// std::cout << glm::to_string(_camera.getPosition()) << '\n';
		}
	}

	// Unlock
	SDL_UnlockTexture(_image);
}

Raytracer::~Raytracer () {
	// Layout
	SDL_FreeFormat(_format);
	SDL_DestroyTexture(_image);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}
