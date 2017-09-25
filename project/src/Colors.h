#include <SDL2/SDL_pixels.h>
#pragma once

namespace Colors
{
  inline SDL_Color Blue()
  {
    SDL_Color blue{0, 0, 255, 255};

    return blue;
  }

  inline SDL_Color Green()
  {
    SDL_Color green{0, 255, 0, 255};

    return green;
  }

  inline SDL_Color Black()
  {
    SDL_Color black{0, 0, 0, 255};

    return black;
  }

  inline SDL_Color Yellow()
  {
    SDL_Color yellow{255, 255, 0, 255};

    return yellow;
  }

  inline SDL_Color White()
  {
    SDL_Color white{255, 255, 255, 255};

    return white;
  }

  inline SDL_Color Red()
  {
    SDL_Color white{255, 0, 0, 255};

    return white;
  }
}
