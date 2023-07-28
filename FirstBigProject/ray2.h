#pragma once

#include <GLM/glm.hpp>
#include <QVector3D>

class Ray2 {
private:
    QVector3D _origin = QVector3D(0.0f,0.0f, 0.0f);
    QVector3D _direction = QVector3D(0.0f, 0.0f, -1.0f);

public:
    Ray2() {}
    Ray2(QVector3D  origin, QVector3D  direction) : _origin(origin), _direction(direction) {}

public:
    inline QVector3D  origin() const { return _origin; }
    inline QVector3D  direction() const { return _direction; }
};