
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


namespace CameraState {
    enum Type {
        CAMERA_BACK_SUB,
        CAMERA_TOP_FIXED,
        CAMERA_LATERAL_SUB,
        CAMERA_TOP_SUB,
        CAMERA_MOUSE,
        CAMERA_MAX
    };
}

class Game
{
  private:
    static constexpr float gameDuration = 60.f;
    float previousElapsedSeconds = 0.f;
    float gameSeconds = 0.f;

    void setCamera() const;

    // CAMERA SETTINGS
    CameraState::Type cameraState = CameraState::CAMERA_BACK_SUB;



    int score = 0;
    GameState gameState;
public:

    StarDestroyer sub;
    Star star;
    SpaceObject spaceObject;
    SecretTechManager junkManager;
    EnemyManager enemyManager;
    MenuManager menuManager;
    TextRenderer textRenderer;
    Space map;
    Minimap minimap;
    Input inputManager;
    Tutorial tutorial;

	static Point2 defaultScreenSize;
	Point2 screenSize;

    Game();

    int getScore() const {
        return score;
    }

    CameraState::Type getCameraState() const {
        return cameraState;
    }

    // Angoli che definiscono la vista
    int mousex = 0;
    int mousey = 0;
    float viewAlpha = 0.f;
    float viewBeta = 0.f;
    float eyeDist = 5.f; // Distanza dell'occhio dall'origine

    void switchCamera();

    void init();

    void doStep();

    GameState getCurrentState() const;

    void render();

    void setState(const GameState newState);

    int incrementScore(int inc);

    void EatKey(int keycode, bool pressed, bool joystick);

    void EatJoyAxis(int axisIndex, int axisValue);

	float getGameSeconds() const {return gameSeconds;}

    bool useShadow = true;


};
