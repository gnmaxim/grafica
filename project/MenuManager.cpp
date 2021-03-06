/*
  22/09/2017
*/

#include <deque>

#include "Game.h"
#include "Menu.h"
#include "Utils.h"
#include "Colors.h"


void Menu::setMenuState(MenuState state) {
  currentMenuState = state;

  return;
}

void Menu::render(const Game &game) const {
  switch (currentMenuState)
  {
    case MenuState::gameOver:
      renderGameOver(game);
      break;
    case MenuState::graphicsMenu:
      renderGraphicsMenu(game);
      break;
  }

  return;
}


bool Menu::capturesInput() const {
  return currentMenuState != MenuState::none;
}

void Menu::onInputEvent(InputEvent inputEvent, Game& game)
{
  if (currentMenuState == MenuState::gameOver)
    if (inputEvent == InputEvent::QUIT_MENU)
      exit(0);

  if (currentMenuState == MenuState::graphicsMenu)
    if (inputEvent == InputEvent::MENU) {
      setMenuState(MenuState::none);
      game.setState(GameStatus::SMASHING_XWING);
    }

  return;
}

void Menu::renderGameOver(const Game &game) const
{
  /* Impostazione viewport */
  glViewport(0, 0, (GLsizei) game.screenSize.X(), (GLsizei) game.screenSize.Y());

  /* Sfondo */
  glClearColor(0, 0, 0, 1);

  /* Riempimento screen buffer con il colore precedentemente impostato */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  utils::setCoordToPixel(game.screenSize);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  /* Visualizzazione punteggio */
  std::stringstream scoreStream;
  scoreStream << "TECNOLOGIA IMPERIALE RACCOLTA";
  Point2 scoreStringPosition = {(game.screenSize.X() / 4 - 200), (game.screenSize.Y() / 2)};
  std::string scoreString = scoreStream.str();

  std::string gameOverString = std::to_string(game.getScore());
  Point2 gameOverStringPosition = {(game.screenSize.X() / 4 - 200), (game.screenSize.Y() / 2 - 100)};

  std::string pressEscString = "(Escape)";
  Point2 pressEscStringPosition = {(game.screenSize.X() / 4 - 200), (game.screenSize.Y() / 2 - 150)};

  /* Rendering del testo */
  game.textRenderer.render(Colors::Red(), Colors::Black(), scoreString.c_str(), scoreStringPosition, shaded, big, false);
  game.textRenderer.render(Colors::Red(), Colors::Black(), gameOverString.c_str(), gameOverStringPosition, blended, big, false);
  game.textRenderer.render(Colors::Red(), Colors::Black(), pressEscString.c_str(), pressEscStringPosition, blended, small, false);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glFinish();

  return;
}


void Menu::renderGraphicsMenu(const Game &game) const {
  /* Impostazione delle matrici */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glPushMatrix();
  {
    utils::setCoordToPixel(game.screenSize);

    /* Disegno del menu */
    glPushMatrix();
    {
      /* Disegno dello sfondo del menu */
      const Point2 menuPosition = Point2{0, 0};
      const Point2 menuSize = Point2{game.RESOLUTION.X(), game.RESOLUTION.Y()};
      glTranslatef(menuPosition.X(), menuPosition.Y(), 1.f);
      glScalef(menuSize.X(), menuSize.Y(), 1.f);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      {
        glBegin(GL_QUADS);
        {
          glColor4f(0.f, 0.f, 0.f, 1.f);
          glVertex2f(0.f, 0.f);
          glVertex2f(0.f, 1.f);
          glVertex2f(1.f, 1.f);
          glVertex2f(1.f, 0.f);
          glColor4f(1.f, 1.f, 1.f, 1.f);
        }
        glEnd();
      }

      glDisable(GL_BLEND);
      glFinish();
    }
    glPopMatrix();

    glPushMatrix();
    {
      const Point2 portraitSize = Point2{141.f, 141.f};
      const Point2 portraitPosition = Point2{game.RESOLUTION.X() / 2 + portraitSize.X() / 2, 250};
      glTranslatef(portraitPosition.X(), portraitPosition.Y(), 0.f);
      glScalef(portraitSize.X(), portraitSize.Y(), 1.f);
      glRotated(180, 0, 0, 1);
      glColor3f(1.f, 1.f, 1.f);

      /* Inserimento della propria foto */
      {
        glBindTexture(GL_TEXTURE_2D, Texture::mg);
        glEnable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
        {
          glTexCoord2f(0.0f, 0.0f);
          glVertex2f(0.f, 0.f);
          glTexCoord2f(0.0f, 1.0f);
          glVertex2f(0.f, 1.f);
          glTexCoord2f(1.0f, 1.0f);
          glVertex2f(1.f, 1.f);
          glTexCoord2f(1.0f, 0.0f);
          glVertex2f(1.f, 0.f);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
      }

      glFinish();
    }
    glPopMatrix();

    std::deque<menuElement> menuElementsList;
    for (int i = 0; i < menuStrings.size() - 2; ++i) {
      Point2 position = {};
      menuElementsList.emplace_front(menuStrings.at(i), Point2{100, game.RESOLUTION.Y() / 5 * 4 - (i * 50)});
    }
    for (const menuElement& element : menuElementsList) {
      game.textRenderer.render(Colors::Red(), Colors::Black(), element.string.c_str(), element.position, blended, small, false);
    }

    std::deque<menuElement> lastElementsList;
    for (int i = 8; i < 9; ++i) {
      Point2 position = {};
      lastElementsList.emplace_front(menuStrings.at(i), Point2{400, 300.f});
    }
    game.textRenderer.render(Colors::Red(), Colors::Black(), lastElementsList[0].string.c_str(), lastElementsList[0].position, solid, big, false);

    std::deque<menuElement> trullylastElementsList;
    for (int i = 9; i < 10; ++i) {
      Point2 position = {};
      trullylastElementsList.emplace_front(menuStrings.at(i), Point2{game.RESOLUTION.X() / 2 - 69, 69.f});
    }
    game.textRenderer.render(Colors::Red(), Colors::Black(), trullylastElementsList[0].string.c_str(), trullylastElementsList[0].position, solid, small, false);
  }
  glPopMatrix();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  return;
}
