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

// Variabili di tipo mesh
Mesh shull("../assets/star/star.obj", StarMeshWinding);

// Per usare l'ambiente mapping
extern bool useWireframe;


void Star::Init() {
  px = -500;
  pz = 0;
  py = 0;

  return;
}


void Star::RenderAllParts(bool usecolor, const Game& game) const
{

 glEnable ( GL_COLOR_MATERIAL ) ;
  glEnable(GL_LIGHTING);

  // Luce del sole
  {
    GLfloat lightColor[4] = {191.f / 255.f, 191.f / 255.f, 255.f / 255.f, 1.0f};
    float direction[4] = {px, py, pz, 1.f}; // ultima comp=0 => luce direzionale
    glLightfv(GL_LIGHT0, GL_POSITION, direction);

    float ambient[4] = {1.0f, 1.0f, 2.0f, 1.f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor );
    float specular[4] = {0.f, 0.f, 0.f, 0.f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
  }


  //glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
  float emission[4] = {100.f, 100.f, 100.f, 100.f};
  glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission);
  glPushMatrix();
  {
    static constexpr float StarDestroyerScale = 40.f;
    const Vector3 ObjectScale{StarDestroyerScale, StarDestroyerScale, -StarDestroyerScale};
    glScalef(ObjectScale.X(), ObjectScale.Y(), ObjectScale.Z());
    static constexpr bool useTexCoords = true;
    utils::checkGLError(__FILE__, __LINE__);
    if (usecolor)
    {
      {
        glDisable(GL_COLOR_MATERIAL);
        float specular[4] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        float diffuse[4] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        float ambient[4] = {0.0f, 0.0f, 0.0f, 1.f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        //glMaterialf(GL_FRONT, GL_SHININESS, 128.f);
        glShadeModel (GL_SMOOTH);
        GLfloat mat_shininess[] = { 128 };
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      }
      utils::checkGLError(__FILE__, __LINE__);
      glColor3f(1.f, 1.f, 1.f);


      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, Texture::Star);
      shull.RenderNxV(useTexCoords); // rendering delle mesh hull usando normali per vertice
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);

      glDisable(GL_COLOR_MATERIAL);
      utils::checkGLError(__FILE__, __LINE__);
    }
    else
    {
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      shull.RenderNxV(useTexCoords); // rendering delle mesh hull usando normali per vertice
    }
    utils::checkGLError(__FILE__, __LINE__);
  }
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
}


// disegna a schermo
void Star::render(const Game &game) const {
  // sono nello spazio mondo
  glPushMatrix();
  {
    glTranslatef(px, py, pz);
    // glRotatef(game.getGameSeconds(), 0.f, 1.f, 0.f);

    RenderAllParts(/*usecolor*/ true, game);
  }
  glPopMatrix();

  utils::checkGLError(__FILE__, __LINE__);

  return;
}
