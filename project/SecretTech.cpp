#include "Game.h"
#include "SecretTech.h"
#include "Textures.h"
#include "Utils.h"
#include "mesh.h"


/* Caricamento della Mesh */
Mesh tech((char *) "mesh/tech.obj");

SecretTech::SecretTech(const Point3 &initialPosition, const float spawnTime, const Vector3 &rotation)
        : initialPosition(initialPosition), spawnTime(spawnTime), rotation(rotation) {}

Point3 SecretTech::getPosition(const float playedSeconds) const {
    return initialPosition;
}

void SecretTech::render(const float playedSeconds, bool renderShadows) const {
	const Point3 currentPosition = getPosition(playedSeconds);
	static constexpr float techSize = 2.5f;
    glPushMatrix();
    {
      glTranslate(currentPosition);
      glScalef(techSize, techSize, techSize);

      /* Rotazioni intorno ad ogni asse */
      glRotatef(rotation.X(), 1.f, 0.f, 0.f);
      glRotatef(rotation.Y(), 0.f, 1.f, 0.f);
      glRotatef(rotation.Z(), 0.f, 0.f, 1.f);

      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      {
        float specular[4] = {10.f, 10.f, 10.f, 1.f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        float diffuse[4] = {10.f, 10.f, 10.f, 1.f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        float ambient[4] = {200.f, 200.1f, 200.1f, 1.f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, 128.f);
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, ambient);
      }

      glBindTexture(GL_TEXTURE_2D, (GLuint)Texture::Artefact);
      glEnable(GL_TEXTURE_2D);

      tech.RenderNxV(true);

      glDisable(GL_TEXTURE_2D);
      glColor4f(1.f, 1.f, 1.f, 1.f);
    }

    glPopMatrix();
    if (currentPosition.X() > 1.f) {
      // Render Shadow
      if (renderShadows) {
        glPushMatrix();
        {
          glTranslatef(190, currentPosition.Y(), currentPosition.Z());

          glScalef(techSize * (1 - (currentPosition.X() / 55.f)),
                   techSize * (1 - (currentPosition.X() / 55.f)),
                   techSize * (1 - (currentPosition.X() / 55.f)));
          glScalef(0.f, 1.01f, 1.01f);

          /* Rotazioni intorno agli assi */
          glRotatef(rotation.X(), 1.f, 0.f, 0.f);
          glRotatef(rotation.Y(), 0.f, 1.f, 0.f);
          glRotatef(rotation.Z() - 180, 0.f, 0.f, 1.f);

          /* Colore nero dellìombra */
          glColor3f(0.f, 0.f, 0.f);

          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

          tech.RenderNxV(false);

          glDisable(GL_BLEND);
          glColor4f(1.f, 1.f, 1.f, 1.f);
        }
        glPopMatrix();
      }
    }
    utils::checkGLError(__FILE__, __LINE__);

    return;
}
