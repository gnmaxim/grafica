#pragma once
#include "point3.h"
#include "Space.h"
#include "InputEvent.h"

class Game;

class Star
{
  private:
    float px, py, pz, facing;

    void RenderAllParts(bool usecolor, const Game& game) const;

  public:
    void Init();
    void render(const Game &game) const;

    Star()
    {
      Init();
    }

    Point3 getPosition() const
    {
      return {px, py, pz};
    }
};
