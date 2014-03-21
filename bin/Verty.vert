#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 texCoords;

uniform vec2 MousePos;
uniform vec2 ScreenSize;

uniform mat4 TransformMat;

out vec4 vertColor;
out vec2 fragTexCoord;

void main()
{
	gl_Position = TransformMat * position;
	vertColor = color;
	fragTexCoord = vec2(texCoords.x, texCoords.y);
}