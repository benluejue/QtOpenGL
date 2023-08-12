#include "LightingMethods.h"
#include "logger.h"

LightingMethods::LightingMethods(QVector3D color) :
    _lightColor(color)
{}

LightingMethods::~LightingMethods() {}

DirLight::DirLight(QVector3D direction, QVector3D color) :
    LightingMethods(color), _direction(direction)
{}

DirLight::~DirLight() {}

void DirLight::updateShader(Shader shader, int index) const {
    // Recall DirLight structure in fragment shader
    // -------------
    // struct DirLight {
    //     vec3 direction;
    //     vec3 ambient;
    //     vec3 diffuse;
    //     vec3 specular;
    // };

   /* Logger::debug("Setting dir light " + std::to_string(index) + " to shader");
    Logger::debug("[+] Direction: " + std::to_string(_direction.x) + ", " + std::to_string(_direction.y) + ", " + std::to_string(_direction.z));
    Logger::debug("[+] Ambient: " + std::to_string(ambientLightColor().x) + ", " + std::to_string(ambientLightColor().y) + ", " + std::to_string(ambientLightColor().z));
    Logger::debug("[+] Diffuse: " + std::to_string(diffuseLightColor().x) + ", " + std::to_string(diffuseLightColor().y) + ", " + std::to_string(diffuseLightColor().z));
    Logger::debug("[+] Specular: " + std::to_string(specularLightColor().x) + ", " + std::to_string(specularLightColor().y) + ", " + std::to_string(specularLightColor().z));*/

    shader.setVec3(("dirlights[" + std::to_string(index) + "].direction").c_str(), _direction);
    shader.setVec3(("dirlights[" + std::to_string(index) + "].ambient").c_str(), ambientLightColor());
    shader.setVec3(("dirlights[" + std::to_string(index) + "].diffuse").c_str(), diffuseLightColor());
    shader.setVec3(("dirlights[" + std::to_string(index) + "].specular").c_str(), specularLightColor());
}

ScopedLight::ScopedLight(QVector3D position, QVector3D direction, QVector3D color) :
    LightingMethods(color), _position(position), _direction(direction), _idealDistance(32)
{
    updateLinear();
    updateQuadratic();
}

ScopedLight::ScopedLight(int distance, QVector3D position, QVector3D direction, QVector3D color) :
    LightingMethods(color), _position(position), _direction(direction), _idealDistance(distance)
{
    updateLinear();
    updateQuadratic();
}

ScopedLight::ScopedLight(QVector3D position, QVector3D direction, float cutOffAngle, int idealDistance, float linear, float quadratic, QVector3D color) :
    LightingMethods(color), _position(position), _direction(direction), _cutOffAngle(cutOffAngle), _idealDistance(idealDistance), _attLinear(linear), _attQuadratic(quadratic)
{}

ScopedLight::~ScopedLight() {}

inline void ScopedLight::updateLinear() {
    // Double is used here to prevent precision loss
    double linear = -0.0001 + 4.7688 / (double)_idealDistance;
    _attLinear = (float)linear;
}

inline void ScopedLight::updateQuadratic() {
    // Double is used here to prevent precision loss
    double quadratic = 82.4448 * std::pow((double)_idealDistance, -2.0192);

    _attQuadratic = (float)quadratic;
}

void ScopedLight::setIdealDistance(int distance) {
    if (distance < 5) {
        distance = 5;
    }
    if (distance > 3500) {
        distance = 3500;
    }
    _idealDistance = distance;
    updateLinear();
    updateQuadratic();
}

void ScopedLight::setCutOffAngle(float angle) {
    if (angle < 0.0f) {
        angle = 0.0f;
    }
    if (angle > 180.0f) {
        angle = 180.0f;
    }
    _cutOffAngle = angle;
}

inline float ScopedLight::innerCutOffAngle() const {
    return 0.0011 * std::pow(_cutOffAngle, 2) + 0.6440 * _cutOffAngle;
}

ScopedLight ScopedLight::toWorldSpace(QMatrix4x4 modelMatrix) const {
    QVector3D position = QVector3D(modelMatrix * QVector4D(_position, 1.0f));
    QVector3D direction = QVector3D(modelMatrix * QVector4D(_direction, 0.0f));
    /*glm::vec3 position = glm::vec3(modelMatrix * glm::vec4(_position, 1.0f));
    glm::vec3 direction = glm::vec3(modelMatrix * glm::vec4(_direction, 0.0f));*/
    return ScopedLight(position, direction, _cutOffAngle, _idealDistance, _attLinear, _attQuadratic, _lightColor);
}

void ScopedLight::updateShader(Shader shader, int index) const {
    // Recall PointLight and SpotLight structure in fragment shader
    // -------------
    // struct PointLight {
    //     vec3 position;
    //     vec3 ambient;
    //     vec3 diffuse;
    //     vec3 specular;
    //     float constant;
    //     float linear;
    //     float quadratic;
    // };
    // ------------
    // struct SpotLight {
    //     vec3 position;
    //     vec3 direction;
    //     vec3 ambient;
    //     vec3 diffuse;
    //     vec3 specular;
    //     float constant;
    //     float linear;
    //     float quadratic;
    //     float cutOff;
    //     float outerCutOff;
    // };

    // Check the cutoff angle to determine the type of light
    if (abs(_cutOffAngle - 180.0f) < 1e-6) {
        // Point light
        /*Logger::debug("Setting point light " + std::to_string(index) + " to shader");
        Logger::debug("[+] Position: " + std::to_string(_position.x) + ", " + std::to_string(_position.y) + ", " + std::to_string(_position.z));
        Logger::debug("[+] Ambient: " + std::to_string(ambientLightColor().x) + ", " + std::to_string(ambientLightColor().y) + ", " + std::to_string(ambientLightColor().z));
        Logger::debug("[+] Diffuse: " + std::to_string(diffuseLightColor().x) + ", " + std::to_string(diffuseLightColor().y) + ", " + std::to_string(diffuseLightColor().z));
        Logger::debug("[+] Specular: " + std::to_string(specularLightColor().x) + ", " + std::to_string(specularLightColor().y) + ", " + std::to_string(specularLightColor().z));
        Logger::debug("[+] Linear: " + std::to_string(_attLinear));
        Logger::debug("[+] Quadratic: " + std::to_string(_attQuadratic));*/
        shader.setVec3(("pointlights[" + std::to_string(index) + "].position").c_str(), _position);
        shader.setVec3(("pointlights[" + std::to_string(index) + "].ambient").c_str(), ambientLightColor());
        shader.setVec3(("pointlights[" + std::to_string(index) + "].diffuse").c_str(), diffuseLightColor());
        shader.setVec3(("pointlights[" + std::to_string(index) + "].specular").c_str(), specularLightColor());
        shader.setFloat(("pointlights[" + std::to_string(index) + "].constant").c_str(), _attConstant);
        shader.setFloat(("pointlights[" + std::to_string(index) + "].linear").c_str(), _attLinear);
        shader.setFloat(("pointlights[" + std::to_string(index) + "].quadratic").c_str(), _attQuadratic);
    }
    else {
        // Spot light
        /*Logger::debug("Setting spot light " + std::to_string(index) + " to shader");
        Logger::debug("[+] Position: " + std::to_string(_position.x) + ", " + std::to_string(_position.y) + ", " + std::to_string(_position.z));
        Logger::debug("[+] Direction: " + std::to_string(_direction.x) + ", " + std::to_string(_direction.y) + ", " + std::to_string(_direction.z));
        Logger::debug("[+] Ambient: " + std::to_string(ambientLightColor().x) + ", " + std::to_string(ambientLightColor().y) + ", " + std::to_string(ambientLightColor().z));
        Logger::debug("[+] Diffuse: " + std::to_string(diffuseLightColor().x) + ", " + std::to_string(diffuseLightColor().y) + ", " + std::to_string(diffuseLightColor().z));
        Logger::debug("[+] Specular: " + std::to_string(specularLightColor().x) + ", " + std::to_string(specularLightColor().y) + ", " + std::to_string(specularLightColor().z));
        Logger::debug("[+] Linear: " + std::to_string(_attLinear));
        Logger::debug("[+] Quadratic: " + std::to_string(_attQuadratic));
        Logger::debug("[+] CutOff: " + std::to_string(_cutOffAngle));
        Logger::debug("[+] InnerCutOff: " + std::to_string(innerCutOffAngle()));*/
        shader.setVec3(("spotlights[" + std::to_string(index) + "].position").c_str(), _position);
        shader.setVec3(("spotlights[" + std::to_string(index) + "].direction").c_str(), _direction);
        shader.setVec3(("spotlights[" + std::to_string(index) + "].ambient").c_str(), ambientLightColor());
        shader.setVec3(("spotlights[" + std::to_string(index) + "].diffuse").c_str(), diffuseLightColor());
        shader.setVec3(("spotlights[" + std::to_string(index) + "].specular").c_str(), specularLightColor());
        shader.setFloat(("spotlights[" + std::to_string(index) + "].constant").c_str(), _attConstant);
        shader.setFloat(("spotlights[" + std::to_string(index) + "].linear").c_str(), _attLinear);
        shader.setFloat(("spotlights[" + std::to_string(index) + "].quadratic").c_str(), _attQuadratic);
        shader.setFloat(("spotlights[" + QString::number(index) + "].innercutoff").toUtf8().constData(), qCos(qDegreesToRadians(innerCutOffAngle())));
        shader.setFloat(("spotlights[" + std::to_string(index) + "].outercutoff").c_str(), qCos(qDegreesToRadians(_cutOffAngle)));
    }
}