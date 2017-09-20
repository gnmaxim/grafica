#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "SpaceObject.h"
#include "mesh.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"


static TriangleWinding StarMeshWinding = TriangleWinding::CCW;

// Variabili di tipo mesh
Mesh deathStar("../assets/obj/deathStar.obj", StarMeshWinding);
Mesh deathStar1("../assets/obj/deathStar1.obj", StarMeshWinding);

// Per usare l'ambiente mapping
extern bool useWireframe;


void SpaceObject::Init() {
  px = 200;
  pz = 0;
  py = 0;

  return;
}

void SpaceObject::RenderAllParts(bool usecolor) const
{

 glEnable ( GL_COLOR_MATERIAL ) ;
  glEnable(GL_LIGHTING);

  glPushMatrix();
  {
    static constexpr float StarDestroyerScale = 79.f;
    const Vector3 ObjectScale{StarDestroyerScale, StarDestroyerScale, -StarDestroyerScale};
    glScalef(ObjectScale.X(), ObjectScale.Y(), ObjectScale.Z());
    static constexpr bool useTexCoords = true;
    utils::checkGLError(__FILE__, __LINE__);
    if (usecolor)
    {
      {
        glDisable(GL_COLOR_MATERIAL);
        float specular[4] = {100.1f, 100.1f, 500.1f, 1.f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        float diffuse[4] = {.5f, .5f, .5f, 0.1f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        float ambient[4] = {0.1f, 0.1f, 0.1f, 1.f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, 10.f);
        GLfloat mat_shininess[] = { 50.f };
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      }
      utils::checkGLError(__FILE__, __LINE__);
      glColor3f(1.f, 1.f, 1.f);


      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, Texture::Plate);
      deathStar.RenderNxV(useTexCoords); // rendering delle mesh hull usando normali per vertice
      deathStar1.RenderNxV(useTexCoords); // rendering delle mesh hull usando normali per vertice
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);

      glDisable(GL_COLOR_MATERIAL);
      utils::checkGLError(__FILE__, __LINE__);
    }
    else
    {
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      deathStar.RenderNxV(useTexCoords); // rendering delle mesh hull usando normali per vertice
    }
    utils::checkGLError(__FILE__, __LINE__);
  }
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
}


// disegna a schermo
void SpaceObject::render(const Game &game) const {
  // sono nello spazio mondo
  glPushMatrix();
  {
    glTranslatef(px, py, pz);
    glRotatef(0, 0.f, 0.f, 0.f);

    RenderAllParts(/*usecolor*/ true);
  }
  glPopMatrix();

  utils::checkGLError(__FILE__, __LINE__);

  return;
}
