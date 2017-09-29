/*
	28/09/2017
*/
#pragma once
#include "point2.h"
#include "Space.h"

class Game;
class StarDestroyer;
class ResistanceFleet;
class SecretTechManager;
class SecretTech;


class SpaceScanner {
	private:
	  const Vector2 spaceScannerSize = Vector2{400.f, 400.f};

	  /* Offset dai bordi dello schermo */
	  const float offsetFromLeft = 70.f;
	  const float offsetFromBottom = 60.f;

		/* Dimensioni dei punti */
	  const float pntSize = 6.f;

		/* Colori dei simboli sullo scanner */
	  const float colorStarDestroyer[3] = {1.f, 0.f, 1.f};
	  const float colorXWing[3] = {1.f, 0.f, 0.f};
	  const float colorArtefact[3] = {0.f, 0.f, 1.f};

		/* Per quanti riguarda il disegno dei bordi */
	  const float colorSpaceBorder[3] = { 0.f, 0.f, 1.f };
	  const float widthBorder = 5.f;

	public:
		bool centersOnPlayer = true;
		bool rotatesWithPlayer = true;

		SpaceScanner() {}
		void render(const Game& game) const;
};
