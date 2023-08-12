#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords); // �������в�����ɫ
    
     if (texColor.a < 0.5) { // �����0.5���Ը�����Ҫ����
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // ��͸�������滻Ϊ��ɫ
    } else {
        FragColor = texColor; // ����ʹ��������ɫ
    }
}
