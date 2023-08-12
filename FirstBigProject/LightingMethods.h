#pragma once

#include <string>
#include <queue>
#include <QVector>

#include "Shader.h"

class LightingMethods {
protected:
    QVector3D _lightColor;

public:
    LightingMethods(QVector3D color);
    ~LightingMethods();

protected:
    virtual QVector3D ambientLightColor() const = 0;
    virtual QVector3D diffuseLightColor() const = 0;
    virtual QVector3D specularLightColor() const = 0;

    virtual void updateShader(Shader shader, int index) const = 0;

public:
    QVector3D lightColor() const { return _lightColor; }
    void setLightColor(QVector3D lightColor) { _lightColor = lightColor; }
};

// Direction light
class DirLight : public LightingMethods {
protected:
    QVector3D _direction;   // The outgoing direction of the light source
    float _intensity = 50;

public:
    DirLight(QVector3D direction = QVector3D(0.0f, -1.0f, 0.0f), QVector3D color = QVector3D(1.0f, 1.0f, 1.0f));
    ~DirLight();

protected:
    virtual QVector3D ambientLightColor() const override { return QVector3D(_intensity / 200.0f, _intensity / 200.0f, _intensity / 200.0f) * _lightColor; }
    virtual QVector3D diffuseLightColor() const override { return QVector3D(_intensity / 200.0f, _intensity / 200.0f, _intensity / 200.0f) * _lightColor; }
    virtual QVector3D specularLightColor() const override { return QVector3D(1.0f, 1.0f, 1.0f) * _lightColor; }

public:
    QVector3D lightDirection() const { return _direction; }  // The same direction as the outgoing direction
    void setLightDirection(QVector3D direction) { _direction = direction; }
    void setIntensity(float intensity) { _intensity = intensity; }

    // Render util function
    virtual void updateShader(Shader shader, int index) const override;
};

// Scoped Light is a combination of point light and spot light
// Shader setting is automatically configured based on the phi value
// If phi < 162.5, then the light is considered as spot light
// Otherwise it's considered as point light
class ScopedLight : public LightingMethods {
protected:
    // Light source status
    QVector3D _position;
    QVector3D _direction;
    float _cutOffAngle = 180.0f;

    // Light property
    int _idealDistance = 32;     // ideally calculated distance 
    const float _attConstant = 1.0f;   // attenuation constant
    float _attLinear = 0.14f;   // attenuation linear term
    float _attQuadratic = 0.07f;    // attenuation quad term

public:
    ScopedLight(QVector3D position, QVector3D direction = QVector3D(0.0f, -1.0f, 0.0f), QVector3D color = QVector3D(1.0f, 1.0f, 1.0f));
    ScopedLight(int distance, QVector3D position, QVector3D direction = QVector3D(0.0f, -1.0f, 0.0f), QVector3D color = QVector3D(1.0f, 1.0f, 1.0f));
    ScopedLight(QVector3D position, QVector3D direction, float cutOffAngle, int idealDistance, float linear, float quadratic, QVector3D color);
    ~ScopedLight();

private:
    // Util function
    inline void updateLinear();
    inline void updateQuadratic();
    inline float innerCutOffAngle() const;

protected:
    virtual QVector3D ambientLightColor() const override { return QVector3D(0.2f,0.2f,0.2f) * _lightColor; }
    virtual QVector3D diffuseLightColor() const override { return QVector3D(0.7f,0.7f,0.7f) * _lightColor; }
    virtual QVector3D specularLightColor() const override { return QVector3D(1.0f,1.0f,1.0f) * _lightColor; }

public:
    // Property setters and getters
    int idealDistance() const { return _idealDistance; }
    void setIdealDistance(int distance);
    QVector3D lightDirection() const { return _direction; }
    void setLightDirection(QVector3D direction) { _direction = direction; }
    float cutOffAngle() const { return _cutOffAngle; }
    void setCutOffAngle(float angle);

    bool isPointLight() const { return abs(_cutOffAngle - 180.0f) < 1e-6; }

    ScopedLight toWorldSpace(QMatrix4x4 modelMatrix) const;

    // Render util function
    virtual void updateShader(Shader shader, int index) const override;
};

