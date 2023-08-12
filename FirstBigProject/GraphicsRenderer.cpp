#include "GraphicsRenderer.h"

GraphicsRenderer::GraphicsRenderer(Model* model) : _model(model) {
    _boundary = model->boundBox();
}

GraphicsRenderer::GraphicsRenderer(Model* model, QVector3D position) : _model(model), _position(position) {
    _boundary = model->boundBox();
}

GraphicsRenderer::~GraphicsRenderer() {
    if (_light != nullptr) {
        delete _light;
        _light = nullptr;
    }
}

void GraphicsRenderer::setModel(Model* model) {
    _model = model;
    _boundary = model->boundBox();
}

void GraphicsRenderer::move(QVector3D deltaVec) {
    _position += deltaVec;
}

void GraphicsRenderer::setPosition(QVector3D position) {
    _position = position;
}

void GraphicsRenderer::rotate(QVector3D axis, float deltaAngle) {
    _rotation.rotate( deltaAngle, axis);
    //_rotation = glm::rotate(_rotation, glm::radians(deltaAngle), axis);
}

void GraphicsRenderer::setRotation(QVector3D axis, float angle) {
    QMatrix4x4 cur;
    cur.setToIdentity();
    cur.rotate(angle, axis);
    _rotation = cur;
    //_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
}

void GraphicsRenderer::scale(float deltaScale) {
    _scale += QVector3D(deltaScale, deltaScale, deltaScale);
}

void GraphicsRenderer::setScale(float scale) {
    _scale = QVector3D(scale,scale, scale);
}

// ScopedLight ScopedLight::toWorldSpace(QVector4D modelMatrix) 
ScopedLight GraphicsRenderer::transformedLight() const {
    // Transform the light position to the world space
    return _light->toWorldSpace(modelMatrix());
}

ScopedLight* GraphicsRenderer::originalLight() const {
    return _light;
}

void GraphicsRenderer::makeLight() {
    _light = new ScopedLight(QVector3D(0.0f, 0.0f, 0.0f));
}

void GraphicsRenderer::disposeLight() {
    if (_light != nullptr) {
        delete _light;
        _light = nullptr;
    }
}

void GraphicsRenderer::render(Shader& shader) {
    // Check if initialized
    if (_model == nullptr) {
        Logger::error("Tries to render an uninitialized renderable object");
        return;
    }
    // Set model matrix
    //shader.setMat4("model", modelMatrix());

    // Render
    _model->render(shader);
}

void GraphicsRenderer::updateBoundary() {
    // Traverse every vertex in the transferred model and update the boundary
    Boundary newBoundary;
    QMatrix4x4 model = modelMatrix();
    for (auto& mesh : _model->meshes()) {
        for (auto& vertex : mesh.vertices()) {
            // 转换为世界坐标系中的位置
            QVector4D transformedVertex = model * QVector4D(vertex._position, 1.0f);
            // 
            newBoundary.updateControlPoints(QVector3D(transformedVertex));
        }
    }
    _boundary = newBoundary;
    Logger::debug("Boundary updated");
    //Logger::debug("[+] Bottom control point: " + std::to_string(newBoundary.bottomControlPoint().x) + ", " + std::to_string(newBoundary.bottomControlPoint().y) + ", " + std::to_string(newBoundary.bottomControlPoint().z));
    //Logger::debug("[+] Top control point: " + std::to_string(newBoundary.topControlPoint().x) + ", " + std::to_string(newBoundary.topControlPoint().y) + ", " + std::to_string(newBoundary.topControlPoint().z));

}

CollisionRecorder GraphicsRenderer::hit(const Ray& ray) const {
    // First test if the ray hits the boundary box
    if (!_boundary.hit(ray)) {
        return CollisionRecorder();
    }
    else {
        // If the ray hits the boundary box
        return _model->hit(ray, modelMatrix());
    }
}

