#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "Scenario.h"
#include "mesh.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"


static TriangleWinding StarMeshWinding = TriangleWinding::CCW;

Mesh deathStar("mesh/deathStar.obj", StarMeshWinding);
Mesh deathStar1("mesh/deathStar1.obj", StarMeshWinding);
Mesh deathStar2("mesh/deathStar2.obj", StarMeshWinding);


void Scenario::Init() {
  px = 200;
  pz = 0;
  py = 0;

  return;
}

void Scenario::RenderAllParts(bool usecolor) const
{
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  glPushMatrix();
  {
    glScalef(90, 90, -90);
    {
      glDisable(GL_COLOR_MATERIAL);
      float specular[4] = {0.f, 0.f, 0.f, 1.f};
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      float diffuse[4] = {.5f, .5f, .5f, 0.1f};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      float ambient[4] = {0.1f, 0.1f, 0.1f, 1.f};
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
      GLfloat mat_shininess[] = { 20.f };
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, ambient);
    }

    glColor3f(1.f, 1.f, 1.f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture::Plate);

    deathStar.RenderNxV(true);
    deathStar1.RenderNxV(true);
    deathStar2.RenderNxV(true);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_COLOR_MATERIAL);
    utils::checkGLError(__FILE__, __LINE__);
  }
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
}

void Scenario::render(const Game &game) const {

  glPushMatrix();
  {
    glTranslatef(px, py, pz);
    glRotatef(game.getGameSeconds(), 1.f, 0.f, 0.f);

    RenderAllParts(true);
  }
  glPopMatrix();

  utils::checkGLError(__FILE__, __LINE__);

  return;
}
