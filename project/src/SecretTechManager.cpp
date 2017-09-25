/*
  22/09/2017
*/

#include "SecretTechManager.h"
#include "Utils.h"
#include "Space.h"
#include "Game.h"
#include <algorithm>

class Game;

/* Disegna tutti i pezzi di junk sullo schermo */
void SecretTechManager::renderSecretTech(const Game& game) const {
    for (const SecretTech &pieceOfSecretTech : allSecretTech) {
        pieceOfSecretTech.render(game.getGameSeconds(), game.renderShadows);
    }
	utils::checkGLError(__FILE__, __LINE__);
}

/* TRUE se c'è una collisione tra i 2 oggetti */
bool SecretTechManager::doesCollide(const SecretTech &pieceOfSecretTech, const Game& game) const {
    const Point3 junkPosition = pieceOfSecretTech.getPosition(game.getGameSeconds());
    const Point3 sDPosition = game.sD.getPosition();
    return utils::distance(junkPosition, sDPosition) < pieceOfSecretTech.getRadius() + game.sD.getRadius();
}

void SecretTechManager::handleCollisions(Game &game) {
    const float secondsNow = game.getGameSeconds();

    // Lambda che restituisce TRUE se la y del pieceOfSecretTech è minore del terrainHeight */
    const auto collidesWithTerrain = [&](const SecretTech &pieceOfSecretTech) {
        return pieceOfSecretTech.getPosition(secondsNow).Y() <
               Space::terrainHeight - pieceOfSecretTech.getRadius();

    };

    /* Lambda perchè erase prende in input una funzione con un solo argomento SecretTech */
    /* Restituisce TRUE se oggetto ha una collisione con il sottomarino o il terreno */
    const auto isSecretTechDestroyed = [&](const SecretTech &pieceOfSecretTech) {
        return doesCollide(pieceOfSecretTech, game) || collidesWithTerrain(pieceOfSecretTech);
    };

    /* Incrementa punteggio se necessario */
    for (const SecretTech& pieceOfSecretTech : allSecretTech){
        if (doesCollide(pieceOfSecretTech, game)) {
            game.sD.fillTurboCharge(game);
            game.incrementScore(50000);
        }
    }

    /* Rimuove dal vettore tutti i junk che hanno una collisione con il sottomarino */
    allSecretTech.erase(
            std::remove_if(
                    allSecretTech.begin(),
                    allSecretTech.end(),
                    isSecretTechDestroyed),
            allSecretTech.end());

}

void SecretTechManager::handleSecretTechSpawning(const Game& game) {
    float secondsNow = game.getGameSeconds();
    if (secondsNow > nextSpawnDate) {
        spawnPieceOfSecretTech(game);
        nextSpawnDate += spawnInterval;
    }
}

void SecretTechManager::doStep(Game &game) {

    handleSecretTechSpawning(game);
    handleCollisions(game);

}

std::vector<Point3> SecretTechManager::getSecretTechPositions(const Game& game) const
{
	std::vector<Point3> junkPositions{};
	const float elapsedSeconds = game.getGameSeconds();
	for (const SecretTech& pieceOfSecretTech : allSecretTech)
	{
		junkPositions.push_back(pieceOfSecretTech.getPosition(elapsedSeconds));
	}
	return junkPositions;
}

void SecretTechManager::spawnPieceOfSecretTech(const Game& game) {
    const Point3 junkRandomPosition = Point3{utils::random_float() * Space::width - Space::width / 2.f,
                                             spawnHeight,
                                             utils::random_float() * Space::width - Space::width / 2.f
    };
    const Vector3 junkRandomRotation = Vector3{utils::random_float()*360.f, // angolo random tra 0 e 360
                                               utils::random_float()*360.f,
                                               utils::random_float()*360.f
    };
    // Aggiungiamo i detriti al vettore, costruendoli in-place
    allSecretTech.emplace_back(
            junkRandomPosition,
            /*spawnTime in seconds*/game.getGameSeconds(),
            /*fallSpeed*/3.f,
            junkRandomRotation);
}
