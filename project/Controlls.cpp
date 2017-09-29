#include <SDL2/SDL_events.h>
#include "Controlls.h"
#include "GameStatus.h"
#include "Game.h"


void Controlls::EatKey(SDL_Keycode keycode, bool pressed, Game &game) {
  if (pressed) {
    switch (keycode) {
      case (SDLK_w):
        moveForwardPressed = true;
      break;
      default:
        bool HasFoundEvent = false;
        InputEvent Event;

        KeyCodeToInputEvent(keycode, Event, HasFoundEvent);

        if (HasFoundEvent)
          onInputEvent(Event, game);
      break;
    }
  }
  else {
    switch (keycode) {
      case SDLK_w:
        moveForwardPressed = false;
      break;
    }
  }

  return;
}

void Controlls::onInputEvent(InputEvent inputEvent, Game &game) {
  if (game.menu.capturesInput())
    game.menu.onInputEvent(inputEvent, game);
  else if (game.getCurrentState() == GameStatus::SMASHING_XWING)
    game.sd.onInputEvent(inputEvent, game);

  return;
}

void Controlls::KeyCodeToInputEvent(SDL_Keycode keycode, InputEvent &outInputEvent, bool &isEventFound) {
  switch (keycode) {
    case SDLK_SPACE:
      outInputEvent = InputEvent::ACTIVATE_ULTRA;
      isEventFound = true;
    break;
    case SDLK_ESCAPE:
      outInputEvent = InputEvent::QUIT_MENU;
      isEventFound = true;
    break;
    case SDLK_F1:
      outInputEvent = InputEvent::MENU;
      isEventFound = true;
    break;
    case SDLK_RETURN:
      outInputEvent = InputEvent::MENU;
      isEventFound = true;
    break;
    case SDLK_F2:
      outInputEvent = InputEvent::SWITCH_POV;
      isEventFound = true;
    break;
    case SDLK_F3:
      outInputEvent = InputEvent::SHADOW_SIMULATION;
      isEventFound = true;
    break;
    default:
      isEventFound = false;
    break;
  }

  return;
}
