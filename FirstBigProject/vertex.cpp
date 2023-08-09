#include "vertex.h"

Vertex::Vertex(){}

Vertex::Vertex(QVector3D position) :
    _position(position) {}

Vertex::Vertex(QVector3D position, QVector3D normal) :
    _position(position), _normal(normal) {}

Vertex::Vertex(QVector3D position, QVector3D normal, QVector2D texCoords) :
    _position(position), _normal(normal), _texCoords(texCoords) {}

