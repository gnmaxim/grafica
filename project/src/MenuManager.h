/*
  22/09/2017
*/

#pragma once
#include <array>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>

#include "point2.h"
#include "InputEvent.h"


class Game;


/* Variabili enum che descrivono lo stato del Menu stesso */
enum class MenuState {graphicsMenu, gameOver, none};


class MenuManager
{
  private:
    MenuState currentMenuState = MenuState::graphicsMenu;

    void renderPlot(const Game& game) const;
    void renderGraphicsMenu(const Game& game) const;
    void renderGameOver(const Game& game) const;

    /* Strutture dati del Menu */
    struct menuElement {
      std::string string;
      Point2 position;
      menuElement(const std::string &string, const Point2 &position) : string(string), position(position) {}
    };

    std::array<std::string, 10> menuStrings = {{"I ribelli sono riusciti a distruggere il nostro modello sperimentale di Morte Nera.",
                                                "In attesa della Flotta Imperiale, e' necessario raccoglierne i resti ed evitare",
                                                "che i ribelli entrino in contatto con le nostre TECNOLOGIE piu' avanzate.",
                                                " ",
                                                "Abbiamo a bordo parecchi prigionieri, inclusa la loro... principessa. Quindi",
                                                "non ci spareranno, ma dobbiamo evitare a tutti i costi l'abbordaggio.",
                                                " ",
                                                "Deve evitare l'avvicinamento!",
                                                "ANNIENTARE LA RESISTENZA",
                                                "(Enter)"
                                              }};

                                              //
                                              // "F1   Help METTERE QUI LA STORIA",
                                              // "F2   Cambia visuale",
                                              // "F3   Faro",
                                              // "F4   Rifrazione acqua",
                                              // "F5   Ombre",

  public:
    void render(const Game& game) const;
    void setMenuState(MenuState state);
    bool capturesInput() const;
    void onInputEvent(InputEvent inputEvent, Game& game);
};
