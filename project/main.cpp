#include <iostream>

#include "StarDestroyer.h"
#include "Game.h"
#include "Textures.h"
#include "SecretTechManager.h"
#include "Physics.h"
#include "SpaceScanner.h"
#include "Menu.h"
#include "ResistanceFleet.h"
#include "Utils.h"


/* Totale passi di fisica */
Uint32 nstep = 0;

/* Frame al secondo */
float fps = 0;                // precedentemente
int fpsNow = 0;               // attualmente
Uint32 timeLastInterval = 0;  // inizio nuovo intervallo


static Game game;

/* Rendering della scena */
void rendering(SDL_Window *win) {
  fpsNow++;
  game.render();

  glFinish();
  utils::checkGLError(__FILE__, __LINE__);
  SDL_GL_SwapWindow(win);
}

void redraw() {
  // ci automandiamo un messaggio che (s.o. permettendo)
  // ci fara' ridisegnare la finestra
  SDL_Event e;
  e.type = SDL_WINDOWEVENT;
  e.window.event = SDL_WINDOWEVENT_EXPOSED;
  SDL_PushEvent(&e);
}

int main(int argc, char *argv[])
{
  SDL_Window *win;
  SDL_GLContext mainContext;
  Uint32 windowID;

  // Inizializzazione di SDL
  SDL_Init(SDL_INIT_VIDEO);

  // Inizializzazione di SDL_TTF
  if (TTF_Init() < 0)
  {
    fprintf(stderr, "Impossibile inizializzare TTF: %s\n", SDL_GetError());
    SDL_Quit();
    return (2);
  }

  /* Viene disabilitato il cursore */
  SDL_ShowCursor(SDL_DISABLE);

  /* Per non bloccare il movimento del mouse, imposta la modalità di movimento relativo */
  SDL_SetRelativeMouseMode(SDL_TRUE);

  // Imposto i parametri della finestra OpenGL
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Creazione finestra
  win = SDL_CreateWindow(argv[0], 0, 0, (int) Game::RESOLUTION.X(), (int) Game::RESOLUTION.Y(),
                         SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

  // Gioco a schermo intero
  // SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);

  // Create our OpengGL context and attach it to our window
  mainContext = SDL_GL_CreateContext(win);

  // Smoothing modes
  {
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  }

  glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_ALWAYS);

  // glEnable(GL_LIGHTING);
  // glEnable(GL_LIGHT0);

  glEnable(GL_NORMALIZE); // opengl, per favore, rinormalizza le normali
  // prima di usarle
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW); // consideriamo Front Facing le facce ClockWise
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_POLYGON_OFFSET_FILL); // caro openGL sposta i
  // frammenti generati dalla
  // rasterizzazione poligoni
  glPolygonOffset(1.f, 1.f);             // indietro di 1

  utils::checkGLError(__FILE__, __LINE__);

  if (!Texture::LoadAllTextures()) return -1;

  game.init();

  game.menu.setMenuState(MenuState::graphicsMenu);
  game.setState(GameStatus::PAUSE);

  int rel = 0;
  bool done = false;
  while (!done) {
    SDL_Event e;
    // guardo se c'e' un evento:
    if (SDL_PollEvent(&e)) {
      // se si: processa evento
      switch (e.type) {
        case SDL_KEYDOWN:
          game.EatKey(e.key.keysym.sym, true);
          break;
        case SDL_KEYUP:
          game.EatKey(e.key.keysym.sym, false);
          break;
        case SDL_QUIT:
          done = true;
          break;
        case SDL_WINDOWEVENT:
          // dobbiamo ridisegnare la finestra
          if (e.window.event == SDL_WINDOWEVENT_EXPOSED)
              rendering(win);
          else {
            windowID = SDL_GetWindowID(win);
            if (e.window.windowID == windowID &&
                e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
              game.screenSize = Point2{(float) e.window.data1, (float) e.window.data2};
              rendering(win);
              redraw();
              break;
            }
          }
          break;
        case SDL_MOUSEMOTION:
          game.viewAlpha += e.motion.xrel;
          game.viewBeta += e.motion.yrel;

          if (game.viewBeta < -30) game.viewBeta = -30;
          if (game.viewBeta > +30) game.viewBeta = +30;

          game.mousex = e.motion.x;
          game.mousey = e.motion.y;
          break;
        }
      }
      else {
        // nessun evento: siamo IDLE
        Uint32 timeNow = SDL_GetTicks(); // che ore sono?

        if (timeLastInterval + Physics::fpsSampling < timeNow) {
          fps = 1000.f * ((float) fpsNow) / (timeNow - timeLastInterval);
          fpsNow = 0;
          timeLastInterval = timeNow;
        }

        bool doneSomething = false;
        int guardia = 0; // sicurezza da loop infinito

        // finche' il tempo simulato e' rimasto indietro rispetto
        // al tempo reale...
        while (nstep * Physics::stepMs < timeNow) {
          game.doStep();
          doneSomething = true;
          nstep++;
          timeNow = SDL_GetTicks();
          if (guardia++ > 1000) {
            done = true;
            break;
          } // siamo troppo lenti!
        }

        if (doneSomething)
          rendering(win);
          //redraw();
        else {
          // tempo libero!!!
        }
      }

      utils::checkGLError(__FILE__, __LINE__);
  }

  SDL_GL_DeleteContext(mainContext);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return (0);
}
