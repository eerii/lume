#version 410 core

layout(location = 0) out vec4 color;

uniform vec3 c;

void main() {
    color = vec4(c, 1.0f);
}
