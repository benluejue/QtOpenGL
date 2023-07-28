#include "ray.h"

Ray::~Ray()
{
}

void Ray::addRay(float x, float y)
{
    QVector3D ray_nds = QVector3D(x, y, 1.0f);
    QVector4D ray_clip = QVector4D(ray_nds, 1.0f);
    QVector4D ray_eye = projection.inverted() * ray_clip;
    QVector4D ray_world = view.inverted() * ray_eye;

    if (ray_world.w() != 0.0f) {
        ray_world.setX(ray_world.x() / ray_world.w());
        ray_world.setY(ray_world.y() / ray_world.w());
        ray_world.setZ(ray_world.z() / ray_world.w());
        QVector3D ray_dir = (QVector3D(ray_world.x(), ray_world.y(), ray_world.z()) - cameraPos).normalized();

        this->vec_ray.push_back(cameraPos);
        this->vec_ray.push_back(cameraPos + ray_dir * 100.0f);
    }
}

void Ray::transData(const QMatrix4x4& projection, const QMatrix4x4& view, const QVector3D& cameraPos) {
    this->projection = projection;
    this->view = view;
    this->cameraPos = cameraPos;
}

int Ray::picking(float x, float y, QVector<QVector<QVector3D>> vec_triangles)
{

    return -1;
}



