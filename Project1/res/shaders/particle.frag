#version 330 core

in float currentLifetime;

uniform float totalLifetime;
uniform vec4 startColor;
uniform vec4 endColor;

void main()
{
    float t = clamp(currentLifetime / totalLifetime, 0.0, 1.0);
    vec4 currentColor = mix(startColor, endColor, t);
    
    gl_FragColor = currentColor;
}