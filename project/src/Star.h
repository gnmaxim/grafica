#pragma once
#include "point3.h"
#include "Space.h"
#include "InputEvent.h"

class Game;

class Star
{
  public:
    void Init();
    void render(const Game &game) const;              // Disegno a schermo

    Star()
    {
      Init();
    }

    // Gestione eventi di input
    void onInputEvent(InputEvent inputEvent, Game& game);

    float getRadius() const
    {
      return 3.f;
    }

    Point3 getPosition() const
    {
      return {px, py, pz};
    }

    float getTurboCharge(const Game& game) const;
    float getFacing() const
    {
      return facing;
    }

  private:
    float px, py, pz, facing;

    void RenderAllParts(bool usecolor, const Game& game) const;
    void SetLight() const;
};
