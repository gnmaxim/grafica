#include "SpaceScanner.h"
#include "Colors.h"
#include "ResistanceFleet.h"
#include "Utils.h"
#include "Game.h"


void SpaceScanner::render(const Game& game) const {
	const Point2 BottomLeftCorner{offsetFromLeft, game.screenSize.Y() - offsetFromBottom - spaceScannerSize.Y()};

  glScissor((int)BottomLeftCorner.X(), (int)BottomLeftCorner.Y(), (int)spaceScannerSize.X(), (int)spaceScannerSize.Y());
  glEnable(GL_SCISSOR_TEST);

	/* Disegno del quadrato scanner e tutto ciò che conterrà */
  glPushMatrix();
  {
    glTranslatef(BottomLeftCorner.X() + 55, BottomLeftCorner.Y() + 55, 1.f);
    glScalef(300, 300, 0.f);

    glPointSize(pntSize);
    glPushMatrix();
    {
      glScalef(1.f / Space::playableWidth, 1.f / Space::playableWidth, 1.f);
      glTranslatef(Space::playableWidth / 2.f, Space::playableWidth / 2.f, 0.f);

      if (rotatesWithPlayer) {
				glRotated(-game.sd.getFacing(), 0, 0, 1);
				glRotated(180, 1, 0, 0);
      }

      if (centersOnPlayer) {
        glTranslatef(-game.sd.getPosition().X(), -game.sd.getPosition().Z(), 0.f);

        // Confini del livello
        glPushMatrix();
        {
          glLineWidth(widthBorder);
          glColor3fv(colorSpaceBorder);
          glScalef(-Space::playableWidth / 2.f, -Space::playableWidth / 2.f, 1.f);
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

      /* Disegna tutti i simboli riguardanti giocatore, xwing e artefatti */
      glBegin(GL_POINTS);
      {
        glColor3fv(colorStarDestroyer);
        glVertex2f(game.sd.getPosition().X(), game.sd.getPosition().Z());

        glColor3fv(colorXWing);
        for (const Point3 &enemyPosition : game.resistanceFleet.getXWingPositions()) {
					/* Disegna 5 vertici per ogni nemico che si trova sopra alla nave */
					if (game.sd.getPosition().Y() < enemyPosition.Y()) {
						glVertex2f(enemyPosition.X(), enemyPosition.Z());
						glVertex2f(enemyPosition.X() + 5, enemyPosition.Z());
						glVertex2f(enemyPosition.X() - 5, enemyPosition.Z());
						glVertex2f(enemyPosition.X(), enemyPosition.Z() + 5);
						glVertex2f(enemyPosition.X(), enemyPosition.Z() - 5);
					}
					/* Se sono sotto ne disegna solo 3 */
					if (game.sd.getPosition().Y() > enemyPosition.Y()) {
						glVertex2f(enemyPosition.X(), enemyPosition.Z());
						glVertex2f(enemyPosition.X() + 5, enemyPosition.Z());
						glVertex2f(enemyPosition.X() - 5, enemyPosition.Z());
					}
					/* Se allo stesso livello diventa un punto */
					else {
						glVertex2f(enemyPosition.X(), enemyPosition.Z());
					}
        }

        glColor3fv(colorArtefact);
        for (const Point3 &junkPosition : game.techCreator.getSecretTechPositions(game))
          glVertex2f(junkPosition.X(), junkPosition.Z());
      }
      glEnd();
    }
    glPopMatrix();
  }
  glPopMatrix();
  glColor3f(1.f,1.f,1.f);

  glDisable(GL_SCISSOR_TEST);

  utils::checkGLError(__FILE__, __LINE__);
}
