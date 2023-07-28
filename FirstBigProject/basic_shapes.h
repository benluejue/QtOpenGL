#pragma once

#include <QVector3D>
#include "utils.h"
#include "vao.h"


/*
* ����ļ��д洢��һЩ��������״
* ��������,ƽ��
*/
/*************������**********/
class Cube {
public:
	// Ĭ�ϻ���һ��1x1x1��������
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
