#version 310 es

const vec2 pos0 = vec2(-1.0,  1.0);
const vec2 pos1 = vec2(-1.0, -1.0);
const vec2 pos2 = vec2( 1.0,  1.0);
const vec2 pos3 = vec2( 1.0, -1.0);

out vec2 texture_coordinates;

void main() {
    vec2 position;
    switch(gl_VertexID) {
        case 0: position = pos0; break;
        case 1: position = pos1; break;
        case 2: position = pos2; break;
        case 3: position = pos3; break;
    }

    texture_coordinates = 0.5 * position + vec2(0.5);
    texture_coordinates.y = 1.0 - texture_coordinates.y;
    gl_Position = vec4(position, 0.0, 1.0);
}
