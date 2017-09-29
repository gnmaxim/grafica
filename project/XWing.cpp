#include "Physics.h"
#include "Game.h"
#include "Utils.h"
#include "XWing.h"


XWing::XWing(const Point3 &position, float speed, float scale, const Vector3 &rotation, bool playerOriented, const Mesh *const enemyMesh, GLuint texture)
        : position(position), speed(speed), playerOriented(playerOriented), enemyMesh(enemyMesh), scale(scale), textureID(texture) {}

void XWing::render(const Game &game) const {
    glPushMatrix();
    {
      /* Rendering degli X-Wing */
      glEnable(GL_LIGHTING);

      glTranslate(position);
      glScalef(scale, scale, scale);

      /* Rotazione intorno agli assi */
      glRotatef(rotation.X(), 1.f, 0.f, 0.f);
      glRotatef(rotation.Y(), 0.f, 1.f, 0.f);
      glRotatef(rotation.Z(), 0.f, 0.f, 1.f);

      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      {
        float specular[4] = {10.f, 10.f, 10.f, 1.f};
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				float diffuse[4] = {1.f, 1.f, 1.f, 1.f};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				float ambient[4] = {20.f, 20.1f, 20.1f, 1.f};
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				glMaterialf(GL_FRONT, GL_SHININESS, 128.f);
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, ambient);
      }

      glBindTexture(GL_TEXTURE_2D, textureID);
      glEnable(GL_TEXTURE_2D);
      assert(enemyMesh);
      enemyMesh->RenderNxV(true);
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_LIGHTING);
    }
    glPopMatrix();

    if (game.renderShadows) {
      glPushMatrix();
      {
        const Point3 sdCoord = game.sd.getPosition();

        /* Ombra del nemico sul piano verticale */
        glTranslatef(190, position.Y(), position.Z());

        /* La scala viene moltiplicata per un parametro che dipende dalla posizione del nemico vero e proprio, per fare in modo
         che più si avvicina al terreno più l'ombre si ingrandisce */
        if (position.X() > 1)
          glScalef(scale * (1 - (position.X() / 40.f)), scale * (1 - (position.X() / 40.f)), scale * (1 - (position.X() / 40.f)));
        else
          glScalef(0.f, 0.f, 0.f);

        /* Appiattimento su X e lieve ingrandimento altrove */
        glScalef(0.f, 1.01f, 1.01f);

        /* Rotazione sincrona dell'ombra rispetto all'oggetto */
        glRotatef(rotation.X(), 1.f, 0.f, 0.f);
        glRotatef(rotation.Y(), 0.f, 1.f, 0.f);
        glRotatef(rotation.Z() -180, 0.f, 0.f, 1.f);

        /* Nero dell'ombra */
        glColor3f(0.f, 0.f, 0.f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        enemyMesh->RenderNxV(false);

        glDisable(GL_BLEND);
        glColor3f(1.f, 1.f, 1.f);
      }
      glPopMatrix();
    }
    utils::checkGLError(__FILE__, __LINE__);

    return;
}

void XWing::doStep(const StarDestroyer &sd) {
    /* Il nemico segue la DS, quindi è necessario ricalcolare le posizioni */
    const Point3 sdCoord = sd.getPosition();

    /* Calcolo del vettore direzione dall'X-Wing alla nave */
    const Vector3 directionVectorToShip = sdCoord - position;
    const float sdToXWingDistance = directionVectorToShip.modulo();

    /* Calcolo della distanza che il nemico percorre in un step di fisica, senza che non si vada troppo lontano */
    const float distanceToTravel = fminf(speed * Physics::stepSeconds, sdToXWingDistance);

    /* Calcolo il vettore che descrive lo spostamento del nemico in uno step */
    const Vector3 travelVector = directionVectorToShip.NormalizeSafe() * distanceToTravel;
    position = position + travelVector;

    /* E la rotazione */
    if (playerOriented) {
      const Vector3 directionVectorToShip = sd.getPosition() - position;

      /* uso atan2f per calcolare l'angolo con l'asse Y*/
      const float facingRadiansY = atan2f(directionVectorToShip.X(), directionVectorToShip.Z());
      const float facingDegreesY = utils::radiansToDegrees(facingRadiansY); // openGL usa i gradi
      rotation = Vector3{0.f, facingDegreesY, 0.f};
    }

    return;
}
