#version 330 core

in vec4 Color;

out vec4 FragColor;

flat in uint TextureIndex;

in vec2 TexCoords;

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

void main()
{
    if(int(TextureIndex) == 0)
    {
        FragColor = Color;
    }
    else
    {
        FragColor = SampleTexture(int(TextureIndex), TexCoords);
    }
}
