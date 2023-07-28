#pragma once

#include <GLM/glm.hpp>
#include <QVector3D>
struct Vertex {
    QVector3D _position = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D _normal = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D _texCoords = QVector3D(0.0f, 0.0f, 0.0f);
    //glm::vec3 _tangent = glm::vec3(0.0f);
    //glm::vec3 _bitangent = glm::vec3(0.0f);

    Vertex();
    Vertex(QVector3D position);
    Vertex(QVector3D position, QVector3D normal);
    Vertex(QVector3D position, QVector3D normal, QVector3D texCoords);
    //Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec3 tangent, glm::vec3 bitangent);
};

