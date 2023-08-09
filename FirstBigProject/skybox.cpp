#include "skybox.h"

SkyBox::SkyBox()
{
    faces.clear();
    QString path = "D:/OpenGL/VSOpenGL/FirstBigProject/FirstBigProject/skybox";
    faces.push_back(path + "/right.jpg");
    faces.push_back(path + "/left.jpg");
    faces.push_back(path + "/top.jpg");
    faces.push_back(path + "/bottom.jpg");
    faces.push_back(path + "/front.jpg");
    faces.push_back(path + "/back.jpg");

    cubemapTexture = loadCubemap(faces);

    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &skyboxVAO);
    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &skyboxVBO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(skyboxVAO);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(0);
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void SkyBox::render() {
    OPENGL_EXTRA_FUNCTIONS->glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    // skybox cube
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(skyboxVAO);
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0);
    cubemapTexture->bind();
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(0);
    OPENGL_EXTRA_FUNCTIONS->glDrawArrays(GL_TRIANGLES, 0, 36);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
    OPENGL_EXTRA_FUNCTIONS->glDepthFunc(GL_LESS); // set depth function back to default
}

QOpenGLTexture* SkyBox::loadCubemap(std::vector<QString> faces)
{
    // ��ת
    // stbi_set_flip_vertically_on_load(false);
    
    
    QImage posX = QImage(faces[0]).convertToFormat(QImage::Format_RGB888); //Right��Ĭ������λ32λRGB��������Ҫ�󣬱���תΪ24λRGB��
    QImage negX = QImage(faces[1]).convertToFormat(QImage::Format_RGB888); //Left
    QImage posY = QImage(faces[2]).convertToFormat(QImage::Format_RGB888); //Top
    QImage negY = QImage(faces[3]).convertToFormat(QImage::Format_RGB888); //Bottom
    QImage posZ = QImage(faces[4]).convertToFormat(QImage::Format_RGB888); //Front
    QImage negZ = QImage(faces[5]).convertToFormat(QImage::Format_RGB888); //Back

    auto skyboxTexture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);

    skyboxTexture->setSize(posX.width(), posX.height(), posX.depth()); //����Ҳ²� ��ȷ��һ������ĳߴ磬Ȼ��allocate���亯��������TargeCubeMap��������������Ŀռ�
    skyboxTexture->setFormat(QOpenGLTexture::RGBFormat);
    //skyboxTexture->create();
    skyboxTexture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8); //�����ڴ�

    skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posX.bits());
    skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posY.bits());
    skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posZ.bits());
    skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negX.bits());
    skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negY.bits());
    skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negZ.bits());
   /* for (unsigned int i = 0; i < faces.size(); i++)
    {
        QImage curImg = QImage(faces[i]).convertToFormat(QImage::Format_RGB888);
        QOpenGLTexture::CubeMapFace cubMapFace = (QOpenGLTexture::CubeMapFace)((int)QOpenGLTexture::CubeMapNegativeX+i);
        skyboxTexture->setData(0,0, cubMapFace, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)curImg.bits());

    }*/

    skyboxTexture->setMinificationFilter(QOpenGLTexture::Linear);     //����Ŵ����Сʱ�����ص�ȡֵ���� �����Ի�ͽ�����
    skyboxTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    skyboxTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);   //���������Ե����չ����
    skyboxTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
   

    return skyboxTexture;
}