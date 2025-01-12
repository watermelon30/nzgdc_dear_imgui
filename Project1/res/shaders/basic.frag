#version 430 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_sampler;

void main()
{
    FragColor = texture(texture_sampler, TexCoord);
}