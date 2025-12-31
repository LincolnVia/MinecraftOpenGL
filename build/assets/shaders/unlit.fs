
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D atlas;

void main() {
    vec4 tex = texture(atlas, TexCoord);

    if (tex.a < 0.1)
        discard;

    FragColor = tex;
}
