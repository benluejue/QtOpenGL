#pragma once

#include <QVector3D>
#include <QVector4D>
#include <QVector>
#include <QDebug>
#include <QMatrix4x4>

class Ray
{
	Ray();
	~Ray();
	void addRay(float x, float y);
	
	void transData(const QMatrix4x4& projection, const QMatrix4x4& view, const QVector3D& cameraPos);
	// ������һ��AABB���ӣ����ؾ���������������
	int picking(float x, float y, QVector<QVector<QVector3D>> vec_triangles);
private:
	QVector<QVector3D> vec_ray;
	QMatrix4x4 projection, view;
	QVector3D cameraPos;
};

