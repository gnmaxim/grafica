#include "Game.h"
#include <GL/glu.h>
#include "Utils.h"
#include "Colors.h"
#include "Scenario.h"


#include <iostream>

// Point2 Game::RESOLUTION = Point2{1920.f, 1080.f};
// Point2 Game::RESOLUTION = Point2{1024.f, 768.f};
Point2 Game::RESOLUTION = Point2{1366.f, 768.f};


Game::Game() {}

void Game::init() {
  textRenderer.initialize();
  setState(GameStatus::SMASHING_XWING);
  screenSize  = RESOLUTION;

  return;
}

int Game::addPoints(int points) {
  return score += points;
}

void Game::doStep() {
	const float deltaSeconds = utils::get_elapsed_seconds() - exPlayedSeconds;
	exPlayedSeconds = utils::get_elapsed_seconds();

	if (gameStatus == GameStatus::SMASHING_XWING) {
		playedSeconds += deltaSeconds;
		sd.DoStep(*this);
		techCreator.doStep(*this);
		resistanceFleet.doStep(*this);
	}

  return;
}

GameStatus Game::getCurrentState() const {
  return gameStatus;
}

void Game::setState(const GameStatus newState) {
  if (gameStatus == newState) return;
  gameStatus = newState;
  switch (gameStatus) {
    case GameStatus::GAME_OVER: {
      menu.setMenuState(MenuState::gameOver);
      break;
    }
  }

  return;
}

void Game::render() {
  glLineWidth(3.f);
  /* Impostazione viewport */
  glViewport(0, 0,  (GLsizei)screenSize.X(),  (GLsizei)screenSize.Y());

  /* Impostazione matrice proiezione */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /* In ordine viene specificato il FOVY, aspect ratio,
      near clipping plane distance,
      far clipping plane distance (distanza di troncamento rendering) in coordinate vista */
  gluPerspective(69, screenSize.X() / screenSize.Y(), 0.2, 3000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* riempimento dello screen buffer di pixel color sfondo */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setCamera();

  map.render(*this);
  star.render(*this);
  sd.render(*this);
  scenario.render(*this);
  techCreator.renderSecretTech(*this);
  resistanceFleet.renderEnemies(*this);

  /* Attesa della fine di rasterizzazione di tutte le primitive */
  glFinish();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  // Passiamo al disegno 2D
  utils::setCoordToPixel(screenSize);

  /* Segnale per l'Ultramotore */
  if (sd.getUltraFuel(*this) == 1) {
    const Point2 textPosition{screenSize.X() - 500, 60};
    textRenderer.render(Colors::Red(), Colors::Black(), "ULTRAENGINE", textPosition, solid, mid, false);
  }

  spaceScanner.render(*this);

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
    textRenderer.render(Colors::Red(), Colors::Black(), scoreString.c_str(), scorePosition, solid, ultra, false);
  }

  {
    /* Gestione del Timer */
    std::stringstream timeStream;
    std::stringstream timeLeftStream;
    float playedSecondsNow = getGameSeconds();
    int timeLeft = (int)maxTime - (int)playedSecondsNow;

    if (timeLeft == 0)
      setState(GameStatus::GAME_OVER);

    if (timeLeft < 60)
    {
      if (timeLeft < 10)
        timeLeftStream << "00:0" << timeLeft;
      else
        timeLeftStream << "00:" << timeLeft;
    }
    else
    {
      if (timeLeft-60 < 10)
        timeLeftStream << "01:0" << timeLeft - 60;
      else
        timeLeftStream << "01:" << timeLeft - 60;
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

  switch (gameStatus)
  {
    case GameStatus::GAME_OVER:
      menu.render(*this);
    break;
    case GameStatus::PAUSE:
      menu.render(*this);
    break;
  }
  glColor3f(1.f, 1.f, 1.f);

  glFinish();
  utils::checkGLError(__FILE__, __LINE__);

  return;
}

void Game::setCamera() const {
  double px = sd.getPosition().X();
  double py = sd.getPosition().Y();
  double pz = sd.getPosition().Z();

  double angle = sd.getFacing();
  double cosf = cos(angle * M_PI / 180.0);
  double sinf = sin(angle * M_PI / 180.0);

  double angley = sd.getFacingY();
  double cosfy = cos(angley * M_PI / 180.0);
  double sinfy = sin(angley * M_PI / 180.0);

  double camd, camh, ex, ey, ez, cx, cy, cz;
  double cosff, sinff;

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
      angle = sd.getFacing() - 40.0;
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

  return;
}

void Game::switchCamera() {
    cameraState = (CameraPos::Type) (((int) cameraState + 1) % (int) CameraPos::CAMERA_MAX);
}

void Game::EatKey(int keycode, bool pressed) {
    controlls.EatKey(keycode, pressed, *this);
}
