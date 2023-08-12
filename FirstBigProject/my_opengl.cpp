#include "my_opengl.h"
#include "basic_shapes.h"
#include "ResourceManager.h"





my_opengl::my_opengl(QWidget* parent)
	: QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    setFormat(format);

    projection.setToIdentity();
    m_model.setToIdentity();

    
}

my_opengl::~my_opengl()
{
    //delete planeShader;
    delete boundaryShader;
    
}

GraphicsRenderer* my_opengl::hitTest(const Ray& ray) {
    CollisionRecorder newRecord = CollisionRecorder();
    GraphicsRenderer* newObject = nullptr;
    // Object hit test
    for (int i = 0; i < _shapes.size(); i++) {
        qDebug() << "-----------------------------------------";
        Logger::debug("Testing object " + std::to_string(i));
        GraphicsRenderer* obj = _shapes[i];
        // 后续补上_operatingObject为在操作的物体
        //if (obj == _operatingObject) {
        //    // Ignore current operating Object
        //    continue;
        //}
        CollisionRecorder hitRecord = obj->hit(ray);
        if (hitRecord.hitted()) {
            Logger::debug("Hitted object " + std::to_string(i));
        }
        else {
            Logger::debug("Missed object " + std::to_string(i));
        }
        if (hitRecord.hitted() && hitRecord.t() < newRecord.t()) {
            newRecord = hitRecord;
            newObject = obj;
        }
    }
    // Terrain hit test
   /* if (_terrain != nullptr) {
        HitRecord hitRecord = _terrain->hit(ray);
        if (hitRecord.hitted()) {
            Logger::debug("Hitted terrain");
        }
        else {
            Logger::debug("Missed terrain");
        }
        if (hitRecord.hitted() && hitRecord.t() < newRecord.t()) {
            newRecord = hitRecord;
            newObject = nullptr;
        }
    }*/
    _hitRecord = newRecord;
    return newObject;
}


void my_opengl::initializeGL()
{
	initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    qDebug() << "initializeGL";


    for (GLuint i = 0; i != 1024; ++i) //初始化键盘按键
        keys[i] = GL_FALSE;

    deltaTime = 0.0f;
    lastFrame = 0.0f;
    time.start();
   
    boundaryShader = new Shader("./boundary_shader.vert", "./boundary_shader.frag");


    camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f));
  

    plane_model = Model("D:/OpenGL/OpenGL/resources/plane/plane.obj");
    plane_shader = new Shader("./plane.vert", "./plane.frag");


    /**** 渲染一个摄像机正方体******/
    camera_cube_model = Model("D:/OpenGL/OpenGL/resources/camera_cube/camera_cube.obj");
    camera_cube_shader = new Shader(":/shader/shader/camera_cube.vert", ":/shader/shader/camera_cube.frag");


    /**天空盒*/
    skybox = new SkyBox;
    /***载入shader**/
    ResourceManager::loadShader("skybox", ":/shader/shader/skybox.vert", ":/shader/shader/skybox.frag");

    /******尝试增加模型**********/ 
    // D:/OpenGL/OpenGL/resources/99-intergalactic_spaceship-obj/Intergalactic_Spaceship-(Wavefront).obj
    //  "D:/OpenGL/VSOpenGL/FirstBigProject/FirstBigProject/3Dmodel/nanosuit/nanosuit.obj"
    // D:/OpenGL/OpenGL/resources/LightBulb1/LightBulbOBJ.obj
    // D:/OpenGL/OpenGL/resources/planet/planet_Quom1200.png
    // D:/OpenGL/OpenGL/resources/rock/rock.obj
    // D:/OpenGL/OpenGL/resources/cube/cube.obj
    // D:/OpenGL/OpenGL/resources/11-obj/.OBJ/Room #1.obj
    std::string path = "D:/OpenGL/OpenGL/resources/cube/base_cube.obj";
    ourModel = Model(path);
    /*载入shader*/
    QString vert_str = ":/shader/shader/our_model.vert";
    QString frag_str = ":/shader/shader/our_model.frag";
    ourModelShader = new Shader(vert_str, frag_str);


    cube_model = Model("D:/OpenGL/VSOpenGL/FirstBigProject/FirstBigProject/3Dmodel/nanosuit/nanosuit.obj");
    cube_shader = new Shader("./shader/colors.vert", "./shader/colors.frag");

    // 添加新物体
    _objects = new GraphicsRenderer(&cube_model);


}

void my_opengl::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

// (1)初始时候 (2)update时候(3) 窗口大小改变时候
void my_opengl::paintGL()
{
    // qDebug() << camera->cameraPos;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
   
    GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    this->processInput(deltaTime);
    this->update();

   
     // 相机调用
    this->view = camera->getViewMat();
    this->projection.setToIdentity();
    this->projection.perspective(camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 100.f);
   
   

    cube_shader->use();
    cube_shader->setMat4("view", view);
    cube_shader->setMat4("projection", projection);
    cube_shader->setVec3("light.position", camera->cameraPos);
    cube_shader->setVec3("light.direction", camera->cameraFront);

    /*qDebug() <<"cameraPos" << camera->cameraPos;*/
    auto cutoff = qCos(qDegreesToRadians(12.5f));
    cube_shader->setFloat("light.cutOff", cutoff);
    cutoff = qCos(qDegreesToRadians(17.5f));
    cube_shader->setFloat("light.outerCutOff", cutoff);

    cube_shader->setVec3("viewPos", camera->cameraPos);
      // light properties "ambient"（环境光）环境光的强弱 
    cube_shader->setVec3("light.ambient", m_ambient);
    cube_shader->setVec3("light.diffuse", m_diffuse);
    cube_shader->setVec3("light.specular", m_specular);
    cube_shader->setFloat("material.shininess", 32.0f);
    cube_shader->setFloat("light.constant", m_constant);
    cube_shader->setFloat("light.linear", m_linear);
    cube_shader->setFloat("light.quadratic", m_quadratic);


    QMatrix4x4 cu_model;
    cu_model.setToIdentity();
    cu_model.scale(0.1, 0.1, 0.1);
    cu_model.translate(0.0, -8.0f, 0.0);
    cube_shader->setMat4("model", cu_model);
    //cube_model.render(*cube_shader);
    _objects->render(*cube_shader);
    
   




    // 绘制 包装盒
    boundaryShader->use();
    boundaryShader->setMat4("view", this->view);
    boundaryShader->setMat4("projection", this->projection);
    boundaryShader->setMat4("model", m_model);
    boundary.render();
    update();

    
    /*****绘制plane*****/
    plane_shader->use();
    plane_shader->setMat4("view", view);
    plane_shader->setMat4("projection", projection);
    QMatrix4x4 pl_model;
    pl_model.translate(-1.0f, 1.0f, -1.0f);
    pl_model.scale(0.2, 0.2, 0.2);
    plane_shader->setMat4("model", pl_model);
    plane_model.render(*plane_shader);


 
    camera_cube_shader->use();
    camera_cube_shader->setMat4("view", view);
    camera_cube_shader->setMat4("projection", projection);
    QMatrix4x4 cam_model;
    cam_model.translate(-1.0f, 1.0f, -1.0f);
    cam_model.scale(0.2, 0.2, 0.2);
    camera_cube_shader->setMat4("model", cam_model);
    camera_cube_model.render(*camera_cube_shader);



    /*****绘制天空盒 ********/
    // 深度换冲默认的值为1.0， 如果不加上小于等于的比较条件，那深度值为1.0的天空盒在小于深度值的条件下永远无法通过深度测试
   // OPENGL_EXTRA_FUNCTIONS->glDepthFunc(GL_LEQUAL);
    // 将最后的w分量设置为0
    QMatrix4x4 skyboxView;
    skyboxView.setRow(0, QVector4D(view(0, 0), view(0, 1), view(0, 2), 0.0f));
    skyboxView.setRow(1, QVector4D(view(1, 0), view(1, 1), view(1, 2), 0.0f));
    skyboxView.setRow(2, QVector4D(view(2, 0), view(2, 1), view(2, 2), 0.0f));
    skyboxView.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f)); //这个去掉位移的4x4矩阵，使天空盒vertices的尺寸的改变，不再影响渲染效果
    ResourceManager::getShader("skybox").use().setMatrix4f("view", skyboxView);
    ResourceManager::getShader("skybox").use().setMatrix4f("projection", this->projection);
    skybox->render();
    OPENGL_EXTRA_FUNCTIONS->glDepthFunc(GL_LESS);

    /******尝试增加3D模型**********/
   // ourModelShader->use();
   // ourModelShader->setMat4("view", view);
   // ourModelShader->setMat4("projection", projection);
   // QMatrix4x4 big_model;
   // big_model.setToIdentity();
   ///* big_model.scale(QVector3D(0.1f, 0.1f, 0.1f));*/
   // big_model.translate(QVector3D(0.0f, 1.25f, 0.0f));
   // ourModelShader->setMat4("model", big_model);
   // ourModel.render(*ourModelShader);

}




// 新的鼠标控制方式 触发
void my_opengl::mouseMoveEvent(QMouseEvent* event)
{
  
    if (event->buttons() & Qt::RightButton) 
    {
        // 移动视角
        /*setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);*/
        //setCursor(Qt::CrossCursor);
        if (m_first_mouse_right == true)
        {
            cam_lastPos = event->position().toPoint();
            m_first_mouse_right = false;
        }
        int dx = cam_lastPos.x() - event->position().toPoint().x();
        int dy = event->position().toPoint().y() - cam_lastPos.y();
        cam_lastPos = event->position().toPoint();
   
        camera->ProcessMouseMovement(dx, dy);
        update();
    }
    else if (event->buttons() & Qt::LeftButton) {
        // 移动 选中 的物品
        if (m_first_mouse_left == true)
        {
            move_lastPos = event->position().toPoint();
            m_first_mouse_left = false;
        }
        int dx_move = event->position().toPoint().x() - move_lastPos.x();
        int dy_move = move_lastPos.y() - event->position().toPoint().y();

        float precision = 0.00005;
 
        
        this->m_model.translate(dx_move * 1.0 * precision, dy_move * 1.0 * precision, 0.0f);
        update();


    }
    //setCursor(Qt::ArrowCursor);
}
// 触发
void my_opengl::wheelEvent(QWheelEvent* event)
{
    //向前滚动一下是offset=0,120
    QPoint offset = event->angleDelta();
    
    camera->DealMouseScroll(offset.y() / 30.0f);
}
// 鼠标摁下事件
void my_opengl::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "mousePressEvent";

}
// 触发
void my_opengl::mouseReleaseEvent(QMouseEvent* event)
{
}

// 连续键盘事件
void my_opengl::processInput(GLfloat dt)
{
    if (keys[Qt::Key_W])
    {
        camera->ProcessKeyboard(CameraMovement::FORWARD, dt);
    }
        
    if (keys[Qt::Key_S])
        camera->ProcessKeyboard(CameraMovement::BACKWARD, dt);
    if (keys[Qt::Key_A])
        camera->ProcessKeyboard(CameraMovement::LEFT, dt);
    if (keys[Qt::Key_D])
        camera->ProcessKeyboard(CameraMovement::RIGHT, dt);
    if (keys[Qt::Key_E])
        camera->ProcessKeyboard(CameraMovement::MOVE_DOWN, dt);
    if (keys[Qt::Key_Q]) {
        camera->ProcessKeyboard(CameraMovement::MOVE_UP, dt);
    }
}

void my_opengl::keyPressEvent(QKeyEvent* event)
{
    GLuint key = event->key();
    if (key >= 0 && key < 1024)
    {
        this->keys[key] = GL_TRUE;
    }
        
}

void my_opengl::keyReleaseEvent(QKeyEvent* event)
{
    GLuint key = event->key();
    if (key >= 0 && key < 1024) 
    {
        this->keys[key] = GL_FALSE;
    }
        
}


static void qNormalizeAngle(int& angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}
void my_opengl::setXRotation(int angle)
{
    
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        // 这个转的快的原因是 原先m_world.setToIdentity();使其都为1，而现在
        // 相当于每次 新转这个180.0f - (m_xRot/16.0f)角度
        // 因此使用m_last_xRot 记录上下两次Rot的差值记录变化情况
        m_model.rotate((180.0f - ((m_last_xRot-m_xRot)/16.0f)), 1, 0, 0);
        m_last_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void my_opengl::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        m_model.rotate( ( m_last_yRot-m_yRot) / 16.0f, 0, 1, 0);
        m_last_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void my_opengl::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;

        m_model.rotate((m_last_zRot -m_zRot) / 16.0f, 0, 0, 1);
        m_last_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}


void my_opengl::Campos_left()
{
    
    // cameraPos -= QVector3D(0.0f, 1.0f, 0.0f) * velocity;
    camera->cameraPos += QVector3D(1.0f, 0.0f, 0.0f) * vel;
    update();
}
void my_opengl::Campos_right() 
{
   
    // cameraPos -= QVector3D(0.0f, 1.0f, 0.0f) * velocity;
    camera->cameraPos -= QVector3D(1.0f, 0.0f, 0.0f) * vel;
    update();
}

void my_opengl::Campos_down()
{
  
    camera->cameraPos += QVector3D(0.0f, 1.0f, 0.0f) * vel;
    update();
}

void my_opengl::Campos_up()
{
 
    camera->cameraPos -= QVector3D(0.0f, 1.0f, 0.0f) * vel;
    update();
}

void my_opengl::setAmbient(int value)
{
    float f_valuse = value/10;
    m_ambient = QVector3D(0.1f, 0.1f, 0.1f)* f_valuse;

    update();
}

void my_opengl::set_diffuse(int diffuse)
{
    float f_valuse = diffuse / 10;
    m_diffuse = QVector3D(0.1f, 0.1f, 0.1f) * f_valuse;
   
    update();
}

void my_opengl::set_specular(int specular)
{
    float f_valuse = specular / 10;
    m_specular = QVector3D(0.1f, 0.1f, 0.1f) * f_valuse;
  
    update();
}

void my_opengl::add_object_cube()
{

    qDebug() << "hi";
}

