#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTexIndex;
layout(location = 3) in vec4 aColour;

out vec2 TexCoord;
out float TexIndex;
out vec4 Colour;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TexCoord = aTexCoord;
	TexIndex = aTexIndex;
	Colour = aColour;
}