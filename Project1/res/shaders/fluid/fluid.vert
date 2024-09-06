#version 430 core

layout (std430, binding = 0) buffer Pos {
    vec2 Positions[];
};

uniform uint windowWidth;
uniform uint windowHeight;

void main()
{
    gl_Position = vec4(Positions[gl_VertexID].x / windowWidth * 2.0f - 1.0f, Positions[gl_VertexID].y / windowHeight * 2.0f - 1.0f, 1.0f, 1.0f);
}
