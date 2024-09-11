#version 430 core

in vec2 vel;
in vec2 pos;

out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    float distSquared = dot(coord, coord);
    if (distSquared > 1.0)
    {
        discard;
    }
    // FragColor = vec4(0.52f, 0.8f, 0.92f, 1.0f);
    FragColor = vec4((pos.x + 1.0) * 0.5, (pos.y + 1.0) * 0.5, (pos.x + 1.0) * 0.25 + (pos.y + 1.0) * 0.25, sin(pos.x));
}
