
#version 310 es

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

void main()
{
    vec2 tex_uv = texture_coordinates;
    if (do_artnet_mapping) {
        tex_uv = texture(artnet_texture, tex_uv).rg;
    };

    float pixel_type = texture(pixel_type_texture, tex_uv).r;

    if (pixel_type < 0.3) {
        // no pixels
        frag_color = vec4(0.6, 0.0, 0.0, 1.0);
    } else if (pixel_type < 0.6) {
        // ghost pixels
        frag_color = vec4(0.0, 0.0, 0.0, 1.0);
    } else if (pixel_type < 0.9) {
        // n pixels
        if (tex_uv.x < 0.5) {
            frag_color = vec4(0.0, 0.0, 0.6, 1.0);
        } else {
            frag_color = vec4(vec3(0.0), 1.0);
        }
        /*
        float barPosition = mod(time * 0.1, 1.0);
        if (tex_uv.x > barPosition - 0.075 && tex_uv.x < barPosition + 0.075) {
            frag_color = vec4(0.0, 0.0, 0.6, 1.0);
        } else {
            frag_color = vec4(vec3(0.0), 1.0);
        }
        */
    } else {
        // drop pixels
        /*
        float barPosition = mod((time + 123.89123) * 0.05, 1.0);
        if (tex_uv.y > barPosition - 0.075 && tex_uv.y < barPosition + 0.075) {
            frag_color = vec4(0.0, 0.6, 0.0, 1.0);
        } else {
            frag_color = vec4(vec3(0.0), 1.0);
        }
        */
        if (tex_uv.x < 0.5) {
            frag_color = vec4(0.0, 0.6, 0.0, 1.0);
        } else {
            frag_color = vec4(vec3(0.0), 1.0);
        }
    }
}