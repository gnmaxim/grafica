#include <math.h>
#include <vector>
#include <GL/gl.h>

#include "StarDestroyer.h"
#include "mesh.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"

 #include <iostream>

static TriangleWinding sdMeshWinding = TriangleWinding::CCW;
float height = 0;

// Variabili di tipo mesh
Mesh hull("mesh/sd.obj", sdMeshWinding); // chiama il costruttore

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

    if (!ultraState) {
      if (game.getGameSeconds() > nextUltra - ultraRigeneration + 1) {
        grip = 0.45;
      }
    }
    else {
      grip = 0.001;
    }

    vxm = +cosf * vx - sinf * vz;
    vym = vy;
    vzm = +sinf * vx + cosf * vz;

    /* Gestione propulsione avanti e indietro */
    if (game.controlls.isMoveForwardPressed()) {
      vzm -= cosfy * accMax;
      vym += sinfy * accMax;
    }

    /* Caso particolare, attivazione dell'Ultramotore */
    if (ultraState) {
        if (game.getGameSeconds() < ultraStart + ultraDuration) {
          vzm -= cosfy * accMax * ultraBoost;
          vym += sinfy * accMax * ultraBoost;
        } else {
            ultraState = false;
            nextUltra = game.getGameSeconds() + ultraRigeneration;
        }
    }

    // Attriti
    vxm *= attritoX;
    vym *= attritoY;
    vzm *= attritoZ;

    /* L'orientamento della nave segue il movimento del mouse in base
      ai due angoli */
    facing = facing + (vzm * grip) * (game.viewAlpha * 1.1f);
    facingy = facingy + (vzm * grip) * (game.viewBeta * 1.1f);

    /* Per scelte di gameplay non si può inclinare la nave di oltre tot gradi in verticale */
    if (facingy < -70) {
      facingy = -70;
    }
    if (facingy > 70) {
      facingy = 70;
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

    /* Attriti uguali su tutti gli assi, siamo nello spazio */
    attritoZ = 0.98f;
    attritoX = 0.98f;
    attritoY = 0.98f;

    // Nota: vel max = accMax*attritoZ / (1-attritoZ)
    raggioRuotaA = 0.25f;
    raggioRuotaP = 0.35f;

    /* Quanto velocemente si adegua il facing dello StarDestroyer allo "sterzo" */
    grip = 0.5f;

    return;
}


void StarDestroyer::RenderAllParts(bool usecolor, bool ultr) const {
  glPushMatrix();
  {
    static constexpr float StarDestroyerScale = 0.11f;
    const Vector3 ObjectScale{StarDestroyerScale, StarDestroyerScale, -StarDestroyerScale};
    glScalef(ObjectScale.X(), ObjectScale.Y(), ObjectScale.Z());
    static constexpr bool useTexCoords = true;
    utils::checkGLError(__FILE__, __LINE__);
    if (usecolor)
    {
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      {
        float specular[4] = {0.9f, 0.9f, 0.9f, 1.f};
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				// float diffuse[4] = {1.f, 1.0f, 1.f, 0.f};
        GLfloat diffuse[4] = {225.f / 255.f, 225.f / 255.f, 225.f / 255.f, 1.0f};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				float ambient[4] = {0.01f, 0.01f, 0.01f, 1.f};

        float ultra[4] = {abs(vz) + abs(vy), 0.f, 0.f, 1.f};
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				// glMaterialf(GL_FRONT, GL_SHININESS, 1.f);
        if (ultr) {
          glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, ambient);
        }
        else {
          glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, ultra);
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

void StarDestroyer::render(const Game &game) const {
  glPushMatrix();
  {
    glTranslatef(px, py, pz);
    glRotatef(facing, 0.f, 1.f, 0.f);
    glRotatef(facingy, 1.f, 0.f, 0.f);

    RenderAllParts(/*usecolor*/ true, getUltraFuel(game));
  }
  glPopMatrix();

	/* Se le ombre sono attive */
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
      if (getPosition().X() > 1)
        glScalef(1 - (getPosition().X() / 10.f), 1 - (getPosition().X() / 10.f), 1 - (getPosition().X() / 10.f));
      else
        glScalef(0.f, 0.f, 0.f);

      // Colore dell'ombra
      glColor3f(0.f / 255.f, 0.f / 255.f, 0.f / 255.f);

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
    case InputEvent::ACTIVATE_ULTRA:
      if (game.getGameSeconds() > nextUltra) {
        ultraState = true;
        ultraStart = game.getGameSeconds();
      }
    break;
    case InputEvent::SWITCH_POV:
      game.switchCamera();
      break;
    case InputEvent::MENU:
      game.menu.setMenuState(MenuState::graphicsMenu);
      game.setState(GameStatus::PAUSE);
      break;
    case InputEvent::SHADOW_SIMULATION:
      game.renderShadows = !game.renderShadows;
      break;
  }

  return;
}

float StarDestroyer::getUltraFuel(const Game& game) const {
  float ultraCharge = 1.f;
  const float dateNow = game.getGameSeconds();
  const float ultraEndDate = ultraStart + ultraDuration;

  if (ultraState)
    ultraCharge = 0.f;
  else if (dateNow >= nextUltra)
    ultraCharge = 1.f;
  else {
    float rawUltra = (dateNow - ultraEndDate) * (1 / (ultraRigeneration));

    /* Valore che dovrà essere compreso fra 0 e 1 */
    ultraCharge = fminf(fmaxf(rawUltra, 0.f), 1.f);
  }

  return ultraCharge;
}

void StarDestroyer::fillUltraFuel(const Game& game) {
  nextUltra = game.getGameSeconds();

  return;
}
