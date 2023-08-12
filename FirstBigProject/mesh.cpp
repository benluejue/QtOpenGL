#include "mesh.h"
#include "utils.h"
#include "logger.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture2D>& textures) {
    _vertices = vertices;
    _indices = indices;
    _textures = textures;

    setupMesh();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Texture2D>& textures) {
    _vertices = vertices;
    _textures = textures;

    setupMesh();
}

void Mesh::render(Shader& shader) const {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    //shader.setUniform("material.shininess", _shininess);
    for (int i = 0; i < _textures.size(); i++) {
        OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = _textures[i].type() == TextureType::DIFFUSE ? "texture_diffuse" : "texture_specular";
        name = "material." + name;

        if (_textures[i].type() == TextureType::DIFFUSE)
            number = std::to_string(diffuseNr++);
        else if (_textures[i].type() == TextureType::SPECULAR)
            number = std::to_string(specularNr++);
        // *************可能报错**********
        shader.setInt(QString::fromStdString(name+number),i);
        //shader.setUniform(name + number, i);
        _textures[i].bind();
    }
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0);

    _vao.bind();
    OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    _vao.unbind();
}

void Mesh::setupMesh() {
    _vao = VertexArrayObject(_vertices, _indices);
    _vao.setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    _vao.setVertexAttributePointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _normal));
    _vao.setVertexAttributePointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _texCoords));
    //_vao.setVertexAttributePointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _tangent));
    //_vao.setVertexAttributePointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, _bitangent));
}


CollisionRecorder Mesh::hit(const Ray& ray, const QMatrix4x4& modelMatrix) const {
    // Test whether the ray hits the mesh

    // Traverse the indices to test every triangle
    for (int i = 0; i < _indices.size(); i += 3) {
        // Get the three vertices of the triangle
        Vertex v0 = _vertices[_indices[i]];
        Vertex v1 = _vertices[_indices[i + 1]];
        Vertex v2 = _vertices[_indices[i + 2]];

        QVector3D v0pos = QVector3D(modelMatrix * QVector4D(v0._position, 1.0f));
        QVector3D v1pos = QVector3D(modelMatrix * QVector4D(v1._position, 1.0f));
        QVector3D v2pos = QVector3D(modelMatrix * QVector4D(v2._position, 1.0f));

        // Moller Trumbore algorithm
        // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
        QVector3D edge1 = v1pos - v0pos;
        QVector3D edge2 = v2pos - v0pos;
        // 这个
        QVector3D pvec = QVector3D::crossProduct(ray.direction(), edge2);
        // float det = glm::dot(edge1, pvec);
        float det =  QVector3D::dotProduct(edge1, pvec);
        if (abs(det) < 0.0001f) {
            continue;
        }
        float invDet = 1.0f / det;
        QVector3D tvec = ray.origin() - v0pos;
        float u = QVector3D::dotProduct(tvec, pvec) * invDet;
        if (u < 0.0f || u > 1.0f) {
            continue;
        }
        QVector3D qvec = QVector3D::crossProduct(tvec, edge1);
        float v = QVector3D::dotProduct(ray.direction(), qvec) * invDet;
        if (v < 0.0f || u + v > 1.0f) {
            continue;
        }
        float t = QVector3D::dotProduct(edge2, qvec) * invDet;
        if (t > 0.0001f) {
            // Hit
            QVector3D normal = QVector3D::crossProduct(edge1, edge2).normalized();
            QVector3D hitPoint = ray.origin() + t * ray.direction();
            return CollisionRecorder(t, hitPoint, normal);
        }
    }
    return CollisionRecorder();
}
