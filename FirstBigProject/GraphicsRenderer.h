#pragma once



#include "model.h"
#include "LightingMethods.h"
#include "Shader.h"
#include "boundary.h"
#include <QQuaternion>
#include <QtMath>
#include "CollisionRecorder.h"
#include "ray.h"
#include "pubilc.h"

enum SHAPE_COLLIDE_STATE
{
    WAS_HIT = 1,
    MISS_HIT
};

class GraphicsRenderer {
public:
    static GraphicsRenderer empty() {
        return GraphicsRenderer();
    }
    SHAPETYPE _shapeType;
    SHAPE_COLLIDE_STATE _shape_collide_state;
private:
    GraphicsRenderer()
        :_shape_collide_state(MISS_HIT)
    {}

private:
    Model* _model = nullptr;
    ScopedLight* _light = nullptr;
    /*glm::vec3 _position = glm::vec3(0.0f);
    glm::mat4 _rotation = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(1.0f);*/
    QVector3D _position{ 0.0f, 0.0f, 0.0f };
    QMatrix4x4 _rotation;
    QVector3D _scale{ 1.0f, 1.0f, 1.0f };
    Boundary _boundary; // the renderable's boudary box, should be updated after a transformation is done
    

public:
    GraphicsRenderer(Model* model);
    GraphicsRenderer(Model* model, QVector3D position);
    ~GraphicsRenderer();

public:
    QVector3D position() const { return _position; }
    QMatrix4x4 rotation() const { return _rotation; }
    QVector3D scaleVal() const { return _scale; }
    Model* model() const { return _model; }

    void setModel(Model* model);
    void move(QVector3D deltaVec);
    void setPosition(QVector3D position);
    void rotate(QVector3D axis, float deltaAngle);
    void setRotation(QVector3D axis, float angle);
    void scale(float deltaScale);
    void setScale(float scale);

    ScopedLight transformedLight() const; // pass out the light object to scene manager to gather all light sources
    ScopedLight* originalLight() const; // pass out the light object to scene manager to set light attributes
    bool hasLight() const { return _light != nullptr; }
    void makeLight();   // create a light source in the object
    void disposeLight();    // remove the light source in the object

    const Boundary& boundary() const { return _boundary; }

    inline QMatrix4x4 modelMatrix() const;

public:
    void updateBoundary();  // update the boundary of the renderable after a transformation is done
    CollisionRecorder hit(const Ray& ray) const;    // test the hit record of an input array
    void render(Shader& shader);
};

/**̫�Ѹģ����ܻ�������**/
inline QMatrix4x4 GraphicsRenderer::modelMatrix() const {
    // Calculate rotate direction and angle from rotation matrix _rotation 
    QVector3D axis;
    float angle;
    // ��Ԫ��quat
    // QQuaternionw��x��y �� z,w��ʵ����xyz���鲿����Ԫ�����Ա�ʾ��ת����ת�Ƕ�
    
    //glm::quat quat(_rotation);
    /*glm::vec3 axisAngle = glm::axis(quat); 
    axis = glm::normalize(axisAngle);   // ����ƶ�����
    angle = glm::angle(quat);*/
    // ����������
    QQuaternion quat;
    
    /*QMatrix3x3 rotationMatrix = _rotation.toGenericMatrix<3, 3>();
    quat = QQuaternion::fromRotationMatrix(rotationMatrix);*/
    // quat.vector()����w��������ʾ��ת����
    QVector3D rotationAxis;
    float rotationAngle;
    quat.getAxisAndAngle(&rotationAxis, &rotationAngle);
    angle =  qRadiansToDegrees(rotationAngle);

    // Calculate model matrix
    QMatrix4x4 model;
    model.setToIdentity();    
    model.translate(_position);
    model.rotate(angle, axis);
    model.scale(_scale);

    return model;
}