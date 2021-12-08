#version 450

layout(input_attachment_index = 0, binding = 0) uniform subpassInput input_color;

layout(location = 0) out vec4 out_color;

void main() {
    vec3 color = subpassLoad(input_color).rgb;
    if (color.r == 0.0f && color.g == 0.0f && color.b == 0.0f)
        discard;
    out_color = vec4(color.r * 1.2f, color.g * 0.8f, color.b, 1.0f);
    //out_color = vec4(0.0f, color.r, 1.0f, 1.0f);
    //out_color = vec4(color, 1.0f);
}
