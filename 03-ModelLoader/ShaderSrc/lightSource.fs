#version 330
out vec4 FragColor;
uniform vec3 lightSrcColor;

void main()
{
    FragColor = vec4(lightSrcColor, 1.0);
}