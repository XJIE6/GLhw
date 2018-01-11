#version 330

layout (location = 0) in vec3 position;
uniform float zoom;
uniform float x;
uniform float y;

out vec2 pos;

void main()
{
    gl_Position = vec4(position, 1.0);
    pos = vec2((position.x + x) / zoom, (position.y + y) / zoom);
}