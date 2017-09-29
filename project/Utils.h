#pragma once
#include "point3.h"

class Point2;

namespace utils {
  /* Float casuale fra 0 e 1 */
  float random_float();

  float get_elapsed_seconds();
  float radiansToDegrees(const float angleRadians);
  float distance(const Point3& A, const Point3& B);

  /* setta le matrici di trasformazione in modo
  che le coordinate in spazio oggetto siano le coord
  del pixel sullo schermo */
  void setCoordToPixel(const Point2& screenSize);
  GLenum checkGLError(const char* file, int line);
}
