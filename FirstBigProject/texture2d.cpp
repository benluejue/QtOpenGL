#include "texture2d.h"

Texture2D::Texture2D() :texture(NULL), internal_format(QOpenGLTexture::RGBFormat),
wrap_s(QOpenGLTexture::Repeat), wrap_t(QOpenGLTexture::Repeat), filter_min(QOpenGLTexture::Linear),
filter_max(QOpenGLTexture::Linear)
{
    
}

Texture2D::Texture2D(TextureType type, std::string path)
{
    _type = type;
    // �ļ����ֿ���������
    _path = path.substr(path.find_last_of('/') + 1);

    QString qfile = QString::fromStdString(path);
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D); //ֱ�����ɰ�һ��2d����, �����ɶ༶����MipMaps
    texture->setFormat(internal_format);
    // ʹ��gltfģ��ʱ��QImage(qfile)����Ҫ��ת��������ҪQImage(qfile).mirrored()
    texture->setData(QImage(qfile).mirrored(), QOpenGLTexture::GenerateMipMaps);

    texture->setWrapMode(QOpenGLTexture::DirectionS, wrap_s);// ����glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionT, wrap_t);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture->setMinificationFilter(filter_min);   //�ȼ���glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setMagnificationFilter(filter_max);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

Texture2D::~Texture2D()
{
    //��Դ�޷��ͷţ���ResourceManager�����������
    //delete texture;
    //texture->release();
}

void Texture2D::generate(const QString& file)
{
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D); //ֱ�����ɰ�һ��2d����, �����ɶ༶����MipMaps
    texture->setFormat(internal_format);
    texture->setData(QImage(file).mirrored(), QOpenGLTexture::GenerateMipMaps);

    texture->setWrapMode(QOpenGLTexture::DirectionS, wrap_s);// ����glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionT, wrap_t);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture->setMinificationFilter(filter_min);   //�ȼ���glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setMagnificationFilter(filter_max);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::bind() const
{
    texture->bind();
}

std::string Texture2D::path() const
{
    return _path;
}

TextureType Texture2D::type() const
{
    return _type;
}

