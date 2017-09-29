/*
	Last update: 28/09/2017
*/

#pragma once
#include "point2.h"
#include "point3.h"
#include <vector>

class Game;
class Point3;


class Space {
	private:
		void drawDeepSpace() const;

	public:
		/* Ampiezza dello spazio giocabile */
		static constexpr float playableWidth = 300.f;

		/* Valore Y del piano che rappresenta "il fondo" dello spazio giocabile */
		static constexpr float topY = -110.f;

		/* Valore Y del piano che rappresenta "il tetto" dello spazio giocabile */
		static constexpr float bottomY = +110.f;

		/* Ampiezza dello spazio disegnato */
		static constexpr float renderedSpace = 3000.f;

		/* Render della scena */
		void render(const Game& game) const;
};
