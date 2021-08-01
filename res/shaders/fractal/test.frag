#version 450

layout(location = 0) out vec4 out_color;

float scale = 0.4;
vec2 center = vec2(720, 360);
float itr = 400;

vec4 map_to_color(float t) {
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1.0);
}

void main() {
    vec2 z, c;

    c.x = ((gl_FragCoord.x - center.x) / 1024) / scale;
    c.y = ((gl_FragCoord.y - center.y) / 1024) / scale;

    int i;
    z = c;
    for(i = 0; i < itr; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }
    
    float col = (i == itr ? 1.0 : float(i)) / 100.0;
    
    out_color = map_to_color(col);
}
