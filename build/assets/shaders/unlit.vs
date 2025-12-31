#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV; // change to location 1

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoord = aUV;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
