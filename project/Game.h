#pragma once
#include "Space.h"
#include "StarDestroyer.h"
#include "Scenario.h"
#include "Star.h"
#include "Menu.h"
#include "SecretTechManager.h"
#include "ResistanceFleet.h"
#include "TextRenderer.h"
#include "SpaceScanner.h"
#include "Controlls.h"
#include "GameStatus.h"


namespace CameraPos {
  enum Type {
    BACK_CAMERA,
    CAMERA_TOP_FIXED,
    LATERAL_CAM,
    TOP_CAM,
    CAMERA_MAX
  };
}

class Game
{
  private:
    int score = 0;
    GameStatus gameStatus;

    float exPlayedSeconds = 0.f;
    float playedSeconds = 0.f;
    static constexpr float maxTime = 120.f;

    CameraPos::Type cameraState = CameraPos::BACK_CAMERA;
    void setCamera() const;

  public:
    StarDestroyer sd;
    Star star;
    Scenario scenario;
    SecretTechManager techCreator;
    ResistanceFleet resistanceFleet;
    Menu menu;
    TextRenderer textRenderer;
    Space map;
    SpaceScanner spaceScanner;
    Controlls controlls;
  	static Point2 RESOLUTION;
  	Point2 screenSize;

    Game();

    int getScore() const {
        return score;
    }

    CameraPos::Type getCameraPos() const {
        return cameraState;
    }

    /* Variabili per gestione visuale */
    int mousex = 0;
    int mousey = 0;
    float viewAlpha = 0.f;
    float viewBeta = 0.f;

    /* Distanza dall'origine */
    float eyeDist = 5.f;
    bool renderShadows = true;

    void init();
    void switchCamera();
    void doStep();

    GameStatus getCurrentState() const;
    void setState(const GameStatus newState);

    void EatKey(int keycode, bool pressed);
    int addPoints(int inc);
    void render();

    float getGameSeconds() const {
      return playedSeconds;
    }
};
