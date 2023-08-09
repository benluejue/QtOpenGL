#pragma once

#include <QVector3D>
#include "utils.h"
#include "vao.h"
// 几个基本形状的基类
class basic_shapes {
public:
	virtual void init() = 0;
	virtual void render(GLboolean isTexture = GL_FALSE, GLboolean isNormal = GL_FALSE) const = 0;
	bool is_chosed;
};


/*
* 这个文件夹存储了一些常见的形状
* 如立方体,平面
*/
/*************立方体**********/
class Cube:public basic_shapes {
public:
	// 默认绘制一个1x1x1的立方体
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


