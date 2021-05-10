#version 410 core

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D pnoise;
uniform sampler2D wnoise;
uniform sampler2D flame;

void main() {
    float pnoise_value = texture(pnoise, f_tex_coord).x;
    float wnoise_value = texture(wnoise, f_tex_coord).x;
    float flame_value = texture(flame, f_tex_coord).x;
    
    float noise_value = pnoise_value;
    
    float s1 = step(pnoise_value, flame_value - 0.01f);
    float s2 = step(wnoise_value, s1);
    
    if (s2 > 0.1f) {
        color = vec4(s2, s2, s2, 1.0f);
    } else {
        color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

