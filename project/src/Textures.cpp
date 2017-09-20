#include "Textures.h"

#include <GL/glu.h>
#include "Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>

GLuint Texture::Font = 0;
GLuint Texture::WaterBackground = 0;
GLuint Texture::Star = 0;
GLuint Texture::Plate = 0;
GLuint Texture::SpecularPlate = 0;
GLuint Texture::StarDestroyerHull = 0;
GLuint Texture::StarDestroyerMotor = 0;
GLuint Texture::StarDestroyerConningTower = 0;
GLuint Texture::Barrels = 0;
GLuint Texture::Jellyfish = 0;
GLuint Texture::Sand = 0;
GLuint Texture::Shark = 0;
GLuint Texture::Sonar = 0;
GLuint Texture::Caustics = 0;
GLuint Texture::PersonalPicture = 0;
GLuint Texture::Seaweed = 0;
GLuint Texture::Rock = 0;


bool Texture::Load(/*out*/ GLuint &textureID, const char *filename) {
	assert(filename);
	assert(!glIsTexture(textureID));
	SDL_Surface * const s = IMG_Load(filename);
	if (!s) {
        printf("Texture not found: %s", filename);
        return false;
    }
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	gluBuild2DMipmaps(
		GL_TEXTURE_2D,
		GL_RGB,
		s->w, s->h,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		s->pixels);
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);

	SDL_FreeSurface(s);
	utils::checkGLError(__FILE__, __LINE__);
	return true;
}

bool Texture::LoadAllTextures()
{
	if (!Load(WaterBackground, "../assets/textures/underwater.jpg")) return false;
	if (!Load(Star,  "../assets/textures/star.jpg")) return false;
	if (!Load(Plate,  "../assets/textures/metal3.jpg")) return false;
	// if (!Load(SpecularPlate,  "../assets/textures/specular-plate.png")) return false;
	if (!Load(StarDestroyerHull,  "../assets/textures/corpo.jpg")) return false;
	if (!Load(StarDestroyerMotor, "../assets/textures/motor.jpg")) return false;
	if (!Load(StarDestroyerConningTower, "../assets/textures/box.jpg")) return false;
	if (!Load(Barrels, "../assets/textures/barrels.jpg")) return false;
	if (!Load(Jellyfish, "../assets/textures/jellyfish.png")) return false;
	if (!Load(Sand, "../assets/textures/space.jpg")) return false;
	if (!Load(Shark, "../assets/textures/shark.jpg")) return false;
	if (!Load(Sonar, "../assets/textures/sonar2.jpg")) return false;
	if (!Load(Caustics, "../assets/textures/Caustics1.png")) return false;
	if (!Load(PersonalPicture, "../assets/textures/fotopersonale.jpg")) return false;
	if (!Load(Seaweed, "../assets/textures/seaweed.png")) return false;
	if (!Load(Rock, "../assets/textures/rock.jpg")) return false;

	return true;
}
