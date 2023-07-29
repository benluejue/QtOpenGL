#include "shader2.h"
void Shader2::compile(const QString& vertexSource, const QString& fragmentSource, const QString& geometrySource) {
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceFile(vertexSource);
    if (!success) {
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED";
        qDebug() << vertexShader.log();
    }

    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success = fragmentShader.compileSourceFile(fragmentSource);
    if (!success) {
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED";
        qDebug() << fragmentShader.log();
    }

    QOpenGLShader geometryShader(QOpenGLShader::Geometry);
    if (geometrySource != nullptr) {
        success = geometryShader.compileSourceFile(geometrySource);
        if (!success) {
            qDebug() << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED";
            qDebug() << geometryShader.log();
        }
    }


    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShader(&vertexShader);
    shaderProgram->addShader(&fragmentShader);
    if (geometrySource != nullptr)
        shaderProgram->addShader(&geometryShader);
    success = shaderProgram->link();
    if (!success) {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED";
        qDebug() << shaderProgram->log();
    }
}

Shader2::~Shader2() {
    //没有析构的ShaderProgram，可tnd的自带opengl函数没有对QOpenGLShaderProgram的 delete删除函数啊

//    delete shaderProgram;
}


