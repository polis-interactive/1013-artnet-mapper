
#version 310 es

precision highp float;

in vec2 texture_coordinates;

uniform sampler2D previous_render_texture;

out vec4 frag_color;

void main() {
    frag_color = texture(previous_render_texture, texture_coordinates);
}