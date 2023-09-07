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
#include "pubilc.h"
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
#include <QMessageBox>
#include <QApplication>




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
	void setBlinn(bool isTrue);
	void choseLight(LIGHTTYPE);


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
	void add_object_plane();
	void add_object_bigtoy();

	void rest_last_xyz();
	


	

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

	void rest_last_xyz_signal();

public:
	// 所有物体都加载在这里
	std::vector<GraphicsRenderer*>_shapes;

	SkyBox* skybox;
	Shader* ourModelShader;
	Shader* camera_cube_shader;
	Shader* easy_shader;
	Shader* full_shader;
	Shader* boundary_shader;

	Model ourModel;
	Model *camera_cube_model;
	Model plane_model;
	Model cube_model;

	GraphicsRenderer* _cmaera_objects;
	CollisionRecorder _hitRecord;
	void set_light_roate(QVector3D angle);
private:
	void moveOperatingObject(const Ray& ray);

	
	Camera* camera;
	QMatrix4x4 m_model;
	QMatrix4x4 m_cam_pos;
	int m_xRot = 0, m_yRot = 0, m_zRot = 0;
	int m_last_xRot = 3000, m_last_yRot = 3000, m_last_zRot = 3000;
	// 右键移动镜头的坐标变量
	QPoint cam_lastPos;
	// 左键 移动物体的坐标变量
	QPoint move_lastPos;

	// 左右移动速度
	float vel = 0.1;
	// 材质
	QOpenGLTexture* metalMap;
	// 光照
	QVector3D m_ambient = QVector3D(0.8f, 0.8f, 0.8f);
	QVector3D m_diffuse = QVector3D(0.5f, 0.5f, 0.5f);
	QVector3D m_specular{ 1.0f, 1.0f, 1.0f };
	// 半程向量光
	bool blinn = false;


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
	int plane_cnt = 0; // plane总数

	QVector3D light_angle{0.0f,0.0f,0.0f};

	// 几个状态之间的转换
	GraphicsRenderer* _hoveredObject = nullptr;
	GraphicsRenderer* _pressedObject = nullptr;
	GraphicsRenderer* _selectedObject = nullptr;
	GraphicsRenderer* _operatingObject = nullptr;
	CollisionRecorder _hit_record;

	bool _controlPressed = false;
	bool _dragged = false;
	bool _hideBound = false;
	// 被选中物体的下标
	int idx_model = 0;
	// 光的类型
	LIGHTTYPE light_type;
	// 100 = 4 --  010 = 2 --- 001 = 1
	int light_local = 0x04;
};


#endif
