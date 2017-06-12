# Raytracing Software
### ENSTA IN104

**C++ parallelized implementation of a raytracing software.**

## Implementation

This software supports shadows, reflection and refraction. It features an almost real-time preview of the output image, which is saved as `.png` afterwards.

The 3D scene can be imported as `.obj` and materials can then be defined and assigned to the different elements. The Camera can be placed anywhere in the scene.

Antialiasing can be applied to the image.

## Libraries

GLM, SDL2 and OpenMP required.
The following packages thereby need to be present on your computer :

`libglm-dev`
`libsdl2-dev`
`libsdl2-image-dev`
`libsdl2-ttf-dev`
`libsdl2-gfx-dev`
`libopenmpi-dev`
`openmpi-bin`
