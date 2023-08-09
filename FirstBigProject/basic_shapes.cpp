#include "basic_shapes.h"
#include "utils.h"





Cube::Cube():VBO(0), VAO(0)
{
    
}

Cube::Cube(float vertices[])
{

}

void Cube::init()
{
    float vertices[] = {
        // positions        // textures        // normals
        //Back Face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f, //bottom-left
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, //top-right
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,

        //Front Face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,

        //Left Face
        -0.5f,  0.5f,  0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

        //Right Face
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,

         //Bottom Face
         -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,

         //Top Face
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    };
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &VAO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);

    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &VBO);

    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}



Cube::~Cube()
{
    if(VBO != 0)
        OPENGL_EXTRA_FUNCTIONS->glDeleteBuffers(1, &VBO);
    if (VAO != 0)
        OPENGL_EXTRA_FUNCTIONS->glDeleteVertexArrays(1, &VAO);
}

void Cube::render(GLboolean isTexture, GLboolean isNormal) const
{
    
    OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 少了这行不行，因为只有绑定了这一行，后面关于定点数据的内容才能正常运行，因为这个VAO记录了定点配置等数据
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);

    OPENGL_FUNCTIONS->glEnableVertexAttribArray(0);
    OPENGL_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    if (isTexture) {
        OPENGL_FUNCTIONS->glEnableVertexAttribArray(1);
        OPENGL_FUNCTIONS->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    if (isNormal) {
        OPENGL_FUNCTIONS->glEnableVertexAttribArray(2);
        OPENGL_FUNCTIONS->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    }

    OPENGL_FUNCTIONS->glDrawArrays(GL_TRIANGLES, 0, 36);
}

Plane::Plane()
    :VAO(0),VBO(0)
{
}
Plane::~Plane() {
    if (VBO != 0)
        OPENGL_EXTRA_FUNCTIONS->glDeleteBuffers(1, &VBO);
    if (VAO != 0)
        OPENGL_EXTRA_FUNCTIONS->glDeleteVertexArrays(1, &VAO);
}

void Plane::init() {
    float vertices[] = {
        // positions        // textures        // normals
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &VAO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);

    OPENGL_FUNCTIONS->glGenBuffers(1, &VBO);

    OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    OPENGL_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Plane::render(GLboolean isTexture, GLboolean isNormal) const{
    OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);

    OPENGL_FUNCTIONS->glEnableVertexAttribArray(0);
    OPENGL_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    if (isTexture) {
        OPENGL_FUNCTIONS->glEnableVertexAttribArray(1);
        OPENGL_FUNCTIONS->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    if (isNormal) {
        OPENGL_FUNCTIONS->glEnableVertexAttribArray(2);
        OPENGL_FUNCTIONS->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    }

    OPENGL_FUNCTIONS->glDrawArrays(GL_TRIANGLES, 0, 6);
}
