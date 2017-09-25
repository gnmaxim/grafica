#include "Game.h"
#include "EnemyManager.h"
#include "Utils.h"
#include "MenuManager.h"


/* Caricamento mesh dell'X-Wing */
Mesh shark((char *) "../assets/obj/chark.obj");


void EnemyManager::renderEnemies(const Game& game) const {
  for (const Enemy &enemy : allEnemies)
    enemy.render(game);

	utils::checkGLError(__FILE__, __LINE__);

  return;
}

void EnemyManager::doStep(Game &game) {
  for (Enemy &enemy : allEnemies) {
    enemy.doStep(game.sD);
  }

  handleEnemySpawning(game);
  handleCollisions(game.sD, game);

  return;
}

void EnemyManager::spawnEnemy(const Game& game) {
  Point3 spawnPosition;
  if (getRandomSpawnPosition(game.sD.getPosition(), /*out*/ spawnPosition))
    allEnemies.emplace_back(spawnPosition, /* velocità */ 10.f, /* scala */ 0.4f, /* rotazione */ Point3{0.f, 0.f, 0.f},
          /* rotazione verso giocatore */ true, /* animazione */ true, /* mesh */ &shark, /* texture */ Texture::Shark);

  return;
}

std::vector<Point3> EnemyManager::getEnemyPositions() const {
	std::vector<Point3> enemyPositions{};

	for (const Enemy& enemy : allEnemies)
		enemyPositions.push_back(enemy.getPosition());

	return enemyPositions;
}

void EnemyManager::handleCollisions(const StarDestroyer &sD, Game &game) {
  auto positions = getEnemyPositions();
  Point3 sDPosition = sD.getPosition();

  for (const Point3& enemyPosition : positions)
    if ( utils::distance(enemyPosition, sDPosition) < sD.getRadius() )
        game.setState(GameState::GameOver);

  return;
}

void EnemyManager::handleEnemySpawning(const Game &game) {
  float secondsNow = game.getGameSeconds();
  if (secondsNow > nextSpawnDate) {
    if (allEnemies.size() < maxEnemyCount)
      spawnEnemy(game);

    nextSpawnDate += getNextSpawnInterval();
    ++spawnWave;
  }

  return;
}

float EnemyManager::getNextSpawnInterval() const {
  return baseSpawnInterval * powf(1.f - spawnIntervalReductionFactor, (float) spawnWave);
}

bool EnemyManager::getRandomSpawnPosition(const Point3& sDPosition, Point3& outSpawnPosition) const
{
	static constexpr int maxAttempts = 89;
	for (int loopGuard = 0; loopGuard < maxAttempts; loopGuard++)
	{
    const Point3 randomPosition = Point3{utils::random_float() * Space::width - Space::width / 2.f,
                                          utils::random_float() *	100.f, // Condizionamento dello spawn sopra
                                          utils::random_float() * Space::width - Space::width / 2.f};

    if (utils::distance(randomPosition, sDPosition) > minSpawnDistanceToPlayer) {
    	outSpawnPosition = randomPosition;

      /* Se la posizione di spawn è stata trovata */
    	return true;
    }
	}

  /* Se non è stata trovata */
	return false;
}
