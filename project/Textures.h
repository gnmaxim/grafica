#pragma once
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


namespace Texture
{
  extern GLuint Font;
  extern GLuint Star;
  extern GLuint Plate;
  extern GLuint DeepSpaceBackground;
  extern GLuint StarDestroyerHull;
  extern GLuint Artefact;
  extern GLuint XWing;
  extern GLuint mg;

	bool Load(/*out*/ GLuint &textureID, const char *filename); // returns true for success
	bool LoadAllTextures(); // returns true for success
}
