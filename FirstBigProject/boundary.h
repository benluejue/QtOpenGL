#pragma once


#include <QVector3D>
#include "ray.h"
#include "utils.h"

class Boundary {
private:
    QVector3D _bottomControlPoint;  // left back bottom
    QVector3D _topControlPoint;     // right front top

public:
    Boundary() 
        : _bottomControlPoint(QVector3D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()) ),
        _topControlPoint(QVector3D(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest())) {}

    Boundary(QVector3D bottomControlPoint, QVector3D topControlPoint)
        : _bottomControlPoint(bottomControlPoint),
        _topControlPoint(topControlPoint) {}

public:
    void updateControlPoints(const QVector3D point);    // update the control points of the boundary box
    

    inline QVector3D bottomControlPoint() const { return _bottomControlPoint; }
    inline QVector3D topControlPoint() const { return _topControlPoint; }
    inline QVector3D bottomCenterPoint() const;
    inline QVector3D centerPoint() const;

    bool hit(const Ray& ray) const;

    void render() const;
};

inline QVector3D Boundary::bottomCenterPoint() const {
    return QVector3D((_bottomControlPoint.x() + _topControlPoint.x()) / 2.0f, _bottomControlPoint.y(), (_bottomControlPoint.z() + _topControlPoint.z()) / 2.0f);
}

inline QVector3D Boundary::centerPoint() const {
    return QVector3D((_bottomControlPoint.x() + _topControlPoint.x()) / 2.0f, (_bottomControlPoint.y() + _topControlPoint.y()) / 2.0f, (_bottomControlPoint.z() + _topControlPoint.z() ) / 2.0f);
}
