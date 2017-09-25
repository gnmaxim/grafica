#pragma once
#include "point3.h"


class SecretTech {
  private:
    Point3 initialPosition;
    float spawnTime; // in seconds
    float fallSpeed;

  public:
    Vector3 rotation; // angoli di Eulero, in gradi
    
    SecretTech(const Point3 &initialPosition,

                const float spawnTime,
                const float fallSpeed,
                const Vector3 &rotation);

    Point3 getPosition(const float playedSeconds) const;
    void render(const float playedSeconds, bool renderShadows) const;
    float getRadius() const { return 1.f; }
};
