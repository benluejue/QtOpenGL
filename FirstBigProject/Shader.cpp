#include "shader.h"

Shader::Shader(const QString& vertexPath, const QString& fragmentPath) {
	QOpenGLShader vertexShader(QOpenGLShader::Vertex);
	bool success = vertexShader.compileSourceFile(vertexPath);
	if (!success) {
		qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" ;
		qDebug() << vertexShader.log();
	}

	QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
	success = fragmentShader.compileSourceFile(fragmentPath);
	if (!success) {
		qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED";
		qDebug() << fragmentShader.log();
	}//

	shaderProgram.addShader(&vertexShader);
	shaderProgram.addShader(&fragmentShader);
	success = shaderProgram.link();
	if (!success) {
		qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED" ;
		qDebug() << shaderProgram.log();
	}
}





Shader::~Shader() {
}


void Shader::use()
{
	shaderProgram.bind();
}

void Shader::setMat4(const QString& name, const QMatrix4x4& value)
{

	GLuint loc = shaderProgram.uniformLocation(name);
	shaderProgram.setUniformValue(loc, value);

}

void Shader::setFloat(const QString& name, const GLfloat& value)
{
	GLuint loc = shaderProgram.uniformLocation(name);
	shaderProgram.setUniformValue(loc, value);
}

void Shader::setVec4(const QString& name, QVector4D& vec4d)
{
	GLuint loc = shaderProgram.uniformLocation(name);
	shaderProgram.setUniformValue(loc, vec4d);

}

void Shader::setVec3(const QString& name,const QVector3D& vec3d)
{
	GLuint loc = shaderProgram.uniformLocation(name);
	shaderProgram.setUniformValue(loc, vec3d);
}
