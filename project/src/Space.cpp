#include "Space.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"

// Variabili di tipo mesh
static Mesh rockMesh((char *) "../assets/obj/rock.obj");


void Space::drawSand(const Point3& sDPosition) const {

  glBindTexture(GL_TEXTURE_2D, Texture::Sand);
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.f, 1.f, 1.f);
  glEnable(GL_LIGHTING);

  // Imposto le proprietà del materiale
  {
    float specular[4] = {0.f, 0.f, 0.f, 1.f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    float diffuse[4] = {0.f, 0.f, 0.f, 1.f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    float ambient[4] = {0.0f, 0.0f, 0.0f, 1.f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialf(GL_FRONT, GL_SHININESS, 32.f);
  }

  // Terreno a basso dettaglio
  drawPlane(terrainWidth, /*quadsPerSide*/ 1, /*tileSize*/ 1000.f);
	drawTop(terrainWidth, /*quadsPerSide*/ 1, /*tileSize*/ 1000.f);

  // Terreno ad alto dettaglio
  //drawPlane(terrainWidth/10.f, /*quadCount*/ 150, /*tileSize*/ 5.f, Point3{sDPosition.X(), 0.05f, sDPosition.Z()});

  glDisable(GL_TEXTURE_2D);
  utils::checkGLError(__FILE__, __LINE__);
}

void Space::drawPlane(const float width, const int quadsPerSide, const float tileSize, const Point3& center) const {

	// Reference: http://www.glprogramming.com/red/chapter09.html#name7
    // la funzione di generazione è una combinazione lineare delle coordinate dei vertici; piano di riferimento in coord mondo
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	float sLinearFactors[] = {1.f/tileSize, 0.f, 0.f, 0.f};
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sLinearFactors); // parametri per la funzione di generazione della texture
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	float tLinearFactors[] = {0.f, 0.f, 1.f/tileSize, 0.f};
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tLinearFactors);
	glEnable(GL_TEXTURE_GEN_T);

	  // Disegno quadCount x quadCount quad
	  glBegin(GL_QUADS);
	  {
		const float cx = center.X();
		const float cy = -500;
		const float cz = center.Z();
		glNormal3f(0.0f, 1.0f, 0.0f); // normale verticale uguale per tutti
		for (int x = 0; x < quadsPerSide; x++)
		  for (int z = 0; z < quadsPerSide; z++) {
			float x0 = -width + 2 * (x + 0) * width / quadsPerSide;
			float x1 = -width + 2 * (x + 1) * width / quadsPerSide;
			float z0 = -width + 2 * (z + 0) * width / quadsPerSide;
			float z1 = -width + 2 * (z + 1) * width / quadsPerSide;
			glVertex3f(x0 + cx, cy, z0 + cz);
			glVertex3f(x1 + cx, cy, z0 + cz);
			glVertex3f(x1 + cx, cy, z1 + cz);
			glVertex3f(x0 + cx, cy, z1 + cz);
		  }
	  }
	  glEnd();
	  glDisable(GL_TEXTURE_GEN_S);
	  glDisable(GL_TEXTURE_GEN_T);
	  utils::checkGLError(__FILE__, __LINE__);
}

void Space::drawTop(const float width, const int quadsPerSide, const float tileSize, const Point3& center) const {

	glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// Reference: http://www.glprogramming.com/red/chapter09.html#name7
    // la funzione di generazione è una combinazione lineare delle coordinate dei vertici; piano di riferimento in coord mondo
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	float sLinearFactors[] = {1.f/tileSize, 0.f, 0.f, 0.f};
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sLinearFactors); // parametri per la funzione di generazione della texture
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	float tLinearFactors[] = {0.f, 0.f, 1.f/tileSize, 0.f};
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tLinearFactors);
	glEnable(GL_TEXTURE_GEN_T);

	  // Disegno quadCount x quadCount quad
	  glBegin(GL_QUADS);
	  {
		const float cx = center.X();
		const float cy = 500;
		const float cz = center.Z();
		glNormal3f(0.0f, 1.0f, 0.0f); // normale verticale uguale per tutti
		for (int x = 0; x < quadsPerSide; x++)
		  for (int z = 0; z < quadsPerSide; z++) {
			float x0 = -width + 2 * (x + 0) * width / quadsPerSide;
			float x1 = -width + 2 * (x + 1) * width / quadsPerSide;
			float z0 = -width + 2 * (z + 0) * width / quadsPerSide;
			float z1 = -width + 2 * (z + 1) * width / quadsPerSide;
			glVertex3f(x0 + cx, cy, z0 + cz);
			glVertex3f(x1 + cx, cy, z0 + cz);
			glVertex3f(x1 + cx, cy, z1 + cz);
			glVertex3f(x0 + cx, cy, z1 + cz);
		  }
	  }
	  glEnd();
	  glDisable(GL_TEXTURE_GEN_S);
	  glDisable(GL_TEXTURE_GEN_T);
	  utils::checkGLError(__FILE__, __LINE__);
}


void Space::drawCaustics(const Game& game) const {}

// Cubo esterno
void drawCubeFill(const float size) {
  const float S = size / 2.f;

  glBegin(GL_QUADS);

    // GL LINES PER FARE UNA LINEA VERSO IL NEMICO

  glNormal3f(0, 0, +1);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(+S, +S, +S);
  glTexCoord2f(1.f, 0.0f);
  glVertex3f(-S, +S, +S);
  glTexCoord2f(1.f, 1.f);
  glVertex3f(-S, -S, +S);
  glTexCoord2f(0.0f, 1.f);
  glVertex3f(+S, -S, +S);

  glNormal3f(0, 0, -1);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(+S, -S, -S);
  glTexCoord2f(1.f, 1.0f);
  glVertex3f(-S, -S, -S);
  glTexCoord2f(1.f, 0.f);
  glVertex3f(-S, +S, -S);
  glTexCoord2f(0.0f, 0.f);
  glVertex3f(+S, +S, -S);

  glNormal3f(0, +1, 0);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(+S, +S, +S);
  glTexCoord2f(1.f, 0.0f);
  glVertex3f(-S, +S, +S);
  glTexCoord2f(1.f, 1.f);
  glVertex3f(-S, +S, -S);
  glTexCoord2f(0.0f, 1.f);
  glVertex3f(+S, +S, -S);

	glNormal3f(  0,-1,0  );
	glTexCoord2f(1.f, 0.f);
	glVertex3f( +S,-S,-S );
	glTexCoord2f(1.f, 1.0f);
	glVertex3f( -S,-S,-S );
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( -S,-S,+S );
	glTexCoord2f(0.0f, 0.f);
	glVertex3f( +S,-S,+S );

  glNormal3f(+1, 0, 0);
  glTexCoord2f(0.0f, 0.f);
  glVertex3f(+S, +S, +S);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(+S, -S, +S);
  glTexCoord2f(1.f, 1.0f);
  glVertex3f(+S, -S, -S);
  glTexCoord2f(1.f, 0.f);
  glVertex3f(+S, +S, -S);

  glNormal3f(-1, 0, 0);
  glTexCoord2f(0.0f, 0.f);
  glVertex3f(-S, +S, -S);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-S, -S, -S);
  glTexCoord2f(1.f, 1.0f);
  glVertex3f(-S, -S, +S);
  glTexCoord2f(1.f, 0.f);
  glVertex3f(-S, +S, +S);

  glEnd();
  utils::checkGLError(__FILE__, __LINE__);
}

void Space::drawWaterBackground() const {
  glColor3f(1.f, 1.f, 1.f);
  glDisable(GL_LIGHTING);
  glPushMatrix();
  {
    glBindTexture(GL_TEXTURE_2D, Texture::WaterBackground);
    glEnable(GL_TEXTURE_2D);
    static constexpr float skyboxSize = 1000.f;
    glTranslatef(0.f, -1.f, 0.f);

    drawCubeFill(skyboxSize);

    glDisable(GL_TEXTURE_2D);
  }
  glPopMatrix();
  glEnable(GL_LIGHTING);
  utils::checkGLError(__FILE__, __LINE__);
}

void Space::initialize()
{
	for (int i = 0; i < rockCount; i++)
	{
		Vector3 position = Point3{
			utils::random_float() * width - width / 2.f,
			0.f,
			utils::random_float() * width - width / 2.f
		};
		const float Scale = (1.f + utils::random_float()) / 10000000.f;
		const Vector3 scale = Vector3{Scale,Scale,Scale};
		const Vector3 rotation = Vector3{0.f, utils::random_float()*360.f, 0.f};
		rocks.emplace_back(position, scale, rotation);
	}
}

void Space::render(const Game& game) const {
  drawWaterBackground();
	drawSand(game.sD.getPosition());
  utils::checkGLError(__FILE__, __LINE__);

	return;
}

void Space::drawRocks() const {
	return;
}
