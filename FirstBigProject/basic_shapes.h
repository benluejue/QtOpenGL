#pragma once

#include <QVector3D>
#include "utils.h"
#include "vao.h"
// ����������״�Ļ���
class basic_shapes {
public:
	virtual void init() = 0;
	virtual void render(GLboolean isTexture = GL_FALSE, GLboolean isNormal = GL_FALSE) const = 0;
	bool is_chosed;
};


/*
* ����ļ��д洢��һЩ��������״
* ��������,ƽ��
*/
/*************������**********/
class Cube:public basic_shapes {
public:
	// Ĭ�ϻ���һ��1x1x1��������
	Cube();
	Cube(float vertices[]);
	virtual void init() override;
	
	~Cube();
	virtual void render(GLboolean isTexture = GL_FALSE, GLboolean isNormal = GL_FALSE) const override;
private:
	float _vertices[1000];
	GLuint VAO,VBO;
};

// plane
class Plane :public basic_shapes {
public:
	Plane();
	~Plane();
	virtual void init() override;
	virtual void render(GLboolean isTexture = GL_FALSE, GLboolean isNormal = GL_FALSE) const override;
private:
	GLuint VBO, VAO;
};


