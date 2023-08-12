#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// ��������
struct Material {
//����ɫ���У�vec3 ���͵ı������ڴ洢�ͼ�����ɫֵ���� sampler2D ���͵ı������ڴ������в�����ɫֵ
    //vec3 ambient; // ��������ɫ
    //vec3 diffuse;  // ������
    sampler2D  specular;   // ���淴��
    float shininess; // �����(Shininess)������ķ����Խ�ߣ�����������Խǿ
    sampler2D diffuse;
    // sampler2D emission; //�������ͼ
}; 

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
void main()
{    
    FragColor = texture(texture_diffuse1+texture_specular1, TexCoords);
}
