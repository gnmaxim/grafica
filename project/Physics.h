#pragma once
#include <SDL2/SDL_hints.h>

namespace Physics {
  /* Numero di millisecondi che un passo di fisica simula */
  constexpr Uint32 stepMs = 10;

  constexpr float stepSeconds = stepMs / 1000.f;

  /* Lunghezza intervallo di calcolo FPS */
  constexpr Uint32 fpsSampling = 3000;
};
