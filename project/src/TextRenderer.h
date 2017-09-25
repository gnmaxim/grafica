#pragma once
#include "point2.h"

enum fontSize {ultra, mid, big, small};
enum textquality {solid, shaded, blended};

class TextRenderer {
public:

    TTF_Font *smallFont;
    TTF_Font *midFont;
    TTF_Font *bigFont;
    TTF_Font *ultraFont;
    void initialize();
    void render(SDL_Color textColor, SDL_Color backgroundColor, const char *text, Point2 position, enum textquality quality,
                    enum fontSize fontSize, bool wrap) const;

private:
    /* Maschere RGB per creare superfici RGB */
    Uint32 rmask, gmask, bmask, amask;

};
