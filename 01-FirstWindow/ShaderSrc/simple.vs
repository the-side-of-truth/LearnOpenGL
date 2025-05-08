#version 330
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;
 
void main(){ 
	vec4 a = vec4(1, 2, 3, 4);
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	ourColor = aColor;
	TexCoord = aTexCoord;     
}