
#version 310 es

precision mediump float;

in vec2 texture_coordinates;

uniform sampler2D pixel_type_texture;
uniform sampler2D artnet_texture;

out vec4 frag_color;

void main()
{
    frag_color = vec4(texture(artnet_texture, texture_coordinates).rg, 0.0, 1.0);
}