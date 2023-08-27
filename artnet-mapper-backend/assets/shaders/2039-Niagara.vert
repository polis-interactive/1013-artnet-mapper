
#version 300 es
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texture;

out vec2 texture_coordinates;

void main() {
    gl_Position = vec4(v_position, 1.0);
    texture_coordinates.x = v_texture.x;
    texture_coordinates.y = v_texture.y;
}