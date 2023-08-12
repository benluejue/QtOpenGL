#include "FirstBigProject.h"
#include <qopengl.h>

FirstBigProject::FirstBigProject(QWidget *parent)
    : QMainWindow(parent)
{
    
    ui.setupUi(this);

    // setCentralWidget(ui.openGLWidget);
    ui.lcdNumber->setSegmentStyle(QLCDNumber::Flat); 
    ui.lcdNumber->setStyleSheet("background:transparent;color:090808;"); //背景透明 设置字体色
    ui.lcdNumber_2->setSegmentStyle(QLCDNumber::Flat); 
    ui.lcdNumber_2->setStyleSheet("background:transparent;color:090808;"); //背景透明 设置字体色
    ui.lcdNumber_3->setSegmentStyle(QLCDNumber::Flat); 
    ui.lcdNumber_3->setStyleSheet("background:transparent;color:090808;"); //背景透明 设置字体色
    ui.lcd_ambient->setSegmentStyle(QLCDNumber::Flat); 
    ui.lcd_ambient->setStyleSheet("background:transparent;color:090808;"); //背景透明 设置字体色
    
    // x轴
    // 滑动时候 lcd变化
    connect(ui.X_Slider, &QSlider::valueChanged, this, &FirstBigProject::handleValueChanged_x);
    // 滑动轴变化 角度变化
    connect(ui.X_Slider, &QSlider::valueChanged, Scene, &my_opengl::setXRotation);
    // 角度变化 lcd显示变化
    connect(Scene, &my_opengl::xRotationChanged, ui.X_Slider, &QSlider::setValue);
    // y轴
    connect(ui.Y_Slider, &QSlider::valueChanged, this, &FirstBigProject::handleValueChanged_y);
    connect(ui.Y_Slider, &QSlider::valueChanged, Scene, &my_opengl::setYRotation);
    connect(Scene, &my_opengl::yRotationChanged, ui.Y_Slider, &QSlider::setValue);

    connect(ui.Z_Slider, &QSlider::valueChanged, this, &FirstBigProject::handleValueChanged_z);
    connect(ui.Z_Slider, &QSlider::valueChanged, Scene, &my_opengl::setZRotation);
    connect(Scene, &my_opengl::zRotationChanged, ui.Z_Slider, &QSlider::setValue);
    // 摄像头 左右移动
    connect(ui.left_p_btn, &QPushButton::clicked, Scene, &my_opengl::Campos_left);
    connect(ui.right_p_btn, &QPushButton::clicked, Scene, &my_opengl::Campos_right);
    connect(ui.down_p_btn, &QPushButton::clicked, Scene, &my_opengl::Campos_down);
    connect(ui.up_p_btn, &QPushButton::clicked, Scene, &my_opengl::Campos_up);

    // 调节光照强度
    connect(ui.ambient_slider, &QSlider::valueChanged, this, &FirstBigProject::handleValueChanged_ambient);
    connect(ui.ambient_slider, &QSlider::valueChanged, Scene, &my_opengl::setAmbient);
    // 调节漫反射
    connect(ui.diffuse_slider, &QSlider::valueChanged, this, &FirstBigProject::handleValueChanged_diffuse);
    connect(ui.diffuse_slider, &QSlider::valueChanged, Scene, &my_opengl::set_diffuse);
    // 调节镜面反射
    connect(ui.specular_slider, &QSlider::valueChanged, this, &FirstBigProject::handleValueChanged_specular);
    connect(ui.specular_slider, &QSlider::valueChanged, Scene, &my_opengl::setAmbient);
    ui.specular_slider->setValue(100);
    ui.diffuse_slider->setValue(50);
    ui.ambient_slider->setValue(20);
    

    // 添加新的渲染对象
    connect(ui.actionCbue, &QAction::triggered, Scene, &my_opengl::add_object_cube);

    setWindowIcon(QIcon("D:/OpenGL/VSOpenGL/FirstBigProject/FirstBigProject/img/CloverIcon.png"));
    ui.OpenGLverticalLayout->addWidget(Scene);
    Scene->setFocusPolicy(Qt::StrongFocus);

    

}

FirstBigProject::~FirstBigProject()
{
    delete Scene;
    Scene = nullptr; // 防止悬空指针，如果我们不小心使用了无效的指针，指向已经释放或无效的内存地址的指针
}

void FirstBigProject::handleValueChanged_x(int value)
{
    value /= 16;
    ui.lcdNumber->display(value);
}

void FirstBigProject::handleValueChanged_y(int value)
{
    value /= 16;
    ui.lcdNumber_2->display(value);
    
}

void FirstBigProject::handleValueChanged_z(int value)
{
    value /= 16;
    ui.lcdNumber_3->display(value);
}

void FirstBigProject::handleValueChanged_ambient(int value)
{
    ui.lcd_ambient->display(value);
}

void FirstBigProject::handleValueChanged_diffuse(int value)
{
    ui.lcd_diffuse->display(value);
}

void FirstBigProject::handleValueChanged_specular(int value)
{
    ui.lcd_specular->display(value);
}





