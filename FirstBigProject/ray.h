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
	// 穿进来一堆AABB盒子，返回距离最近的物体盒子
	int picking(float x, float y, QVector<QVector<QVector3D>> vec_triangles);
private:
	QVector<QVector3D> vec_ray;
	QMatrix4x4 projection, view;
	QVector3D cameraPos;
};

