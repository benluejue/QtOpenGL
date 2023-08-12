#ifndef MCR_OPENGL_H
#define MCR_OPENGL_H

#include <QObject>
#include <qopengl.h>
#include <QOpenGLWidget>//QT
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QDebug>
#include <QTime>
#include <qdebug.h>
#include <QVector3D>
#include <QtMath>
#include <QOpenGLTexture>
#include <QDateTime>
#include <QElapsedTimer>
#include "Shader.h"
#include "pubilc.hpp"
#include "Camera.h"
#include "utils.h"
#include "ray.h"
#include "boundary.h"
#include "ResourceManager.h"
#include "basic_shapes.h"
#include "skybox.h"
#include "model.h"
#include "shader2.h"
#include "GraphicsRenderer.h"
#include "CollisionRecorder.h"



class my_opengl : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit my_opengl(QWidget* parent = nullptr);
	~my_opengl();

	// 重载的都在protect里
protected:
	virtual void initializeGL(); // 负责初始化，也就是缓冲区，vao，vbo，着色器
	virtual void resizeGL(int w, int h); // 视窗 窗口大小，
	virtual void paintGL(); // 画 渲染一次，循环要添加到update函数 初始时候会画一下，调用update时候也会画一下

public:

	void mouseMoveEvent(QMouseEvent* event);//鼠标事件
	void wheelEvent(QWheelEvent* event);    //滚轮事件
	void mousePressEvent(QMouseEvent* event);  //鼠标按下事件
	void mouseReleaseEvent(QMouseEvent* event); //鼠标释放事件
	//void keyPressEvent(QKeyEvent* event);		// 键盘控制
	void processInput(GLfloat dt);				// 键盘控制 好处是这种方式可以实现连续控制
	void keyPressEvent(QKeyEvent* event);   //键盘按下事件
	void keyReleaseEvent(QKeyEvent* event);  //键盘释放事件
	

public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void Campos_left();
	void Campos_right();
	void Campos_down();
	void Campos_up();

	void setAmbient(int Ambient);
	void set_diffuse(int diffuse);
	void set_specular(int specular);

	void add_object_cube();


signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

public:
	std::vector<GraphicsRenderer*>_shapes;
	

	SkyBox* skybox;
	Shader* ourModelShader;
	Shader* camera_cube_shader;
	Shader* plane_shader;
	Shader* cube_shader;

	Model ourModel;
	Model camera_cube_model;
	Model plane_model;
	Model cube_model;

	GraphicsRenderer* _objects;
	CollisionRecorder _hitRecord;

private:
	GLuint VAO, VBO;
	QOpenGLShaderProgram program;



	Shader* LightShader;
	Shader* boundaryShader;


	Camera* camera;
	QMatrix4x4 m_model;
	QMatrix4x4 m_cam_pos;
	int m_xRot = 0, m_yRot = 0, m_zRot = 0;
	int m_last_xRot = 0, m_last_yRot = 0, m_last_zRot = 0;
	// 右键移动镜头的坐标变量
	QPoint cam_lastPos;
	// 左键 移动物体的坐标变量
	QPoint move_lastPos;

	// 左右移动速度
	float vel = 0.1;
	// 材质
	QOpenGLTexture* diffuseMap;//漫反射贴图
	QOpenGLTexture* specularMap;//高光贴图
	QOpenGLTexture* metalMap;
	// 光照
	QVector3D m_ambient = QVector3D(0.2f, 0.2f, 0.2f);
	QVector3D m_diffuse = QVector3D(0.5f, 0.5f, 0.5f);
	QVector3D m_specular{ 1.0f, 1.0f, 1.0f };


	// 时间差
	QElapsedTimer  time;
	GLfloat deltaTime;
	GLfloat lastFrame;//上一帧
	// 键盘事件实现多按键触控
	GLboolean keys[1024];//获取键盘按键，实现多键触控
	// 模型是否被选中
	bool isSelected = false;
	// 单个模型碰撞盒, 在我们只有一个物体(cube)时候，给出固定的点,最后用
	QVector3D bottomControlPoint = QVector3D(0.7f, 0.7f, 0.7f);
	QVector3D topControlPoint = QVector3D(-0.7f, -0.7f, -0.7f);
	// 列表初始化
	Boundary boundary{ bottomControlPoint, topControlPoint };
	// 摄影机使用的几个变量
	QMatrix4x4 view;
	QMatrix4x4 projection;

	GraphicsRenderer* hitTest(const Ray& ray);

	bool m_first_mouse_right = true;
	bool m_first_mouse_left = true;
	float m_constant = 1.0;
	float m_linear = 0.09f;
	float m_quadratic = 0.032f;
	
};

#endif
