
#version 310 es

precision mediump float;

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
    vec2 tex_uv = texture_coordinates.xy / vec2(2.0, 2.0);
    frag_color = vec4(vec3(texture(pixel_type_texture, tex_uv).r), 1.0);
    return;
    /*
    vec2 uv = gl_FragCoord.xy / resolution;
    if (do_artnet_mapping) {
        uv = texture(artnet_texture, uv).rg;
    };
    float use_brightness = brightness * 0.75;
    float pixel_type = texture(pixel_type_texture, uv).r;
    if (pixel_type < 0.3) {
        if(time > 1000000.0) {
            // dummy for shader compilation
            frag_color = vec4(vec3(0.0, 0.0, 0.0), 1.0);
        } else {
            frag_color = vec4(0.0);

        }
    } else if (pixel_type < 0.6) {
        frag_color = vec4(use_brightness, 0.0, use_brightness, 1.0);
    } else if (pixel_type < 0.9) {
        // dummy for shader compilation
        if (pixel_multiplier > 5) {
            frag_color = vec4(vec3(0.0), 1.0);
        } else {
            frag_color = vec4(0.0);
        }
    } else {
        frag_color = vec4(vec3(use_brightness), 1.0);
    }
    */
}