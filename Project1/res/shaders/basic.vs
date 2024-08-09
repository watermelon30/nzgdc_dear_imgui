#version 330 core

layout (location = 0) in vec3 vertex_position;

void main()
{
    gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);
}