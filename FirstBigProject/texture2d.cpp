#include "texture2d.h"

Texture2D::Texture2D() :texture(NULL), internal_format(QOpenGLTexture::RGBFormat),
wrap_s(QOpenGLTexture::Repeat), wrap_t(QOpenGLTexture::Repeat), filter_min(QOpenGLTexture::Linear),
filter_max(QOpenGLTexture::Linear)
{
    
}

Texture2D::Texture2D(TextureType type, std::string path)
{
    _type = type;
    // 文件名字可能有问题
    _path = path.substr(path.find_last_of('/') + 1);

    QString qfile = QString::fromStdString(path);
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    texture->setFormat(internal_format);
    // 使用gltf模型时候QImage(qfile)不需要翻转，否则需要QImage(qfile).mirrored()
    texture->setData(QImage(qfile).mirrored(), QOpenGLTexture::GenerateMipMaps);

    texture->setWrapMode(QOpenGLTexture::DirectionS, wrap_s);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionT, wrap_t);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture->setMinificationFilter(filter_min);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setMagnificationFilter(filter_max);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

Texture2D::~Texture2D()
{
    //资源无法释放，在ResourceManager里解决这个问题
    //delete texture;
    //texture->release();
}

void Texture2D::generate(const QString& file)
{
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    texture->setFormat(internal_format);
    texture->setData(QImage(file).mirrored(), QOpenGLTexture::GenerateMipMaps);

    texture->setWrapMode(QOpenGLTexture::DirectionS, wrap_s);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionT, wrap_t);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture->setMinificationFilter(filter_min);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

