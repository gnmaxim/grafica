#pragma once
#include "point3.h"
#include "mesh.h"
#include "StarDestroyer.h"
#include "Textures.h"
#include <memory>


class Game;

class XWing {
private:
  Point3 position;

  /* m/s */
  float speed;

  /* Indica se il nemico è rivolto verso il giocatore */
  bool playerOriented;

  const Mesh *enemyMesh;
  float scale;
  GLuint textureID;

public:
  XWing(const Point3 &position, float speed, float scale, const Vector3 &rotation, bool playerOriented, const Mesh *enemyMesh, GLuint textureName);

  /* Restituisce la posizione del nemico */
  Point3 getPosition() const {
      return position;
  }

  /* Restituisce il raggio di azione del nemico */
  float getRadius() const {
      return 0.5f;
  }

  /* Resa grafica del nemico */
  void render(const Game &game) const;

  /* Esegue un passo di fisica */
  void doStep(const StarDestroyer &sd);

  /* Angoli di eulero in grado per la rotazione dell'X-Wing */
  Vector3 rotation;
};
