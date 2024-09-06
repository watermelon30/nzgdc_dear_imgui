#version 430 core

out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    float distSquared = dot(coord, coord);
    if (distSquared > 1.0)
    {
        discard;
    }
    FragColor = vec4(0.52f, 0.8f, 0.92f, 1.0f);
}
