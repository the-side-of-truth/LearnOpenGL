#version 330
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_wall;
uniform sampler2D texture_face;
uniform float alpha;

void main() {
    // FragColor = texture(texture_wall, TexCoord);
    FragColor = mix(texture(texture_wall, TexCoord), texture(texture_face, vec2(1.0 - TexCoord.x, TexCoord.y)), alpha); 
}