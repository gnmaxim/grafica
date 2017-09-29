#include "Space.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"


void drawSpaceCube(const float size) {
  const float S = size / 2.f;

  /* Per disegnare da entrambi i lati */
  glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT_AND_BACK);

  glBegin(GL_QUADS);

  glNormal3f(0, 0, +1);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(+S, +S, +S);
  glTexCoord2f(1.f, 0.0f);
  glVertex3f(-S, +S, +S);
  glTexCoord2f(1.f, 1.f);
  glVertex3f(-S, -S, +S);
  glTexCoord2f(0.0f, 1.f);
  glVertex3f(+S, -S, +S);

  glNormal3f(0, 0, -1);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(+S, -S, -S);
  glTexCoord2f(1.f, 1.0f);
  glVertex3f(-S, -S, -S);
  glTexCoord2f(1.f, 0.f);
  glVertex3f(-S, +S, -S);
  glTexCoord2f(0.0f, 0.f);
  glVertex3f(+S, +S, -S);

  glNormal3f(0, +1, 0);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(+S, +S, +S);
  glTexCoord2f(1.f, 0.0f);
  glVertex3f(-S, +S, +S);
  glTexCoord2f(1.f, 1.f);
  glVertex3f(-S, +S, -S);
  glTexCoord2f(0.0f, 1.f);
  glVertex3f(+S, +S, -S);

	glNormal3f(  0,-1,0  );
	glTexCoord2f(1.f, 0.f);
	glVertex3f( +S,-S,-S );
	glTexCoord2f(1.f, 1.0f);
	glVertex3f( -S,-S,-S );
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( -S,-S,+S );
	glTexCoord2f(0.0f, 0.f);
	glVertex3f( +S,-S,+S );

  glNormal3f(+1, 0, 0);
  glTexCoord2f(0.0f, 0.f);
  glVertex3f(+S, +S, +S);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(+S, -S, +S);
  glTexCoord2f(1.f, 1.0f);
  glVertex3f(+S, -S, -S);
  glTexCoord2f(1.f, 0.f);
  glVertex3f(+S, +S, -S);

  glNormal3f(-1, 0, 0);
  glTexCoord2f(0.0f, 0.f);
  glVertex3f(-S, +S, -S);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-S, -S, -S);
  glTexCoord2f(1.f, 1.0f);
  glVertex3f(-S, -S, +S);
  glTexCoord2f(1.f, 0.f);
  glVertex3f(-S, +S, +S);

  glEnd();
  utils::checkGLError(__FILE__, __LINE__);
}

void Space::render(const Game& game) const {
  glColor3f(1.f, 1.f, 1.f);
  glDisable(GL_LIGHTING);
  glPushMatrix();
  {
    glBindTexture(GL_TEXTURE_2D, Texture::DeepSpaceBackground);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(0.f, -1.f, 0.f);
    drawSpaceCube(3000);

    glDisable(GL_TEXTURE_2D);
  }
  glPopMatrix();
  glEnable(GL_LIGHTING);
  utils::checkGLError(__FILE__, __LINE__);

	return;
}
