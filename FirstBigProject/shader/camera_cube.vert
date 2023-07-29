#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);


void main()
{
    // 模型(Model)、观察(View)、投影(Projection)三个矩阵
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
 

}