#version 430 core

layout (std430, binding = 0) buffer Pos {
    vec2 Positions[];
};

layout (std430, binding = 1) buffer Vel {
    vec2 Velocities[];
};

uniform uint windowWidth;
uniform uint windowHeight;

out vec2 vel;
out vec2 pos;

void main()
{
    gl_Position = vec4(Positions[gl_VertexID].x / windowWidth * 2.0f - 1.0f, Positions[gl_VertexID].y / windowHeight * 2.0f - 1.0f, 1.0f, 1.0f);
    vel = vec2(Velocities[gl_VertexID].x, Velocities[gl_VertexID].y);
    pos = gl_Position.xy;
}
