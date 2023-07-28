#include "my_opengl.h"

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
    delete planeShader;
    delete boundaryShader;
    
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
    //connect(ui.X_Slider, &QSlider::valueChanged, ui.lcdNumber, QOverload<int>::of(&QLCDNumber::display));
    // connect(glWidget, &GLWidget::xRotationChanged, xSlider, &QSlider::setValue);
    // connect(this,  xRotationChanged, , );
    LightShader = new Shader("./shader/colors.vert","./shader/colors.frag");
    planeShader = new Shader("./plane.vert", "./plane.frag");
    boundaryShader = new Shader("./boundary_shader.vert", "./boundary_shader.frag");

    camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f));
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &VAO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);
    // 代表了显卡缓冲的地址
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定缓冲对象
    // 将顶点数据传输到显存中
    glBufferData(GL_ARRAY_BUFFER, vertices_num, vertices, GL_STATIC_DRAW);

    // 顶点 法向量 texture纹理
    //                                              // 每行几个
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);

    // plane 
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);


    //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    // G:\c++\VSOpenGL\FirstBigProject\FirstBigProject\texture
    diffuseMap = new QOpenGLTexture(QImage(container2).mirrored(), QOpenGLTexture::GenerateMipMaps);
    diffuseMap->setWrapMode(QOpenGLTexture::Repeat);
    diffuseMap->setMinificationFilter(QOpenGLTexture::Nearest);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    diffuseMap->setMagnificationFilter(QOpenGLTexture::Linear);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    specularMap = new QOpenGLTexture(QImage(specular).mirrored(), QOpenGLTexture::GenerateMipMaps);
    specularMap->setWrapMode(QOpenGLTexture::Repeat);
    specularMap->setMinificationFilter(QOpenGLTexture::Nearest);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    specularMap->setMagnificationFilter(QOpenGLTexture::Linear);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // metal贴图
    metalMap = new QOpenGLTexture(QImage(metal_path2).mirrored(), QOpenGLTexture::GenerateMipMaps);
    metalMap->setWrapMode(QOpenGLTexture::Repeat);
    metalMap->setMinificationFilter(QOpenGLTexture::Nearest);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    metalMap->setMagnificationFilter(QOpenGLTexture::Linear);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    LightShader->use();
    LightShader->shaderProgram.setUniformValue(LightShader->shaderProgram.uniformLocation("material.diffuse"), 0);
    LightShader->shaderProgram.setUniformValue(LightShader->shaderProgram.uniformLocation("material.specular"), 1);

    // 相对应glActiveTexture(GL_TEXTURE2);
    // metalMap->bind();
    planeShader->use();
    planeShader->shaderProgram.setUniformValue(planeShader->shaderProgram.uniformLocation("texture1"), 2);

    boundaryShader->use();

     



    // 绑定空缓冲区 作为结尾 将当前的VAO,VBO与OpenGL上下文分离。
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    

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

    glActiveTexture(GL_TEXTURE0);
    diffuseMap->bind();
   
    glActiveTexture(GL_TEXTURE1);
    specularMap->bind();


    glActiveTexture(GL_TEXTURE2);
    metalMap->bind();


    LightShader->use();
    QVector3D lightPos = QVector3D(1.2f, 1.0f, 2.0f);
    LightShader->setVec3("light.position", camera->cameraPos);
    LightShader->setVec3("light.direction", camera->cameraDirection);
    auto cutoff = qCos(qDegreesToRadians(12.5f));
    LightShader->setFloat("light.cutOff", cutoff);
    cutoff = qCos(qDegreesToRadians(17.5f));
    LightShader->setFloat("light.outerCutOff", cutoff);

    LightShader->setVec3("viewPos", camera->cameraPos);
    // light properties "ambient"（环境光）环境光的强弱
    // CubeShader->setVec3("objectColor", this->objectColor);
    
   
    LightShader->setVec3("light.ambient", m_ambient);
    LightShader->setVec3("light.diffuse", m_diffuse);
    LightShader->setVec3("light.specular", m_specular);
    LightShader->setFloat("material.shininess", 32.0f);


    // 相机调用
    // model 改变看的角度， view看的大小，project投射到屏幕上
    //m_model.setToIdentity();
    /*m_model.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_model.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_model.rotate(m_zRot / 16.0f, 0, 0, 1);*/
    LightShader->setMat4("model", m_model);
    LightShader->setFloat("light.constant", m_constant);
    LightShader->setFloat("light.linear", m_linear);
    LightShader->setFloat("light.quadratic", m_quadratic);

    this->view = camera->getViewMat();
    LightShader->setMat4("view", this->view);
    this->projection.setToIdentity();
    this->projection.perspective(camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 100.f);
    
    LightShader->setMat4("projection", this->projection);
    //绑定VAO的，进行绘制
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 绘制 包装盒
    boundaryShader->use();
    boundaryShader->setMat4("view", this->view);
    boundaryShader->setMat4("projection", this->projection);
    boundaryShader->setMat4("model", m_model);
    boundary.render();
    update();


    // 绘制 plane
    planeShader->use();
    planeShader->setMat4("projection", this->projection);
    planeShader->setMat4("view", this->view);
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();  // 设置为单位矩阵
    planeShader->setMat4("model", modelMatrix);
    
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    update();
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

        float precision = 0.00003;
 
        
        // qDebug() << dx * 1.0 * precision << " " << dy * 1.0 * precision;
        this->m_model.translate(dx_move * 1.0 * precision, dy_move * 1.0 * precision, 0.0f);
        //qDebug() << this->m_model;
        update();


    }
    //setCursor(Qt::ArrowCursor);
}
// 触发
void my_opengl::wheelEvent(QWheelEvent* event)
{
    //向前滚动一下是offset=0,120
    QPoint offset = event->angleDelta();
    qDebug() << "wheelEvent "<<offset;
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
    qDebug() << "left_Campos "<<camera->cameraPos;
    // cameraPos -= QVector3D(0.0f, 1.0f, 0.0f) * velocity;
    camera->cameraPos += QVector3D(1.0f, 0.0f, 0.0f) * vel;
    update();
}
void my_opengl::Campos_right() 
{
    qDebug() << "right_Campos";
    // cameraPos -= QVector3D(0.0f, 1.0f, 0.0f) * velocity;
    camera->cameraPos -= QVector3D(1.0f, 0.0f, 0.0f) * vel;
    update();
}

void my_opengl::Campos_down()
{
    qDebug() << "Campos_down";
    camera->cameraPos += QVector3D(0.0f, 1.0f, 0.0f) * vel;
    update();
}

void my_opengl::Campos_up()
{
    qDebug() << "Campos_up";
    camera->cameraPos -= QVector3D(0.0f, 1.0f, 0.0f) * vel;
    update();
}

void my_opengl::setAmbient(int value)
{
    float f_valuse = value/10;
    m_ambient = QVector3D(0.1f, 0.1f, 0.1f)* f_valuse;
    qDebug() << m_ambient;
    update();
}

void my_opengl::set_diffuse(int diffuse)
{
    float f_valuse = diffuse / 10;
    m_diffuse = QVector3D(0.1f, 0.1f, 0.1f) * f_valuse;
    qDebug() << m_diffuse;
    update();
}

void my_opengl::set_specular(int specular)
{
    float f_valuse = specular / 10;
    m_specular = QVector3D(0.1f, 0.1f, 0.1f) * f_valuse;
    qDebug() << m_specular;
    update();
}


