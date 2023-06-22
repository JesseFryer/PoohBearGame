#version 330 core
out vec4 FragColour;

in vec2 TexCoord;
in float TexIndex;
in vec4 Colour;

uniform sampler2D u_Textures[32];

void main()
{
    int index = int(TexIndex);
    if (index == -1)
    {
        FragColour = Colour;
    }
    else 
    {
        FragColour = texture(u_Textures[index], TexCoord);
    }
    if (FragColour == 0) discard;
}