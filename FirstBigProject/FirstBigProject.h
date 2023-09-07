#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FirstBigProject.h"
#include "my_opengl.h"
#include <QKeyEvent>


class FirstBigProject : public QMainWindow
{
    Q_OBJECT
public:
    FirstBigProject(QWidget *parent = nullptr);
    ~FirstBigProject();
    // lcd数值显示逻辑修改
    void handleValueChanged_x(int value);
    void handleValueChanged_y(int value);
    void handleValueChanged_z(int value);
    void handleValueChanged_ambient(int value);
    void handleValueChanged_diffuse(int value);
    void handleValueChanged_specular(int value);

    

private slots:
    void restXYZ_slider(); // 重置xyz
    
signals:
    void angleSent(QVector3D angel);

private:
    Ui::FirstBigProjectClass ui;
    my_opengl *Scene = new my_opengl;
   
    
};
