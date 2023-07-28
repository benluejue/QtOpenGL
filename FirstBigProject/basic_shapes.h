#pragma once

#include <QVector3D>
#include "utils.h"
#include "vao.h"


/*
* 这个文件夹存储了一些常见的形状
* 如立方体,平面
*/
/*************立方体**********/
class Cube {
public:
	// 默认绘制一个1x1x1的立方体
	Cube();
	Cube(float vertices[]);
	void init();
	// vao(vbo, ebo) vbo std::vector<Vertex> vertices; ebo std::vector<unsigned int> indices;
	
	~Cube();
	void render(GLboolean isTexture = GL_FALSE, GLboolean isNormal = GL_FALSE) const;
private:
	float _vertices[1000];
	GLuint VBO;
};

class basic_shapes {
	// ... other class members
};
