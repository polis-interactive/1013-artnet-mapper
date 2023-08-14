
#version 310 es

precision mediump float;

in vec2 texture_coordinates;

uniform vec2 resolution;

uniform sampler2D previous_render;
uniform sampler2D artnet_texture;

out vec4 frag_color;

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution;
    frag_color = texture(previous_render, texture(artnet_texture, uv).rg);
}