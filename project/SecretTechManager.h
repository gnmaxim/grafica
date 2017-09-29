#pragma once
#include <vector>
#include "SecretTech.h"


class Game;
class StarDestroyer;
class SecretTech;

class SecretTechManager {
  private:
    std::vector<SecretTech> allSecretTech{};
    static constexpr float spawnHeight = 91.f;
    float nextSpawn = 11.f;
    const float spawnInterval = 7.f;

    void handleSecretTechSpawning(const Game& game);
    void checkImpact(Game &game);
    bool doesCollide(const SecretTech& pieceOfSecretTech, const Game& game) const;
    void spawnPieceOfSecretTech(const Game& game);

  public:
    void renderSecretTech(const Game& game) const;
    void doStep(Game &game);
    std::vector<Point3> getSecretTechPositions(const Game& game) const;
  };
