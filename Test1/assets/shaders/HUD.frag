#version 330 core

in vec4 Color;

out vec4 FragColor;

flat out int TextureIndex;

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

void main()
{
    if(TextureIndex == 0)
    {
        FragColor = Color;
    }
    else
    {
        FragColor = SampleTexture(TextureIndex, )
    }
}