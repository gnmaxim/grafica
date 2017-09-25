#include "Minimap.h"
#include "Colors.h"
#include "EnemyManager.h"
#include "Utils.h"
#include "Game.h"

void Minimap::render(
	const Game& game) const {
	const Point2 BottomLeftCorner{offsetFromLeft,
                                game.screenSize.Y() - offsetFromBottom - minimapSize.Y()};

  // Disegnamo solo sulla minimappa
  glScissor((int)BottomLeftCorner.X(), (int)BottomLeftCorner.Y(),
            (int)minimapSize.X(), (int)minimapSize.Y());
  glEnable(GL_SCISSOR_TEST);

  // Disegno un quadrato su cui verrà applicata la texture del sonar
  glPushMatrix();
  {

    glTranslatef(BottomLeftCorner.X() + 55, BottomLeftCorner.Y() + 55, 1.f);
    glScalef(300, 300, 0.f);

    glPointSize(sonarPointSize);
    glPushMatrix();
    {

      glScalef(1.f / Space::width, 1.f / Space::width, 1.f);
      glTranslatef(Space::width / 2.f, Space::width / 2.f, 0.f);

      if (rotatesWithPlayer) {
				glRotated(-game.sD.getFacing(), 0, 0, 1);
				glRotated(180, 1, 0, 0);
      }

      if (centersOnPlayer) {
        glTranslatef(-game.sD.getPosition().X(), -game.sD.getPosition().Z(), 0.f);

        // Confini del livello
        glPushMatrix();
        {
          glLineWidth(mapBoundsLineWidth);
          glColor3fv(mapBoundsColor);
          glScalef(-Space::width / 2.f, -Space::width / 2.f, 1.f);
          glBegin(GL_LINE_LOOP);
          {
            glVertex2f(-1.f, -1.f);
            glVertex2f(1.f, -1.f);
            glVertex2f(1.f, 1.f);
            glVertex2f(-1.f, 1.f);
          }
          glEnd();
        }
        glPopMatrix();
      }

      // Draw minimap dots
      glBegin(GL_POINTS);
      {
        // Draw player
        glColor3fv(playercolor);
        glVertex2f(game.sD.getPosition().X(), game.sD.getPosition().Z());

        // Draw enemies
        glColor3fv(enemyColor);
        for (const Point3 &enemyPosition : game.enemyManager.getEnemyPositions()) {
					/* Disegna 5 vertici per ogni nemico che si trova sopra alla nave */
					if (game.sD.getPosition().Y() < enemyPosition.Y()) {
						glVertex2f(enemyPosition.X(), enemyPosition.Z());
						glVertex2f(enemyPosition.X() + 5, enemyPosition.Z());
						glVertex2f(enemyPosition.X() - 5, enemyPosition.Z());
						glVertex2f(enemyPosition.X(), enemyPosition.Z() + 5);
						glVertex2f(enemyPosition.X(), enemyPosition.Z() - 5);
					}
					if (game.sD.getPosition().Y() > enemyPosition.Y()) {
						glVertex2f(enemyPosition.X(), enemyPosition.Z());
						glVertex2f(enemyPosition.X() + 5, enemyPosition.Z());
						glVertex2f(enemyPosition.X() - 5, enemyPosition.Z());
					}
					else {
						glVertex2f(enemyPosition.X(), enemyPosition.Z());
					}
        }

        // Draw junk
        glColor3fv(junkColor);
        for (const Point3 &junkPosition : game.techCreator.getSecretTechPositions(game)) {
          glVertex2f(junkPosition.X(), junkPosition.Z());
        }
      }
      glEnd();

      // Cerchi sonar
      glPushMatrix();
      {
        glLineWidth(sonarCircleLineWidth);
        glColor3fv(sonarCircleColor);
        const Point3 sDPosition = game.sD.getPosition();
        glTranslatef(sDPosition.X(), sDPosition.Z(), 0.f);
        for (int circleIndex = 0; circleIndex < sonarCircleCount;
             circleIndex++) {
          const float circleRadius = circleIndex * sonarCircleRadiusIncrement;
          glBegin(GL_LINE_LOOP);
          {
            const float angleIncrement =
                2.f * (float)M_PI / (float)sonarCircleLineSections;
            for (float angleRadians = 0.f; angleRadians < 2.f * M_PI;
                 angleRadians += angleIncrement) {
              // Equazione di un cerchio: x = r*sin(angolo), y = r*cos(angolo)
              // dobbiamo fare i calcoli noi perche glTranslate e glScale non
              // sono permessi dentro glBegin/glEnd
              glVertex2f(circleRadius * sinf(angleRadians),
                         circleRadius * cosf(angleRadians));
            }
          }
          glEnd();
        }
      }
      glPopMatrix();
    }
    glPopMatrix();
  }
  glPopMatrix();
  glColor3f(1.f,1.f,1.f);

  glDisable(GL_SCISSOR_TEST);

  utils::checkGLError(__FILE__, __LINE__);
}
