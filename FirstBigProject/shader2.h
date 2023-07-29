#ifndef SHADER2_H
#define SHADER2_H

#include <QDebug>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QString>

class Shader2 {
    friend class ResourceManager;
public:
    Shader2() {}
    void compile(const QString& vertexSource, const QString& fragmentSource, const QString& geometrySource = NULL);
    //  Shader(const QString& vertexSourcePath, const QString& fragmentSourcePath);

    ~Shader2();


    Shader2& use() {
        shaderProgram->bind();
        return *this;
    }

    //���ǰ�������ɫ��uniform��������д��Shader���inline��Ա����������ķ���ܶࡣ
    void setFloat(const QString& name, const GLfloat& value) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setInteger(const QString& name, const GLint& value) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setVector2f(const QString& name, const GLfloat& x, const GLfloat& y) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, QVector2D(x, y));
    }

    void setVector2f(const QString& name, const QVector2D& value) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setVector3f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, QVector3D(x, y, z));
    }

    void setVector3f(const QString& name, const QVector3D& value) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setVector4f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, QVector4D(x, y, z, w));
    }

    void setVector4f(const QString& name, const QVector4D& value) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setMatrix4f(const QString& name, const QMatrix4x4& value) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setBool(const QString& name, const GLboolean& value) {
        GLuint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }
private:
    QOpenGLShaderProgram* shaderProgram;//������resourcemanager ɾ��һ��

};

#endif // SHADER_H
