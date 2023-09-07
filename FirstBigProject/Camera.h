#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QOpenGLShader>
#include <QDebug>
#include <QKeyEvent>
#include <QWidget>
#include "ray.h"


enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	MOVE_UP,
	MOVE_DOWN

};


class Camera {
public:
	Camera(QVector3D position = QVector3D(1.0f, 0.0f, 0.0f));


public:
	QMatrix4x4 getViewMat(); 

	void ProcessMouseMovement(GLfloat Xoffset, GLfloat Yoffset);
	void DealMouseScroll(GLfloat Yoffset);
	void DealKeyInput(CameraMovement direction);
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void updateCameraVectors();
	Ray generateRay(QVector2D mouseRelativePosition, float aspectRatio) const;

public:
	// 摄像机位置
	QVector3D cameraPos = QVector3D(0.0f, 0.0f, 1.0f);
	// 即相机的目标方向
	QVector3D cameraTarget = QVector3D(0.0, 0.0f, 0.0f);
	// 从摄像机指向要拍的东西的原点
	QVector3D cameraDirection = QVector3D(cameraPos - cameraTarget);
	// 上向量与右向量 右向量(Right Vector)，它代表摄像机空间的x轴的正方向
	QVector3D Up = QVector3D(0.0f, 1.0f, 0.0f);
	QVector3D cameraRight = QVector3D::crossProduct(cameraDirection, Up);
	QVector3D cameraUp = QVector3D::crossProduct(cameraDirection, cameraRight);
	QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
	// right x轴正方向  cameraup z轴正方形 direction/front y正方形 direction指向物体，front一般cameraFront 0 0 -1
	
	GLboolean isFirstMouse = GL_TRUE;
	GLboolean isLeftMousePress = GL_FALSE;
	
	// euler Angles
	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;
	GLfloat MouseSensitivity = 0.1f;
	GLfloat movementSpeed = 0.12f;
	GLfloat zoom = 45.0f;

};
#endif // CAMERA_H