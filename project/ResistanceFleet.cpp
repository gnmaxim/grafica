#include "Game.h"
#include "ResistanceFleet.h"
#include "Utils.h"
#include "Menu.h"


/* Caricamento mesh dell'X-Wing */
Mesh xwing((char *) "mesh/xwing.obj");


void ResistanceFleet::renderEnemies(const Game& game) const {
  for (const XWing &enemy : xWings)
    enemy.render(game);

	utils::checkGLError(__FILE__, __LINE__);

  return;
}

void ResistanceFleet::doStep(Game &game) {
  for (XWing &enemy : xWings) {
    enemy.doStep(game.sd);
  }

  prepareSpawn(game);
  checkImpact(game.sd, game);

  return;
}

void ResistanceFleet::spawn(const Game& game) {
  Point3 spawnPosition;
  if (generateSpawnPos(game.sd.getPosition(), spawnPosition))
    xWings.emplace_back(spawnPosition, 10.f, 0.4f, Point3{0.f, 0.f, 0.f}, true, &xwing, Texture::XWing);

  return;
}

std::vector<Point3> ResistanceFleet::getXWingPositions() const {
	std::vector<Point3> enemyPositions{};

	for (const XWing& enemy : xWings)
		enemyPositions.push_back(enemy.getPosition());

	return enemyPositions;
}

void ResistanceFleet::checkImpact(const StarDestroyer &sd, Game &game) {
  auto positions = getXWingPositions();
  Point3 sdPosition = sd.getPosition();

  int k = 0;
  for (const Point3& enemyPosition : positions) {
    if (utils::distance(enemyPosition, sdPosition) < sd.getRadius())
      if (sd.getUltraFuel(game) > 0)
        game.setState(GameStatus::GAME_OVER);
      else
        xWings.erase(xWings.begin() + k);
    k++;
  }

  return;
}

void ResistanceFleet::prepareSpawn(const Game &game) {
  float secondsNow = game.getGameSeconds();
  if (secondsNow > nextSpawn) {
    if (xWings.size() < maxWings)
      spawn(game);

    nextSpawn += nextSpawnGap();
    ++attackWave;
  }

  return;
}

float ResistanceFleet::nextSpawnGap() const {
  return initialSpawnGap * powf(1.f - spawnIntensificator, (float) attackWave);
}

bool ResistanceFleet::generateSpawnPos(const Point3& sdPosition, Point3& outSpawnPosition) const
{
  bool spawnFound = false;
  Point3 randomPosition;

  /* Ripeti al massimo per un certo numero di volte */
  do {
    /* Gli xWing saranno generati da qualche parte nello spazio giocabile con coordinate casuali */
    float randomX = utils::random_float() * Space::playableWidth - Space::playableWidth / 2.f;
    float randomY = utils::random_float() *	(Space::topY - Space::bottomY);
    float randomZ = utils::random_float() * Space::playableWidth - Space::playableWidth / 2.f;

    randomPosition = Point3{randomX, randomY, randomZ};
    if (utils::distance(randomPosition, sdPosition) > minSpawnDistance) {
    	outSpawnPosition = randomPosition;
      /* Se la posizione di spawn è stata trovata */
    	spawnFound = true;
    }
  }
	while (utils::distance(randomPosition, sdPosition) <= minSpawnDistance);

	return spawnFound;
}
