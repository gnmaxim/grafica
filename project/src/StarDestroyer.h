#pragma once

#include "point3.h"
#include "Space.h"
#include "InputEvent.h"


class Game;
class Enemy;
class Input;
enum class InputEvent;

class StarDestroyer {
  public:
    void Init();
    void DoStep(Game &game); /* Evoluzione dello stato della nave */
    void render(const Game &game) const;
    StarDestroyer() {
      Init();
    }

    /* Gestore degli Input */
    void onInputEvent(InputEvent inputEvent, Game& game);

    float getRadius() const {
        return 3.f;
    }

    Point3 getPosition() const {
        return {px, py, pz};
    }

  	float getFacing() const { return facing;}
    float getFacingY() const { return facingy;}

    float getTurboCharge(const Game& game)const;
    void fillTurboCharge(const Game& game);

  private:
    /* Variabili che descrivono la posizione */
	  float px = 0.f;
    float py;
    float pz;

    /* Orientamento della nave secondo i due angoli */
    float facing, facingy;

    /* Velocità attuale sugli assi */
    float vx, vy, vz;

  	float mozzoA, mozzoP, sterzo;
    float velSterzo, velRitornoSterzo, accMax;
    float raggioRuotaA, raggioRuotaP, grip;

    /* Attriti sui vari assi */
    float attritoX, attritoY, attritoZ; // attriti

    bool useHeadlight = true;

    /* Veriabili per la gestione dell'Ultramotore */
    bool turbo = false; // TRUE se il turbo è attivo, FALSE altrimenti
    float startTurboTime = 0.f; // (in secondi) ultima data di inizio del turbo
    float nextTurbo = 0.f; // (in secondi) prossima data dopo la quale sarà possibile usare il turbo
    float turboDelay = 45.f; // (in secondi) il turbo può essere usato solo una volta ogni 30 secondi
    float turboDuration = 1.f; // durata del turbo (in secondi)
    float turboAcc = 20.f; // accelerazione del turbo

    // Variabili per evitare che il sottomarino esca dai limiti della mappa "giocabile"
    float maxX = Space::width / 2;
    float minX = - (Space::width / 2);
    float maxZ = Space::width / 2;
    float minZ = - (Space::width / 2);
    float maxY = Space::waterHeight;
    float minY = Space::terrainHeight;

    void RenderAllParts(bool usecolor, bool trb) const;
    void SetLight() const;
};
