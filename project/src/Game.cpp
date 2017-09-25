#include "Game.h"
#include <GL/glu.h>
#include "Utils.h"
#include "Colors.h"
#include "SpaceObject.h"

#include <iostream>

Point2 Game::defaultScreenSize = Point2{1920.f, 1080.f};

Game::Game() {}

void Game::init() {
  textRenderer.initialize();
  map.initialize();
  setState(GameState::Playing);
  screenSize  = defaultScreenSize;

  return;
}

int Game::incrementScore(int inc) {
  return score += inc;
}

void Game::doStep() {
	const float deltaSeconds = utils::get_elapsed_seconds() - exPlayedSeconds;
	exPlayedSeconds = utils::get_elapsed_seconds();

	if (gameState == GameState::Playing) {
		playedSeconds += deltaSeconds;
		sD.DoStep(*this);
		techCreator.doStep(*this);
		enemyManager.doStep(*this);
	}

  return;
}

GameState Game::getCurrentState() const {
  return gameState;
}

void Game::setState(const GameState newState) {
  if (gameState == newState) return;
  gameState = newState;
  switch (gameState) {
    case GameState::GameOver: {
      menuManager.setMenuState(MenuState::gameOver);
      break;
    }
  }

  return;
}

void Game::render() {
  glLineWidth(3.f);

  /* Setting viewport */
  glViewport(0, 0,  (GLsizei)screenSize.X(),  (GLsizei)screenSize.Y());

  /* Impostazione matrice proiezione */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(69, //fovy,
      screenSize.X() / screenSize.Y(),//aspect Y/X,
      0.2,//distanza del NEAR CLIPPING PLANE in coordinate vista
      1000  /*distanza del FAR CLIPPING PLANE in coordinate vista*/ );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // riempe tutto lo screen buffer di pixel color sfondo
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setCamera();

  star.render(*this);
  glDisable(GL_COLOR_MATERIAL);
  map.render(*this);
  sD.render(*this);
  spaceObject.render(*this);
  techCreator.renderSecretTech(*this);
  enemyManager.renderEnemies(*this);

  // attendiamo la fine della rasterizzazione di
  // tutte le primitive mandate
  glFinish();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  // Passiamo al disegno 2D
  utils::setCoordToPixel(screenSize);

  /* Segnale per l'Ultramotore */
  if (sD.getTurboCharge(*this) == 1) {
    const Point2 textPosition{screenSize.X() - 450, 60};
    textRenderer.render(Colors::Red(), Colors::Black(), "ULTRAENGINE", textPosition, solid, mid, false);
  }

  minimap.render(*this);

  {
    /* Punteggio */
    std::stringstream scoreStream;
    scoreStream << "IMPERIAL TECH";
    Point2 textPosition = {70, 60};
    std::string scoreString = scoreStream.str();
    textRenderer.render(Colors::Red(), Colors::Black(), scoreString.c_str(), textPosition, solid, mid, false);

    std::stringstream scoreStreamN;
    scoreStreamN << getScore();
    Point2 scorePosition = {70, 150};
    scoreString = scoreStreamN.str();
    textRenderer.render(Colors::Red(), Colors::Black(), scoreString.c_str(), scorePosition, solid, ultra,
    false);
  }
  {
    /* Tempo */
    std::stringstream timeStream;
    std::stringstream timeLeftStream;
    float playedSecondsNow = getGameSeconds();
    int timeLeft = (int)maxTime - (int)playedSecondsNow + 3;
    if (timeLeft == 0)
    {
      setState(GameState::GameOver);
    }
    if (timeLeft < 60)
    {
      if (timeLeft < 10)
      {
        timeLeftStream << "00:0" << timeLeft;
      }
      else
      {
        timeLeftStream << "00:" << timeLeft;
      }
    }
    else
    {
      if (timeLeft-60 < 10)
      {
        timeLeftStream << "01:0" << timeLeft - 60;
      }
      else
      {
        timeLeftStream << "01:" << timeLeft - 60;
      }
    }

    Point2 textPosition = {screenSize.X() / 2 - 65, (screenSize.Y() - 120)};
    std::string timeString = timeLeftStream.str();
    if (playedSecondsNow >= 3) {
      textRenderer.render(Colors::Blue(), Colors::Black(), timeString.c_str(), textPosition, solid, mid, false);
      textPosition = {screenSize.X() / 2 - 40, (screenSize.Y() - 50)};
      textRenderer.render(Colors::Red(), Colors::Black(), "FLOTTA", textPosition, solid, small, false);
    }
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  switch (gameState)
  {
    case GameState::GameOver :
    {
      menuManager.render(*this);
    }
    case GameState::Paused :
    {
      menuManager.render(*this);
    }
  }
  glColor3f(1.f, 1.f, 1.f);

  glFinish();
  utils::checkGLError(__FILE__, __LINE__);

  return;
}

void Game::setCamera() const {

    double px = sD.getPosition().X();
    double py = sD.getPosition().Y();
    double pz = sD.getPosition().Z();

    double angle = sD.getFacing();
    double cosf = cos(angle * M_PI / 180.0);
    double sinf = sin(angle * M_PI / 180.0);

    double angley = sD.getFacingY();
    double cosfy = cos(angley * M_PI / 180.0);
    double sinfy = sin(angley * M_PI / 180.0);

    double camd, camh, ex, ey, ez, cx, cy, cz;
    double cosff, sinff;

// controllo la posizione della camera a seconda dell'opzione selezionata
    switch (cameraState) {
        case CameraPos::BACK_CAMERA:
            camd = 1;
            camh = 0.35;
            ex = px + camd * sinf;
            ey = py - sinfy + camh;
            ez = pz + camd * cosf;
            cx = px - camd * sinf;
            cy = py + sinfy + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);

            break;
        case CameraPos::LATERAL_CAM:
            camd = 6.0;
            camh = 1.15;
            angle = sD.getFacing() - 40.0;
            cosff = cos(angle*M_PI/180.0);
            sinff = sin(angle*M_PI/180.0);
            ex = px + camd*sinff;
            ey = py + camh;
            ez = pz + camd*cosff;
            cx = px - camd*sinf;
            cy = py + camh;
            cz = pz - camd*cosf;
            gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
            break;
        case CameraPos::CAMERA_TOP_FIXED:
            camd = 2.5;
            camh = 1.0;
            ex = px + camd * sinf;
            ey = py + camh;
            ez = pz + camd * cosf;
            cx = px - camd * sinf;
            cy = py + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey + 5, ez, cx, cy, cz, 0.0, 1.0, 0.0);
            break;
        case CameraPos::TOP_CAM:
            camd = 0.5;
            camh = 0.55;
            cosff = cos(angle * M_PI / 180.0);
            sinff = sin(angle * M_PI / 180.0);
            ex = px + camd * sinff;
            ey = py + camh;
            ez = pz + camd * cosff;
            cx = px - camd * sinf;
            cy = py + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);
            break;
    }
}

void Game::switchCamera() {
    cameraState = (CameraPos::Type) (((int) cameraState + 1) % (int) CameraPos::CAMERA_MAX);
}

void Game::EatKey(int keycode, bool pressed, bool joystick) {
    inputManager.EatKey(keycode, pressed, joystick, *this);
}

void Game::EatJoyAxis(int axisIndex, int axisValue) {
    inputManager.EatJoyAxis(axisIndex, axisValue);
}
