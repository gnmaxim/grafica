#include "Textures.h"

#include <GL/glu.h>
#include "Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>


GLuint Texture::Font = 0;
GLuint Texture::DeepSpaceBackground = 0;
GLuint Texture::Star = 0;
GLuint Texture::Plate = 0;
GLuint Texture::StarDestroyerHull = 0;
GLuint Texture::Artefact = 0;
GLuint Texture::XWing = 0;
GLuint Texture::mg = 0;


bool Texture::Load(GLuint &textureID, const char *texturename) {
	SDL_Surface * const s = IMG_Load(texturename);
	bool success = true;

	if (!s) {
    printf("Texture non trovata: %s", texturename);
    success = false;
  }
	else {
		/* Generazione nome per texture */
		glGenTextures(1, &textureID);

		/* Binding del nome appena generato all'unità di texture attiva */
		glBindTexture(GL_TEXTURE_2D, textureID);

		/* Costruisci MIPMAP, mappa di texture a risoluzioni decrescenti a potenze di 2 */
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, s->w, s->h, GL_RGB, GL_UNSIGNED_BYTE, s->pixels);

		/* GL_TEXTURE_MIN_FILTER specifica funzione di minimizzazione nel caso
		 		il pixel texturato sia maggiore del singolo elemento da texturare.

			 GL_TEXTURE_MAG_FILTER, quando il pixel texturato sia invece minore. */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		SDL_FreeSurface(s);

		utils::checkGLError(__FILE__, __LINE__);
	}

	return true;
}

bool Texture::LoadAllTextures()
{
	if (!Load(DeepSpaceBackground, "texture/deepspace.jpg")) return false;
	if (!Load(Star, "texture/star.jpg")) return false;
	if (!Load(Plate, "texture/metal.jpg")) return false;
	if (!Load(StarDestroyerHull, "texture/sdsurface.jpg")) return false;
	if (!Load(Artefact, "texture/artefact.jpg")) return false;
	if (!Load(XWing, "texture/xwing.jpg")) return false;
	if (!Load(mg, "texture/mg.jpg")) return false;

	return true;
}
