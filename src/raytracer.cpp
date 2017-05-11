#include "raytracer.h"
#include "glm/ext.hpp"
#include "objloader.h"
#include "methods.h"
#include <glm/glm.hpp>
#include <iostream>
#include <unistd.h>
#include <algorithm>

Raytracer::Raytracer (ProgramOptions& po_) :
	_antialiaser {po_.antialiasing},
	_scene {glm::vec3(2.1,2.5,3), glm::vec3(0,0,0), 90.0,
			(float)po_.image_width / (float)po_.image_height,
			glm::vec3(0.7, -1.5, -1.2)} {
	// Options
	po = po_;

	// Create SLD window
	_window = SDL_CreateWindow(
		"Raytracer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		po.image_width,
		po.image_height,
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
								po.image_width, po.image_height);
	_box = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
								po.target_size, po.target_size);
	_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	// Create a box
	SDL_SetRenderTarget(_renderer, _box);
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(_renderer, 0, 0, po.target_size-1, 0);
	SDL_RenderDrawLine(_renderer, 0, 0, 0, po.target_size-1);
	SDL_RenderDrawLine(_renderer, po.target_size-1, 0, po.target_size-1, po.target_size-1);
	SDL_RenderDrawLine(_renderer, po.target_size-1, po.target_size-1, 0, po.target_size-1);
	SDL_RenderPresent(_renderer);
	SDL_SetRenderTarget(_renderer, NULL);

	// Materials
	Material matSolid = {
		"Solid",
		glm::vec4(0.0,1.0,1.0,1.0),
		glm::vec4(1.0,1.0,1.0,1.0),
		1.0,
		0.00,
		0.30,
		1.00,
		true
	};

	Material matGreenGlass = {
		"GreenGlass",
		glm::vec4(1.0,0.95,0.95,1.0),
		glm::vec4(0.15,1.0,0.15,1.0),
		0.15,
		0.50,
		0.50,
		1.50,
		true
	};

	Material matBlueGlass = {
		"BlueGlass",
		glm::vec4(1.0,0.95,0.95,1.0),
		glm::vec4(0.15,0.1,0.95,1.0),
		0.15,
		0.90,
		0.50,
		1.50,
		true
	};

	Material matRedGlass = {
		"RedGlass",
		glm::vec4(1.0,0.95,0.95,1.0),
		glm::vec4(0.95,0.1,0.15,1.0),
		0.15,
		0.90,
		0.50,
		1.50,
		true
	};

	// Set scene
	_scene.backgroundColor = glm::vec4(0.9,0.9,1.0,1.0);

	_scene.elements.push_back(new Element()); // Floor
	objLoader("scene/ground.obj", _scene.elements[0]->faces);
	_scene.elements[0]->material = matSolid;
	_scene.elements.push_back(new Element()); // Cubes
	objLoader("scene/suzane.obj", _scene.elements[1]->faces);
	_scene.elements[1]->material = matGreenGlass;
	_scene.elements.push_back(new Element()); // Cubes
	// objLoader("scene/cube2.obj", _scene.elements[2]->faces);
	// _scene.elements[2]->material = matBlueGlass;
	// _scene.elements.push_back(new Element()); // Cubes
	// objLoader("scene/cube3.obj", _scene.elements[3]->faces);
	// _scene.elements[3]->material = matRedGlass;
	_scene.elementNumber = 2;
}

void Raytracer::computeImage () {
	SDL_Event event;
	bool isRunning = true;
	bool hasFinished = false;

	// Coordinates
	int x = 0;
	int y = 0;
	SDL_Rect box_rect;
	box_rect.w = po.target_size;
	box_rect.h = po.target_size;

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
			x += po.target_size;
			if (x >= po.image_width) {
				x = 0;
				y += po.target_size;
			}
			if (y >= po.image_height) {
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
	SDL_Surface *sshot = SDL_CreateRGBSurface(0, po.image_width, po.image_height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, file_name);
	SDL_FreeSurface(sshot);
}

// > traceZone
//		Compute a particlar square (X, Y) of the image
void Raytracer::traceZone (int X, int Y) {
	// usleep(3000); // TODO

	void *tmp;
	Uint32 *pixels;
	int pitch;

	// Lock
	SDL_LockTexture(_image, NULL, &tmp, &pitch);
	pixels = (Uint32 *)tmp;
	glm::vec4 color_samp;

	int sx = std::min(po.target_size, po.image_width - X);
	int sy = std::min(po.target_size, po.image_height - Y);
	// For all pixels
	for (int i = 0; i < sx; i++) {
		for (int j = 0; j < sy; j++) {
			_antialiaser.resetPixelValue();
			// For all samples
			for (glm::vec2 samp : _antialiaser.getPixelSamplesRepartition()) {
				// Get the ray coming from the camera
				float x = (float)(X + i + samp.x) / (float)po.image_width - 0.5;
				float y = (float)(Y + j + samp.y) / (float)po.image_height - 0.5;
				glm::vec3 ray = _scene.camera.getRay(x, y);

				// TRACE !
				color_samp = tracer(_scene, ray, _scene.camera.getPosition(), po.maxDepth);

				// Add color to sampling process
				_antialiaser.setSampleValue (color_samp);
			}

			// Set pixel color
			color_t color = _antialiaser.getPixelValue() * 255;
			int p = (Y + j) * po.image_width + (X + i);
			pixels[p] = SDL_MapRGBA(_format, (Uint8)color.x, (Uint8)color.y, (Uint8)color.z, (Uint8)color.t);
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
