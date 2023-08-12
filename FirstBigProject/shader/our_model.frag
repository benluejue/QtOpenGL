#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// 材质属性
struct Material {
//在着色器中，vec3 类型的变量用于存储和计算颜色值，而 sampler2D 类型的变量用于从纹理中采样颜色值
    //vec3 ambient; // 环境光颜色
    //vec3 diffuse;  // 漫反射
    sampler2D  specular;   // 镜面反射
    float shininess; // 反光度(Shininess)个物体的反光度越高，反射光的能力越强
    sampler2D diffuse;
    // sampler2D emission; //放射光贴图
}; 

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
void main()
{    
    FragColor = texture(texture_diffuse1+texture_specular1, TexCoords);
}
