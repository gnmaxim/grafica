#include <math.h>
#include <vector>
#include <GL/gl.h>

#include "StarDestroyer.h"
#include "mesh.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"

static TriangleWinding submarineMeshWinding = TriangleWinding::CCW;
float height = 0;

// Variabili di tipo mesh
Mesh hull("../assets/obj/corpo.obj", submarineMeshWinding); // chiama il costruttore

extern bool useWireframe; // var globale esterna: per usare l'evnrionment mapping

// DoStep: facciamo un passo di fisica (a delta_t costante)
//
// Indipendente dal rendering.
void StarDestroyer::DoStep(Game &game) {
    // Computiamo l'evolversi del sottomarino
    float vxm, vym, vzm; // velocita' in spazio macchina
    // da vel frame mondo a vel frame macchina
    float cosf = cos(facing * (float) M_PI / 180.f);
    float sinf = sin(facing * (float) M_PI / 180.f);
    float sinfy = sin(facingy * (float) M_PI / 180.f);
    vxm = +cosf * vx - sinf * vz;
    vym = sinfy * 0.1f;
    vzm = +sinf * vx + cosf * vz;

    /* Gestione propulsione avanti e indietro */
    if (game.inputManager.isMoveForwardPressed()) vzm -= accMax;
    if (game.inputManager.isMoveBackwardPressed()) vzm += accMax;

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
    if (facingy < -60) {
      facingy = -60;
    }
    if (facingy > 60) {
      facingy = 60;
    }

    /* È necessario fermare la rotazione della nave se non viene più mosso il mouse */
    game.viewAlpha = 0;
    game.viewBeta = 0;

    // Rotazione mozzo (a seconda della velocita' sulla z)
    float da; //delta angolo
    da = (360.f * vzm) / (2.f * (float) M_PI * raggioRuotaA);
    mozzoA += da;
    da = (360.f * vzm) / (2.f * (float) M_PI * raggioRuotaP);
    mozzoP += da;

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
    px = pz = facing = facingy = 0.f; // posizione e orientamento
    py = 0;

    mozzoA = mozzoP = sterzo = 0.f;   // stato
    vx = vy = vz = 0.f;      // velocita' attuale


    //velSterzo=3.4f;         // A
    velSterzo = 2.4f;         // A
    velRitornoSterzo = 0.93f; // B, sterzo massimo = A*B / (1-B)

    accMax = 0.0011f;

    // Attriti: percentuale di velocita' che viene mantenuta
    // 1 = no attrito
    // <<1 = attrito grande
    attritoZ = 0.991f;  // piccolo attrito sulla Z
    attritoX = 0.8f;  // grande attrito sulla X (per non fare slittare il sottomarino)
    attritoY = 0.95f; // attrito per la salita e discesa

    // Nota: vel max = accMax*attritoZ / (1-attritoZ)

    raggioRuotaA = 0.25f;
    raggioRuotaP = 0.35f;

    grip = 0.45f; // quanto il facing del sottomarino si adegua velocemente allo sterzo
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


void StarDestroyer::RenderAllParts(bool usecolor) const {
  // Disegna corpo del sottomarino con una mesh
  // glEnable(GL_COLOR_MATERIAL);
  glPushMatrix();
  {
    static constexpr float StarDestroyerScale = 0.12f; // 0.15
    const Vector3 ObjectScale{StarDestroyerScale, StarDestroyerScale, -StarDestroyerScale};
    glScalef(ObjectScale.X(), ObjectScale.Y(), ObjectScale.Z());
    static constexpr bool useTexCoords = true;
    utils::checkGLError(__FILE__, __LINE__);
    if (usecolor)
    {
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      {
        float specular[4] = {1.f, 1.f, 1.f, 1.f};
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				float diffuse[4] = {1.f, 1.f, 1.f, 1.f};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				float ambient[4] = {0.1f, 0.1f, 0.1f, 1.f};
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				glMaterialf(GL_FRONT, GL_SHININESS, 128.f);
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, ambient);
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
        RenderAllParts(/*usecolor*/ true);

    }
    glPopMatrix();

	/* Ombre */
  if (game.useShadow) {
    glPushMatrix();
    {
      /* Per far sì che l'ombra venga visualizzata sopra al pannello */
      glTranslatef(190, py, pz);

      /* Ruota l'ombra in modo da vedere movimenti sincroni alla nave */
      glRotatef(facing, 0.0f, 1.f, 0.f);
      glRotatef(facingy + 180, 1.f, 0.f, 0.f);

      /* Seleziona la zona della mappa in cui prendere in considerazione il
        calcolo delle ombre */
      if (getPosition().X() > 0) {
        glScalef(1-(getPosition().X()/5.f), 1- (getPosition().X()/5.f), 1-(getPosition().X()/5.f));
      }
      else {
        glScalef(0.f, 0.f, 0.f);
      }

      // Colore dell'ombra
      glColor3f(12.f/255.f, 12.f/255.f, 12.f/255.f);

      // Appiattimento sull'asse X, viene aggiunto 1% su Y e Z
      glScalef(1.f, 1.01f, 1.01f);
      glTranslatef(0.f, 0, 0.f); // alzo l'ombra di un epsilon per evitare z-fighting con il pavimento
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      RenderAllParts(/*usecolor*/ false); // disegno il sottomarino appiattito
      glDisable(GL_BLEND);
      glColor3f(1.f, 1.f,1.f);
      }
      glPopMatrix();
    }



    utils::checkGLError(__FILE__, __LINE__);
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
        case InputEvent::UseHeadlight:
            useHeadlight = !useHeadlight;
            break;

        case InputEvent::ToggleHelpMenu:
            if (game.getGameSeconds() > game.tutorial.tutorialEndDate) {
                game.menuManager.setMenuState(MenuState::graphicsMenu);
                game.setState(GameState::Paused);
            }
            break;

        case InputEvent::ToggleCaustics:
            game.map.caustics = !game.map.caustics;
            break;

        case InputEvent::UseShadows:
            game.useShadow = !game.useShadow;
            break;
    }
}

    float StarDestroyer::getTurboCharge(const Game& game)const {
    float turboCharge = 1.f;
    const float dateNow = game.getGameSeconds();
    const float turboEndDate = startTurboTime + turboDuration;
    if (turbo) {
        turboCharge = 0.f;
    } else if (dateNow >= nextTurbo) {
        turboCharge = 1.f;
    } else {
        float rawTurboCharge = (dateNow - turboEndDate)* (1/(turboDelay));
        turboCharge = fminf(fmaxf(rawTurboCharge, 0.f), 1.f); // deve essere compreso tra 0 ed 1
    }
    return turboCharge;
}
