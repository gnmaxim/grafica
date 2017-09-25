#include "Game.h"
#include "SecretTech.h"
#include "Textures.h"
#include "Utils.h"
#include "mesh.h"

// Variabili di tipo mesh
Mesh barrels((char *) "../assets/obj/barrels.obj");

SecretTech::SecretTech(const Point3 &initialPosition, const float spawnTime, const float fallSpeed, const Vector3 &rotation)
        : initialPosition(initialPosition), spawnTime(spawnTime), fallSpeed(fallSpeed), rotation(rotation) {}

Point3 SecretTech::getPosition(const float playedSeconds) const {
    const float lifeTime = playedSeconds - spawnTime;
    return initialPosition + -Vector3::GetUpVector() * fallSpeed * lifeTime;
}

void SecretTech::render(const float playedSeconds, bool renderShadows) const {
	const Point3 currentPosition = getPosition(playedSeconds);
	static constexpr float junkScale = 2.5f;
    glPushMatrix();
    {
      glTranslate(currentPosition);
      glScalef(junkScale,junkScale,junkScale);
      glRotatef(rotation.X(), 1.f, 0.f, 0.f); // rotazione intorno all'asse X
      glRotatef(rotation.Y(), 0.f, 1.f, 0.f); // rotazione intorno all'asse Y
      glRotatef(rotation.Z(), 0.f, 0.f, 1.f); // rotazione intorno all'asse Z

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

      glBindTexture(GL_TEXTURE_2D, (GLuint)Texture::Barrels);
      glEnable(GL_TEXTURE_2D);

      barrels.RenderNxV(true);

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

                glScalef(junkScale * (1 - (currentPosition.X() / 50.f)),
                         junkScale * (1 - (currentPosition.X() / 50.f)),
                         junkScale * (1 - (currentPosition.X() / /* spawnHeight */50.f)));
                glScalef(0.f, 1.01f, 1.01f);

                glRotatef(rotation.X(), 1.f, 0.f, 0.f); // rotazione intorno all'asse X
                glRotatef(rotation.Y(), 0.f, 1.f, 0.f); // rotazione intorno all'asse Y
                glRotatef(rotation.Z() - 180, 0.f, 0.f, 1.f); // rotazione intorno all'asse Z

                glColor3f(12.f / 255.f, 12.f / 255.f, 12.f / 255.f); // colore fisso
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                barrels.RenderNxV(/*usTexCoords*/ false);
                glDisable(GL_BLEND);
                glColor4f(1.f, 1.f, 1.f, 1.f);
            }
            glPopMatrix();
        }
    }
    utils::checkGLError(__FILE__, __LINE__);
}
