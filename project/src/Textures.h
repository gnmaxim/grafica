#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Texture
{
  extern GLuint Font;
  extern GLuint Star;
  extern GLuint Plate;
  extern GLuint SpecularPlate;
  extern GLuint WaterBackground;
  extern GLuint StarDestroyerHull;
  extern GLuint StarDestroyerMotor;
  extern GLuint StarDestroyerConningTower;
  extern GLuint Barrels;
  extern GLuint Jellyfish;
  extern GLuint Sand;
  extern GLuint Shark;
  extern GLuint Sonar;
  extern GLuint Caustics;
  extern GLuint PersonalPicture;
  extern GLuint Seaweed;
  extern GLuint Rock;

	bool Load(/*out*/ GLuint &textureID, const char *filename); // returns true for success
	bool LoadAllTextures(); // returns true for success
}
