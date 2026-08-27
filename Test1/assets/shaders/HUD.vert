#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in uint a_TextureIndex; // 0 = none
layout(location = 3) in vec2 a_TexCoords;
layout(location = 4) in uint a_FillTextureIndex; // 0 = none

out vec4 Color;
flat out uint TextureIndex;
out vec2 TexCoords;
flat out uint FillTextureIndex;

void main()
{
    Color = a_Color;
    TexCoords = a_TexCoords;
    TextureIndex = a_TextureIndex;
    FillTextureIndex = a_FillTextureIndex;

    gl_Position = vec4(a_Pos, 0.f, 1.f);
}
