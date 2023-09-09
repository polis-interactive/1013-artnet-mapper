
#version 300 es

precision highp float;

in vec2 texture_coordinates;

uniform vec2 resolution;

uniform sampler2D previous_render_texture;
uniform sampler2D artnet_texture;

out vec4 frag_color;

void main()
{
    vec2 tex_uv = texture(artnet_texture, texture_coordinates).rg;
    frag_color = texture(previous_render_texture, tex_uv);
}