
#version 300 es

precision highp float;

in vec2 texture_coordinates;

uniform float time;
uniform float brightness;

uniform int pixel_multiplier;
uniform vec2 resolution;

uniform sampler2D pixel_type_texture;

uniform bool do_artnet_mapping;
uniform sampler2D artnet_texture;

out vec4 frag_color;

#define TAU 6.28318530718
#define PI 3.14158
#define MAX_ITER 9

float rand(float n){return fract(sin(n) * 43758.5453123);}

float noise(float p){
    float fl = floor(p);
    float fc = fract(p);
    return mix(rand(fl), rand(fl + 1.0), fc);
}

float pcurve( float x, float a, float b ){
    float k = pow(a+b,a+b) / (pow(a,a)*pow(b,b));
    return k * pow( x, a ) * pow( 1.0-x, b );
}


vec4 drawNPixels(vec2 uv) {
    float t1 = time * 0.1;
    uv.y = uv.y * 2.1;
    uv.x = uv.x * 0.7 + time * 0.04 ;
    vec2 p = mod(uv*TAU, TAU)-250.0;
    vec2 i = vec2(p);
    float c = 1.0;
    float inten = .005;
    float contrast = 0.5;
    float gamma = 4.0;

    for (int n = 0; n < MAX_ITER; n++)
    {
        float t = t1 * (1.0 - (3.5 / float(n+1)));
        i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
        c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
    }
    c /= float(MAX_ITER);
    c = 1.17-pow(c, 1.4);
    float value = pow(abs(c), 8.0);
    value += brightness;
    value = mix(0.5, value, contrast);
    vec3 color = vec3(0.0, value * 0.6, value);

    return vec4(pow(color, vec3(gamma)), 1.0);

}

void main()
{
    if (brightness < 0.0001) {
        frag_color = vec4(0.0);
        return;
    }
    vec2 tex_uv = texture_coordinates;
    tex_uv = (floor(vec2(tex_uv.x * resolution.x, tex_uv.y * resolution.y)) + 0.5) / resolution;
    if (do_artnet_mapping) {
        tex_uv = texture(artnet_texture, tex_uv).rg;
    }

    float pixel_type = texture(pixel_type_texture, tex_uv).r;

    if (pixel_type < 0.3) {
        // no pixels
        frag_color = vec4(0.6, 0.0, 0.0, 1.0);
    } else if (pixel_type < 0.6) {
        // ghost pixels
        frag_color = vec4(0.0, 0.0, 0.0, 1.0);
    } else if (pixel_type < 0.9) {
        frag_color = drawNPixels(tex_uv);
    } else {
        // drop pixels

        frag_color = vec4(vec3(mix(0.1, 0.45, brightness)), 1.0);
        /*
        float barPosition = mod((time + 123.89123) * 0.05, 1.0);
        if (tex_uv.y > barPosition - 0.075 && tex_uv.y < barPosition + 0.075) {
            frag_color = vec4(0.0, 0.6, 0.0, 1.0);
        } else {
            frag_color = vec4(vec3(0.0), 1.0);
        }
        /*
        if (tex_uv.x < 0.5) {
            frag_color = vec4(0.0, 0.6, 0.0, 1.0);
        } else {
            frag_color = vec4(vec3(0.0), 1.0);
        }
        */
    }
}