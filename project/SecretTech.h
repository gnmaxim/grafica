#pragma once
#include "point3.h"



class SecretTech {
  private:
    Point3 initialPosition;
    float spawnTime;
    float fallSpeed;

  public:
    /* Si noti che si tratta di angoli di Eulero in gradi */
    Vector3 rotation;

    SecretTech(const Point3 &initialPosition,
                const float spawnTime,
                const Vector3 &rotation);

    Point3 getPosition(const float playedSeconds) const;
    void render(const float playedSeconds, bool renderShadows) const;
    float getRadius() const { return 1.f; }
};
