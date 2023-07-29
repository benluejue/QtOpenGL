#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D plane_texture;

void main()
{    
    FragColor = texture(plane_texture, TexCoords);
}