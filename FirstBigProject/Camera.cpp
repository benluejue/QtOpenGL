#include "camera.h"

Camera::Camera(QVector3D position)
{
	this->cameraPos = position;

	updateCameraVectors();
}

QMatrix4x4 Camera::getViewMat()
{
	// 观察矩阵 
	QMatrix4x4 view;
	//view.lookAt(cameraPos, cameraTarget, cameraUp);
	// glm::LookAt函数需要一个位置、目标和上向量。它会创建一个和在上一节使用的一样的观察矩阵。
	// view.lookAt(this->cameraPos, this->cameraDirection, this->cameraUp);
	// this->cameraDirection 从摄像机指向要拍的物品
	// cameraDirection =  cameraPos - cameraTarget
	view.lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
	return view;
}

void Camera::DealKeyInput(CameraMovement direction)
{
	GLfloat velocity = this->movementSpeed;
	if (direction == FORWARD)
		this->cameraPos += this->cameraDirection * velocity;
	if (direction == BACKWARD)
		this->cameraPos -= this->cameraDirection * velocity;
	if (direction == LEFT)
		this->cameraPos -= this->cameraRight * velocity;
	if (direction == RIGHT)
		this->cameraPos += this->cameraRight * velocity;
}

void Camera::ProcessMouseMovement(GLfloat Xoffset, GLfloat Yoffset)
{
	this->MouseSensitivity = 0.15;
	if (Xoffset >= 50 || Xoffset <= -50) return;
	if (Yoffset >= 50 || Yoffset <= -50) return;
	Xoffset *= this->MouseSensitivity;
	Yoffset *= this->MouseSensitivity;
	
	this->yaw += Xoffset;
	this->pitch += Yoffset;
	
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	updateCameraVectors();
}

void Camera::DealMouseScroll(GLfloat Yoffset)
{
	if (this->zoom >= 1.0f && this->zoom <= 90.0f)
		this->zoom -= Yoffset;
	qDebug() << this->zoom;
	if (this->zoom > 90.0f)
		this->zoom = 90.0f;
	if (this->zoom < 1.0f)
		this->zoom = 1.0f;
}


void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	GLfloat velocity = this->movementSpeed* deltaTime;
	// camera->cameraPos += QVector3D(1.0f, 0.0f, 0.0f) * vel;
	if (direction == FORWARD) {
		this->cameraPos += cameraFront * velocity;
	}
	else if (direction == BACKWARD)
		this->cameraPos -= cameraFront * velocity;
	else if (direction == LEFT)
		this->cameraPos -= cameraRight * velocity;
	else if (direction == RIGHT)
		this->cameraPos += cameraRight * velocity;
	else if (direction == MOVE_UP) {
		this->cameraPos += QVector3D(0.0f, 1.0f, 0.0f) * velocity;
	}
	else if (direction == MOVE_DOWN)
		this->cameraPos -= QVector3D(0.0f, 1.0f, 0.0f) * velocity;
}

void Camera::updateCameraVectors()
{
	QVector3D front;
	//  calculate the new Front vector
	front.setX(qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
	front.setY(qSin(qDegreesToRadians(pitch)));
	front.setZ(qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
	this->cameraFront = front.normalized();


	this->cameraRight = QVector3D::crossProduct(cameraFront, Up);
	this->cameraRight = cameraRight.normalized();
	this->cameraUp = QVector3D::crossProduct(cameraRight, cameraFront);
	this->cameraUp = cameraUp.normalized();

}






