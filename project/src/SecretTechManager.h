#pragma once
#include <vector>
#include "SecretTech.h"


class Game;
class StarDestroyer;
class SecretTech;

class SecretTechManager {
  private:
    std::vector<SecretTech> allSecretTech{};
    static constexpr float spawnHeight = 80.f;
    float nextSpawnDate = 6.f;
    const float spawnInterval = 3.f;

    void handleSecretTechSpawning(const Game& game);
    void handleCollisions(Game &game);
    bool doesCollide(const SecretTech& pieceOfSecretTech, const Game& game) const;
    void spawnPieceOfSecretTech(const Game& game);

  public:
    void renderSecretTech(const Game& game) const;
    void doStep(Game &game);
    std::vector<Point3> getSecretTechPositions(const Game& game) const;
  };
