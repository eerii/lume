#version 410 core

#define MAX_LIGHTS 32
#define SMOOTHNESS 0.5

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

//Light
uniform vec4 light[MAX_LIGHTS];
uniform int light_size;
uniform float light_distortion;

float smooth_min(float a, float b, float k) {
    float h = max(k-abs(a-b), 0.0) / k;
    float v = float(h*h*h*k);
    return min(a,b) - v/6.0;
}

void main() {
    float light_accumulator = 1.0;
    float light_dithering = 0.0;
    
    float luminance = 1.0;
    
    for (int i = 0; i < light_size; i++) {
        if (light[i].w <= 0.0)
            break;
        vec2 to_light = abs(light[i].xy - f_tex_coord);
        to_light.x *= light_distortion;
        float dst_to_light = sqrt(to_light.x * to_light.x + to_light.y * to_light.y);
        
        float light_at_point = (dst_to_light - light[i].w) / (light[i].z - light[i].w);
        light_accumulator = (dst_to_light > light[i].w) ? smooth_min(light_at_point, light_accumulator, SMOOTHNESS) : 0.0;
        if (light_accumulator < 0.0)
            light_accumulator = 0.0;
    }
    
    float light_at_point = 1.0 - light_accumulator;
    
    luminance *= light_at_point;
    
    if (luminance > 0.999)
        luminance = 0.999;
    
    vec3 luma_color = luminance * vec3(1.0, 1.0, 1.0);
    color = vec4(luma_color, 1.0);
}
