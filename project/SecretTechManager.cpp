/*
  22/09/2017
*/

#include "SecretTechManager.h"
#include "Utils.h"
#include "Space.h"
#include "Game.h"
#include <algorithm>


class Game;

/* Disegna tutti gli artefatti */
void SecretTechManager::renderSecretTech(const Game& game) const {
  for (const SecretTech &pieceOfSecretTech : allSecretTech) {
    pieceOfSecretTech.render(game.getGameSeconds(), game.renderShadows);
  }
	utils::checkGLError(__FILE__, __LINE__);

  return;
}


bool SecretTechManager::doesCollide(const SecretTech &pieceOfSecretTech, const Game& game) const {
    const Point3 artefactPos = pieceOfSecretTech.getPosition(game.getGameSeconds());
    const Point3 sdPosition = game.sd.getPosition();

    /* Il valore del ritorno è vero se accade collisione con entro un certo raggio */
    return utils::distance(artefactPos, sdPosition) < pieceOfSecretTech.getRadius() + game.sd.getRadius();
}

/* Gestione delle collisioni con degli artefatti */
void SecretTechManager::checkImpact(Game &game) {
  const float secondsNow = game.getGameSeconds();

  const auto collidesWithTerrain = [&](const SecretTech &pieceOfSecretTech) {
      return pieceOfSecretTech.getPosition(secondsNow).Y() < Space::topY - pieceOfSecretTech.getRadius(); };

  /* Il valore di ritorno è vero tech collide con il fondo del cubo o la nave */
  const auto isSecretTechDestroyed = [&](const SecretTech &pieceOfSecretTech) {
      return doesCollide(pieceOfSecretTech, game) || collidesWithTerrain(pieceOfSecretTech); };

  /* Se collisione avvenuta vengono aggiunti punteggi */
  for (const SecretTech& pieceOfSecretTech : allSecretTech) {
    if (doesCollide(pieceOfSecretTech, game)) {
      game.sd.fillUltraFuel(game);
      game.addPoints(50000);
    }
  }

  /* Quindi c'è rimozione se dal vettore degli artefatti se è stato raccolto */
  allSecretTech.erase(std::remove_if(allSecretTech.begin(), allSecretTech.end(), isSecretTechDestroyed), allSecretTech.end());

  return;
}

void SecretTechManager::handleSecretTechSpawning(const Game& game) {
  float secondsNow = game.getGameSeconds();
  if (secondsNow > nextSpawn) {
    spawnPieceOfSecretTech(game);
    nextSpawn += spawnInterval;
  }

  return;
}

void SecretTechManager::doStep(Game &game) {
  handleSecretTechSpawning(game);
  checkImpact(game);

  return;
}

std::vector<Point3> SecretTechManager::getSecretTechPositions(const Game& game) const
{
	std::vector<Point3> artefactPoss{};
	const float elapsedSeconds = game.getGameSeconds();
	for (const SecretTech& pieceOfSecretTech : allSecretTech)
	{
		artefactPoss.push_back(pieceOfSecretTech.getPosition(elapsedSeconds));
	}

	return artefactPoss;
}

void SecretTechManager::spawnPieceOfSecretTech(const Game& game) {
  /* Genera casualmente valori di posizione nello spazio giocabile */
  const Point3 randomPos = Point3{utils::random_float() * Space::playableWidth - Space::playableWidth / 2.f, utils::random_float() * spawnHeight,
                                           utils::random_float() * Space::playableWidth - Space::playableWidth / 2.f};

  /* Genera casualmente valori di rotazione fino a 360 gradi */
  const Vector3 randomRot = Vector3{utils::random_float() * 360.f, utils::random_float() * 360.f, utils::random_float() * 360.f};

  /* Aggiunta del nuovo artefatto.
    In ordine i parametri attuali sono: posizione, tempo di spawn, velocità di movimento, rotazione casuale */
  allSecretTech.emplace_back(randomPos, game.getGameSeconds(), randomRot);

  return;
}
