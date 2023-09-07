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
  
}

GraphicsRenderer* my_opengl::hitTest(const Ray& ray) {
    CollisionRecorder newRecord = CollisionRecorder();
    GraphicsRenderer* newObject = nullptr;
    // Object hit test
    for (int i = 0; i < _shapes.size(); i++) {
       
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
            qDebug() << i <<"is Hitted";
            idx_model = i;
            obj[i]._shape_collide_state = WAS_HIT;

            update();
        }
        else {
            Logger::debug("Missed object " + std::to_string(i));
        }
        if (hitRecord.hitted() && hitRecord.t() < newRecord.t()) {
            newRecord = hitRecord;
            newObject = obj;
        }
    }
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
   
    camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f));

    
    
    
    easy_shader = new Shader("./plane.vert", "./plane.frag");
    full_shader = new Shader("./shader/colors.vert", "./shader/colors.frag");
    camera_cube_shader = new Shader(":/shader/shader/camera_cube.vert", ":/shader/shader/camera_cube.frag");

    boundary_shader = new Shader("boundary_shader.vert","boundary_shader.frag");

    /**** 渲染一个摄像机正方体******/
    camera_cube_model = new Model(camera_cube_objpath);
    _cmaera_objects = new GraphicsRenderer(camera_cube_model);
    _cmaera_objects->_shapeType = SHAPETYPE::CAMERA_RENDER;
    _shapes.push_back(_cmaera_objects);

    /**天空盒*/
    skybox = new SkyBox;
    /***载入shader**/
    ResourceManager::loadShader("skybox", ":/shader/shader/skybox.vert", ":/shader/shader/skybox.frag");
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
   
   
    

 
   // // 渲染所有_shape里的模型，公用一个shader？
   for (auto shape : _shapes) {
       if (shape->_shapeType == SHAPETYPE::EASY_RENDER) {
           makeCurrent();
           easy_shader->use();
           easy_shader->setMat4("view", view);
           easy_shader->setMat4("projection", projection);
    
           //pl_model.rotate(120.0f, QVector3D(0.0f, 0.0f, 1.0f));
           easy_shader->setMat4("model", shape->modelMatrix());
           shape->render(*easy_shader);
           shape->boundary().render();
           easy_shader->unuse();
           //update();
       }
       else if(shape->_shapeType == SHAPETYPE::FULL_RENDER) {
           makeCurrent();
           full_shader->use();
           full_shader->setMat4("view", view);
           full_shader->setMat4("projection", projection);
           // 灯的位置 full_shader->setVec3("light.position", camera->cameraPos);_shapes[0]->position())
           full_shader->setVec3("light.position", _shapes[0]->position());
           // 光的朝向full_shader->setVec3("light.direction", camera->cameraFront);
           
           auto dit = light_angle - _shapes[0]->position();
           full_shader->setVec3("light.direction", dit);
           
         /*  qDebug() <<"cameraPos" << camera->cameraFront;
           qDebug() << "light.position" << _shapes[0]->position();*/

           auto cutoff = qCos(qDegreesToRadians(12.5f));
           full_shader->setFloat("light.cutOff", cutoff);
           cutoff = qCos(qDegreesToRadians(17.5f));
           full_shader->setFloat("light.outerCutOff", cutoff);

           full_shader->setVec3("viewPos", camera->cameraPos);
           // light properties "ambient"（环境光）环境光的强弱 
           full_shader->setVec3("light.ambient", m_ambient);
           full_shader->setVec3("light.diffuse", m_diffuse);
           full_shader->setVec3("light.specular", m_specular);
           full_shader->setFloat("material.shininess", 32.0f);
           full_shader->setFloat("light.constant", m_constant);
           full_shader->setFloat("light.linear", m_linear);
           full_shader->setFloat("light.quadratic", m_quadratic);
           QMatrix4x4 cu_model;
           cu_model.setToIdentity();
           full_shader->setMat4("model", shape->modelMatrix());
           full_shader->setInt("bilnn", blinn);
           full_shader->setInt("direction_light", light_local==0x04);
           full_shader->setInt("isdirection", light_local == 0x02);
           full_shader->setInt("spotlight", light_local == 0x01);
           shape->render(*full_shader);
           //shape->boundary().render();
           full_shader->unuse();
           update();
       }
       else if (shape->_shapeType == SHAPETYPE::CAMERA_RENDER) {
           makeCurrent();
           camera_cube_shader->use();
           camera_cube_shader->setMat4("view", view);
           camera_cube_shader->setMat4("projection", projection);
           QMatrix4x4 cam_model;
           cam_model.setToIdentity();
           camera_cube_shader->setMat4("model", shape->modelMatrix());

          
           //qDebug() <<"now camera cube pos is" << shape->position();
           //camera_cube_model->render(*camera_cube_shader);
           shape->render(*camera_cube_shader);

           camera_cube_shader->unuse();
           update();
       }
       // 渲染盒子
       for (auto shape : _shapes) {
           boundary_shader->use();
           boundary_shader->setMat4("view", view);
           boundary_shader->setMat4("projection", projection);
           boundary_shader->setVec3("viewPos", camera->cameraPos);
           boundary_shader->setVec3("lightPos", _shapes[0]->position());

           QMatrix4x4 cu_model;
           cu_model.setToIdentity();
           boundary_shader->setMat4("model", cu_model);
           shape->boundary().render();
           boundary_shader->unuse();
       }


   }



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
        /*  float precision = 0.00005;       
            this->m_model.translate(dx_move * 1.0 * precision, dy_move * 1.0 * precision, 0.0f);*/
         
        //QVector2D delta{ dx_move , dy_move };
        float relX = (float)event->x() / (float)width();
        float relY = 1 - (float)event->y() / (float)height();
        Ray ray = camera->generateRay(QVector2D(relX, relY), (float)width() / (float)height());
        moveOperatingObject(ray);
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
    if (event->button() == Qt::LeftButton) {
        _pressedObject = _hoveredObject;
    }

}
// 触发
void my_opengl::mouseReleaseEvent(QMouseEvent* event)
{
    // State transfer
    bool startOperatingObject = false;
    if (_operatingObject != nullptr) {
        // Click when having an operating object
        _operatingObject->updateBoundary();
        if (!_dragged) {
            // if haven't changed since last mouse press, it's a submission click
            _operatingObject = nullptr;
            _hideBound = false;
        }
        else {
            // dragged, keep it operational
            _dragged = false;
            _hideBound = true;
            _operatingObject = _operatingObject;
        }
    }
    else if (_pressedObject != nullptr && _pressedObject == _selectedObject) {
        if (!_dragged) {
            // Double select on an object, set in operating mode
            _operatingObject = _selectedObject;
            _hideBound = true;
            startOperatingObject = true;
        }
        else {
            // keep it selected
            _dragged = false;
            _hideBound = false;
            _selectedObject->updateBoundary();
        }
    }
    else if (_dragged) {
        _dragged = false;
        _hideBound = false;
        if (_selectedObject != nullptr) {
            _selectedObject->updateBoundary();
        }
    }
    else {
        _selectedObject = _pressedObject;
        _hideBound = false;
        //emit onSelect(_selectedObject);
    }

    // Reset pressed object
    _pressedObject = nullptr;


    float relX = (float)event->x() / (float)width();
    float relY = 1 - (float)event->y() / (float)height();
    // 返回射线的方向和位置
    Ray ray = camera->generateRay(QVector2D(relX, relY), (float)width() / (float)height());
    //qDebug() <<"mouseReleaseEvent :ray.direction() " << ray.direction();
    auto car = hitTest(ray);
    //if (car != nullptr)qDebug() << car->_shapeType;
    if (startOperatingObject) {

    }
        //moveOperatingObject(ray);
    //qDebug() << ray.direction();

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

void my_opengl::setBlinn(bool isTrue)
{
    blinn = isTrue;
}

void my_opengl::choseLight(LIGHTTYPE light)
{
    switch (light)
    {
    case DIRECTIONLIGHT:
        m_ambient = QVector3D{ 0.8f, 0.8f, 0.8f };
        light_local = 4;
        break;
    case POINTLIGHT:
        m_ambient = QVector3D{ 0.2f, 0.2f, 0.2f };
        light_local = 2;
        break;
    case SPOTLIGHT:
        light_local = 1;
        m_ambient = QVector3D{ 0.2f, 0.2f, 0.2f };
        break;
    default:
        break;
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
    
    //qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;

        auto cur_pos = _shapes[idx_model]->position();
        float temp = (m_last_xRot - angle) * 2.0f * 0.001;
        _shapes[idx_model]->setPosition(cur_pos - QVector3D{temp, 0.0, 0.0f});

        // 这个转的快的原因是 原先m_world.setToIdentity();使其都为1，而现在
        // 相当于每次 新转这个180.0f - (m_xRot/16.0f)角度
        // 因此使用m_last_xRot 记录上下两次Rot的差值记录变化情况
        m_model.rotate((180.0f - ((m_last_xRot-m_xRot)/16.0f)), 1, 0, 0);
        m_last_xRot = angle;
        _shapes[idx_model]->updateBoundary();

        emit xRotationChanged(angle);
        update();
    }
}

void my_opengl::setYRotation(int angle)
{
    //qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        //m_model.rotate( ( m_last_yRot-m_yRot) / 16.0f, 0, 1, 0);
        auto cur_pos = _shapes[idx_model]->position();
        float temp = (m_last_yRot - angle)*2.0f*0.001;
        _shapes[idx_model]->setPosition(cur_pos - QVector3D{0.0, temp, 0.0f});
        m_last_yRot = angle;
        _shapes[idx_model]->updateBoundary();
        emit yRotationChanged(angle);
        update();
    }
}


void my_opengl::setZRotation(int angle)
{
    //qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        auto cur_pos = _shapes[idx_model]->position();
        float temp = (m_last_zRot - angle) * 2.0f * 0.001;
        _shapes[idx_model]->setPosition(cur_pos - QVector3D{0.0f, 0.0, temp});

        m_model.rotate((m_last_zRot -m_zRot) / 16.0f, 0, 0, 1);
        m_last_zRot = angle;
        _shapes[idx_model]->updateBoundary();
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
    makeCurrent();
    Model* newModel = new Model(base_cube_objpath);
    GraphicsRenderer* cur_object = new GraphicsRenderer(newModel);
    cur_object->_shapeType = SHAPETYPE::FULL_RENDER;
    _shapes.push_back(cur_object);

    parentWidget()->update();
}

void my_opengl::add_object_plane()
{
    qDebug() << "add plane";
    if (plane_cnt > 0) {
        //父窗口指针：nullptr 表示没有父窗口。
        QMessageBox::warning(nullptr, "警告", "只允许加载一个地板", QMessageBox::Ok);

        return;
    }
    makeCurrent();
    // GraphicsRenderer* 
    //_objects = new GraphicsRenderer(&cube_model);
    Model* newModel = new Model(base_plane_objpath);
    GraphicsRenderer* cur_object = new GraphicsRenderer(newModel);
    cur_object->_shapeType = SHAPETYPE::FULL_RENDER;
    _shapes.push_back(cur_object);
    //parentWidget()->update();
    plane_cnt++;
}

void my_opengl::add_object_bigtoy()
{
    makeCurrent();
    Model* newModel = new Model(nanosuit);
    GraphicsRenderer* cur_object = new GraphicsRenderer(newModel);
    cur_object->_shapeType = SHAPETYPE::FULL_RENDER;
    _shapes.push_back(cur_object);
    parentWidget()->update();
}


void my_opengl::moveOperatingObject(const Ray& ray) {
    
    hitTest(ray);
    if (!_hitRecord.hitted()) {
        QVector3D target = camera->cameraPos + ray.direction()*10.0f ;
        //qDebug() << "was not hitted and target is" << target;
        //target.setZ(0.0f);
        
      /*  _shapes[0]->setPosition(target);
        _shapes[0]->updateBoundary();*/
    }
    else {
        //auto target = _hit_record.position();
        //// _operatingObject->modelMatrix() QMatrix4x4  glm::mat4
        //auto temp = QVector4D(QVector3D{ 0.0f, 0.0f, 0.0f }, 1.0);
        //auto modelMatrix = _operatingObject->modelMatrix();
        //QVector4D cur = modelMatrix * QVector4D(QVector3D(0.0f, 0.0f, 0.0f), 1.0f);
        //QVector3D modelCenter{ cur.x(),cur.y(), cur.z() };
        //QVector4D bottomCenter_temp = modelMatrix * QVector4D(_operatingObject->model()->boundBox().bottomCenterPoint(), 1.0f);   // model center in world space
        //QVector3D bottomCenter{ bottomCenter_temp.x(), bottomCenter_temp.y(), bottomCenter_temp.z() };
        //QVector3D newCenter = target + modelCenter - bottomCenter;
        //_shapes[0]->setPosition(newCenter);
        //_operatingObject->setPosition(newCenter);

        // Update the boundary
        //_operatingObject->updateBoundary();
    }
    


}

void my_opengl::rest_last_xyz()
{
    m_last_xRot = 3000;
    m_last_yRot = 3000;
    m_last_zRot = 3000;
    emit rest_last_xyz_signal();
}

void my_opengl::set_light_roate(QVector3D angle)
{
    this->light_angle = angle;
}
