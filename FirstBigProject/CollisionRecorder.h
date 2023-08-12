#pragma once
#include <QVector3D>

class CollisionRecorder {
private:
    bool _hitted;
    float _t;
    QVector3D _position = QVector3D(0.0f,0.0f,0.0f);
    QVector3D _normal = QVector3D(0.0f, 0.0f, 0.0f);

public:
    CollisionRecorder() : _hitted(false), _t(std::numeric_limits<float>::max()) {}
    CollisionRecorder(float t, QVector3D position, QVector3D normal) : _hitted(true), _t(t), _position(position), _normal(normal) {}

public:
    inline bool hitted() const { return _hitted; }
    inline float t() const { return _t; }
    inline QVector3D position() const { return _position; }
    inline QVector3D normal() const { return _normal; }
};
