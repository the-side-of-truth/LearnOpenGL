#version 330
in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_wall;
uniform sampler2D texture_face;

void main() {
    // FragColor = texture(texture_wall, TexCoord);
    FragColor = mix(texture(texture_wall, TexCoord), texture(texture_face, TexCoord), 0.2f);
}