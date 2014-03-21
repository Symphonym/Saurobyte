#version 330 core

out vec4 outputColor;

uniform vec2 MousePos;
uniform vec2 ScreenSize;

uniform sampler2D tex;
in vec2 fragTexCoord;

in vec4 vertColor;

void main()
{
	outputColor = texture(tex, fragTexCoord) * vertColor;
}