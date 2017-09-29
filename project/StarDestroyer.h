#pragma once

#include "point3.h"
#include "Space.h"
#include "InputEvent.h"


class Game;
class XWing;
class Controlls;
enum class InputEvent;

class StarDestroyer {
  public:
    void Init();

    /* L'evoluzione della nave ad ogni passo */
    void DoStep(Game &game);

    void render(const Game &game) const;

    StarDestroyer() {
      Init();
    }

    /* Gestore degli Controlls */
    void onInputEvent(InputEvent inputEvent, Game& game);

    float getRadius() const {
        return 2.5f;
    }

    Point3 getPosition() const {
        return {px, py, pz};
    }

  	float getFacing() const { return facing;}
    float getFacingY() const { return facingy;}

    float getUltraFuel(const Game& game)const;
    void fillUltraFuel(const Game& game);

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

    /* Accelerazione in modalità Ultra */
    float ultraBoost = 100.f;

    /* Durata della spinta UltraEngine */
    float ultraDuration = 0.25f;

    /* Secondi necessari per la rigenerazione della spinta */
    float ultraRigeneration = 45.f;

    /* Veriabili per la gestione dell'Ultramotore */
    bool ultraState = false;
    float ultraStart = 0.f;
    float nextUltra = 0.f;

    /* Settiamo i confini dello spazio esplorabile dalla nave */
    float maxX = Space::playableWidth / 2;
    float minX = - (Space::playableWidth / 2);
    float maxZ = Space::playableWidth / 2;
    float minZ = - (Space::playableWidth / 2);
    float maxY = Space::bottomY;
    float minY = Space::topY;

    void RenderAllParts(bool usecolor, bool ultra) const;
};
