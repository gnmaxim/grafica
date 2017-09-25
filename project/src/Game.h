#pragma once
#include "Space.h"
#include "StarDestroyer.h"
#include "SpaceObject.h"
#include "Star.h"
#include "MenuManager.h"
#include "SecretTechManager.h"
#include "EnemyManager.h"
#include "TextRenderer.h"
#include "Minimap.h"
#include "Input.h"
#include "GameState.h"
#include "Tutorial.h"

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
    GameState gameState;

    float exPlayedSeconds = 0.f;
    float playedSeconds = 0.f;
    static constexpr float maxTime = 120.f;

    CameraPos::Type cameraState = CameraPos::BACK_CAMERA;
    void setCamera() const;

  public:
    StarDestroyer sD;
    Star star;
    SpaceObject spaceObject;
    SecretTechManager techCreator;
    EnemyManager enemyManager;
    MenuManager menuManager;
    TextRenderer textRenderer;
    Space map;
    Minimap minimap;
    Input inputManager;
  	static Point2 defaultScreenSize;
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

    GameState getCurrentState() const;
    void setState(const GameState newState);

    void EatKey(int keycode, bool pressed, bool joystick);
    void EatJoyAxis(int axisIndex, int axisValue);
    int incrementScore(int inc);
    void render();

    float getGameSeconds() const {
      return playedSeconds;
    }
};
