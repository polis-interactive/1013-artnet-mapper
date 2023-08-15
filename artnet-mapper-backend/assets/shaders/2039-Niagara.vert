
#version 310 es
layout (location = 0) in vec3 v_position;
layout (location = 2) in vec2 v_texture;

out vec2 texture_coordinates;

void main() {
    gl_Position = vec4(v_position, 1.0);
    texture_coordinates.x = v_texture.x;
    texture_coordinates.y = 1.0 - v_texture.y;
}