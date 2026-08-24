#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_TextureIndex;

out vec4 Color;

void main()
{
    Color = a_Color;

    gl_Position = vec4(a_Pos, 0.f, 1.f);
}