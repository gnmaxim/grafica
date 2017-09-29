#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "Star.h"
#include "mesh.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"


static TriangleWinding StarMeshWinding = TriangleWinding::CCW;


/* Caricamento mesh sferica */
Mesh shull("mesh/star.obj", StarMeshWinding);

void Star::Init() {
  px = -1400;
  pz = 0;
  py = 0;

  return;
}

void Star::RenderAllParts(bool usecolor, const Game& game) const
{
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  /* Luce del sole */
  {
    GLfloat lightColor[4] = {210.f / 255.f, 210.f / 255.f, 255.f / 255.f, 1.0f};
    float direction[4] = {px, py, pz, 1.f};
    glLightfv(GL_LIGHT0, GL_POSITION, direction);

    float ambient[4] = {0.5f, 0.5f, 1.0f, 1.f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    float specular[4] = {0.9f, 0.9f, 0.9f, 1.f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
  }

  float emission[4] = {0.9f, 0.9f, 1.f, 1.f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
  glPushMatrix();
  {
    glScalef(100, 100, -100);

    {
      glDisable(GL_COLOR_MATERIAL);
      float specular[4] = {1.f, 1.f, 1.f, 1.f};
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      float diffuse[4] = {1.f, 1.f, 1.f, 1.f};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      float ambient[4] = {1.0f, 1.0f, 1.0f, 1.f};
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
      glMaterialf(GL_FRONT, GL_SHININESS, 128.f);
      glShadeModel(GL_SMOOTH);
      GLfloat mat_shininess[] = { 128 };
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    }

    glColor3f(1.f, 1.f, 1.f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture::Star);
    /* Rendering usando normali per verice */
    shull.RenderNxV(true);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_COLOR_MATERIAL);

    utils::checkGLError(__FILE__, __LINE__);
  }
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
}


void Star::render(const Game &game) const {
  glPushMatrix();
  {
    glTranslatef(px, py, pz);
    RenderAllParts(true, game);
  }
  glPopMatrix();

  utils::checkGLError(__FILE__, __LINE__);

  return;
}
