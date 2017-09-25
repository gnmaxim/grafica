#include <math.h>
#include <vector>
#include <GL/gl.h>

#include "StarDestroyer.h"
#include "mesh.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"

 #include <iostream>

static TriangleWinding sDMeshWinding = TriangleWinding::CCW;
float height = 0;

// Variabili di tipo mesh
Mesh hull("../assets/obj/corpo.obj", sDMeshWinding); // chiama il costruttore

extern bool useWireframe; // var globale esterna: per usare l'evnrionment mapping

// DoStep: facciamo un passo di fisica (a delta_t costante)
// Indipendente dal rendering.
void StarDestroyer::DoStep(Game &game) {
    /* Evoluzione della fisica simulata della nave */
    float vxm, vym, vzm; // velocita' in spazio macchina

    // da vel frame mondo a vel frame macchina
    float cosf = cos(facing * (float) M_PI / 180.f);
    float sinf = sin(facing * (float) M_PI / 180.f);

    float sinfy = sin(facingy * (float) M_PI / 180.f);
    float cosfy = cos(facingy * (float) M_PI / 180.f);

    if (!turbo) {
      if (game.getGameSeconds() > nextTurbo - turboDelay + 1) {
        grip = 0.45;
        attritoY = 0.92;
        vym = sinfy * 0.1f;
      }
    }
    else {
      grip = 0.001;
      attritoY = 1;
      vym = sinfy * 0.5f;
    }
    vxm = +cosf * vx - sinf * vz;
    // vym = vy;
    vzm = +sinf * vx + cosf * vz;

    /* Gestione propulsione avanti e indietro */
    if (game.inputManager.isMoveForwardPressed()) vzm -= accMax;
    if (game.inputManager.isMoveBackwardPressed()) vzm += accMax;

    // if (game.inputManager.isUpKeyPressed()) vym += accMax; // salita
    // if (game.inputManager.isDownKeyPressed()) vym -= accMax; // discesa

    // Se il turbo è attivato
    if (turbo) {
        if (game.getGameSeconds() < startTurboTime + turboDuration) {
            vzm -= accMax * turboAcc;
        } else {
            turbo = false;
            nextTurbo = game.getGameSeconds() + turboDelay;
        }
    }

    // Attriti
    vxm *= attritoX;
    vym *= attritoY;
    vzm *= attritoZ;

    /* L'orientamento della nave segue il movimento del mouse  in base
      ai due angoli */
    facing = facing + (vzm * grip) * (game.viewAlpha * 1.1f);
    facingy = facingy + (vzm * grip) * (game.viewBeta * 1.1f);

    /* Per scelte di gameplay non si può inclinare la nave di oltre 30 gradi in verticale */
    if (facingy < -45) {
      facingy = -45;
    }
    if (facingy > 45) {
      facingy = 45;
    }

    /* È necessario fermare la rotazione della nave se non viene più mosso il mouse */
    game.viewAlpha = 0;
    game.viewBeta = 0;

    // Ritorno a vel coord mondo
    vx = +cosf * vxm + sinf * vzm;
    vy = vym;
    vz = -sinf * vxm + cosf * vzm;

    // posizione = posizione + velocita * delta t (ma delta t e' costante)
    px += vx;
    if (px < minX) { px = minX; }
    if (px > maxX) { px = maxX; }
    py += vy;
    if (py < minY) { py = minY; }
    if (py > maxY) { py = maxY; }
    pz += vz;
    if (pz < minZ) { pz = minZ; }
    if (pz > maxZ) { pz = maxZ; }

    return;
}

void StarDestroyer::Init() {
    // inizializzo lo stato della macchina
    px = pz = facingy = 0.f; // posizione e orientamento
    facing = 0;
    py = 0;

    mozzoA = mozzoP = sterzo = 0.f;   // stato
    vx = vy = vz = 0.f;      // velocita' attuale


    //velSterzo=3.4f;         // A
    velSterzo = 2.4f;         // A
    velRitornoSterzo = 0.93f; // B, sterzo massimo = A*B / (1-B)

    accMax = 0.0025f;

    // Attriti: percentuale di velocita' che viene mantenuta
    // 1 = no attrito
    // <<1 = attrito grande
    attritoZ = 0.98f;  // piccolo attrito sulla Z
    attritoX = 0.98f;  // grande attrito sulla X (per non fare slittare il sottomarino)
    attritoY = 0.88f; // attrito per la salita e discesa

    // Nota: vel max = accMax*attritoZ / (1-attritoZ)

    raggioRuotaA = 0.25f;
    raggioRuotaP = 0.35f;

    grip = 0.5f; // quanto il facing del sottomarino si adegua velocemente allo sterzo

    return;
}


// Attiva una luce di openGL per simulare un faro della macchina
void StarDestroyer::SetLight() const {
    // GLenum enumerazione di luci usate nella scena
  GLenum usedLight = GL_LIGHT1;
  glEnable(usedLight);

  float position[4] = {0.f, 2.f, 0.f, 1.f};
  glLightfv(usedLight, GL_POSITION, position);

  float direction[3] = {0.f, 0.f, -1.f};
  glLightfv(usedLight, GL_SPOT_DIRECTION, direction);

  glLightf(usedLight, GL_SPOT_CUTOFF, 30.f);
  glLightf(usedLight, GL_SPOT_EXPONENT, 5.f);

  float ambient[4] = {0.0f, 0.0f, 0.0f, 1.f};
  glLightfv(usedLight, GL_AMBIENT, ambient);

  float diffuse[4] = {1.f, 1.f, 1.f, 1.f};
  glLightfv(usedLight, GL_DIFFUSE, diffuse);

  float specular[4] = {0.f, 0.f, 0.f, 1.f};
  glLightfv(usedLight, GL_SPECULAR, specular);

  glLightf(usedLight, GL_CONSTANT_ATTENUATION, 0.f);
  glLightf(usedLight, GL_LINEAR_ATTENUATION, 0.0014f);
  glLightf(usedLight, GL_QUADRATIC_ATTENUATION, 0.007f);


	utils::checkGLError(__FILE__, __LINE__);
}

void StarDestroyer::RenderAllParts(bool usecolor, bool trb) const {
  glPushMatrix();
  {
    static constexpr float StarDestroyerScale = 0.12f;
    const Vector3 ObjectScale{StarDestroyerScale, StarDestroyerScale, -StarDestroyerScale};
    glScalef(ObjectScale.X(), ObjectScale.Y(), ObjectScale.Z());
    static constexpr bool useTexCoords = true;
    utils::checkGLError(__FILE__, __LINE__);
    if (usecolor)
    {
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      {
        float specular[4] = {100.f, 100.f, 100.f, 1.f};
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				float diffuse[4] = {1.f, 1.f, 1.f, 1.f};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				float ambient[4] = {0.01f, 0.01f, 0.01f, 1.f};
        float turbo[4] = {100.f, 0.f, 0.f, 1.f};
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				glMaterialf(GL_FRONT, GL_SHININESS, 128.f);
        if (trb)
        {
          glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, ambient);
        }
        else {
          glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, turbo);
        }
      }
      utils::checkGLError(__FILE__, __LINE__);
      glColor3f(1.f, 1.f, 1.f);

      glEnable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, Texture::StarDestroyerHull);
      hull.RenderNxV(true); // rendering delle mesh hull usando normali per vertice
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);

      utils::checkGLError(__FILE__, __LINE__);
    }
    else
    {
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      hull.RenderNxV(true); // rendering delle mesh hull usando normali per vertice
    }
    utils::checkGLError(__FILE__, __LINE__);
  }
  glPopMatrix();
  // glDisable(GL_COLOR_MATERIAL);
}

// disegna a schermo
void StarDestroyer::render(const Game &game) const {
    // sono nello spazio mondo
    glPushMatrix();
    {
        glTranslatef(px, py, pz);
        glRotatef(facing, 0.f, 1.f, 0.f);
        glRotatef(facingy, 1.f, 0.f, 0.f);

        // sono nello spazio MACCHINA
        SetLight();
        if (!useHeadlight) {glDisable(GL_LIGHT1);}
        RenderAllParts(/*usecolor*/ true, getTurboCharge(game));

    }
    glPopMatrix();

	/* Ombre */
  if (game.renderShadows) {
    glPushMatrix();
    {
      /* Per far sì che l'ombra venga visualizzata sopra al pannello */
      glTranslatef(190, py, pz);

      /* Ruota l'ombra in modo da vedere movimenti sincroni alla nave */
      glRotatef(facing, 0.0f, 1.f, 0.f);
      glRotatef(facingy + 180, 1.f, 0.f, 0.f);

      /* Seleziona la zona della mappa in cui prendere in considerazione il
        calcolo delle ombre */
      if (getPosition().X() > 1) {
        glScalef(1 - (getPosition().X() / 5.f),
                  1 - (getPosition().X() / 5.f),
                  1 - (getPosition().X() / 5.f));
      }
      else {
        glScalef(0.f, 0.f, 0.f);
      }

      // Colore dell'ombra
      glColor3f(12.f / 255.f, 12.f / 255.f, 12.f / 255.f);

      // Appiattimento sull'asse X, viene aggiunto 1% su Y e Z
      glScalef(1.f, 1.01f, 1.01f);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      /* Rendering della nave in versione piatta */
      RenderAllParts(false, false);

      glDisable(GL_BLEND);
      glColor3f(1.f, 1.f, 1.f);
    }
    glPopMatrix();
  }

  utils::checkGLError(__FILE__, __LINE__);

  return;
}

void StarDestroyer::onInputEvent(InputEvent inputEvent, Game& game) {
  switch (inputEvent){
    case InputEvent::TurboButtonPressed:
      if (game.getGameSeconds() > nextTurbo) {
        turbo = true;
        startTurboTime = game.getGameSeconds();
      }
    break;
    case InputEvent::SwitchCamera:
      game.switchCamera();
      break;
    case InputEvent::ToggleHelpMenu:
      game.menuManager.setMenuState(MenuState::graphicsMenu);
      game.setState(GameState::Paused);
      break;
    case InputEvent::UseShadows:
      game.renderShadows = !game.renderShadows;
      break;
  }

  return;
}

float StarDestroyer::getTurboCharge(const Game& game)const {
  float turboCharge = 1.f;
  const float dateNow = game.getGameSeconds();
  const float turboEndDate = startTurboTime + turboDuration;

  if (turbo) {
    turboCharge = 0.f;
  }
  else if (dateNow >= nextTurbo) {
    turboCharge = 1.f;
  }
  else {
    float rawTurboCharge = (dateNow - turboEndDate)* (1/(turboDelay));
    turboCharge = fminf(fmaxf(rawTurboCharge, 0.f), 1.f); // deve essere compreso tra 0 ed 1
  }

  return turboCharge;
}

void StarDestroyer::fillTurboCharge(const Game& game) {
  nextTurbo = game.getGameSeconds();

  return;
}
