#pragma once
#include <SDL2/SDL_keycode.h>
#include "InputEvent.h"


class Game;

class Controlls {
  private:
    bool moveForwardPressed = false;

public:
    bool isMoveForwardPressed() const { return moveForwardPressed; }

    void KeyCodeToInputEvent(SDL_Keycode keycode, InputEvent &outInputEvent, bool &isEventFound);

    void onInputEvent(InputEvent inputEvent, Game &game);

    void EatKey(SDL_Keycode keycode, bool pressed, Game &game);
};
