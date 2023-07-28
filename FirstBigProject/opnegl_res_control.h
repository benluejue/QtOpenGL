#pragma once


#include <QOpenGLFunctions_3_3_Core>
#include <QVector4D>
#include <QVector3D>
#include <QVector>
#include <QMatrix4x4>

class ModelSelect {
public:
    ModelSelect() : isSelect(false) {

    }
    ModelSelect(QMatrix4x4 model) : isSelect(false) {
        this->model = model;
    }
    ~ModelSelect() {

    }
    QMatrix4x4 model;
    bool isSelect;
};
