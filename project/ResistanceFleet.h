#pragma once
#include <vector>
#include <SDL2/SDL_ttf.h>
#include "XWing.h"


class Game;
class StarDestroyer;
class Menu;
class Point3;

class ResistanceFleet {
  private:
    std::vector<XWing> xWings;
    static constexpr int maxWings = 20;
    static constexpr float minSpawnDistance = 31.f;
    float nextSpawn = 9.f;
    int attackWave = 0;
    static constexpr float spawnIntensificator = 0.0069f;
    static constexpr float initialSpawnGap = 5.f;

    /* Controllo della collisione */
    void checkImpact(const StarDestroyer &sd, Game &game);

    void prepareSpawn(const Game& game);
    void spawn(const Game& game);
    float nextSpawnGap() const;
    bool generateSpawnPos(const Point3& sdPosition, Point3& outSpawnRadius) const;

  public:
    void renderEnemies(const Game& game) const;
    void doStep(Game &game);
    std::vector<Point3> getXWingPositions() const;
};
