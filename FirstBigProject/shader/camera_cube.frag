#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords); // 从纹理中采样颜色
    
     if (texColor.a < 0.5) { // 这里的0.5可以根据需要调整
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // 将透明部分替换为白色
    } else {
        FragColor = texColor; // 否则使用纹理颜色
    }
}
