#version 330 core

in vec4 Color;
out vec4 FragColor;
flat in uint TextureIndex;
in vec2 TexCoords;
flat in uint FillTextureIndex;

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

// TODO: in vertex
uniform float progress;

void main()
{
    if(int(TextureIndex) == 0)
    {
        FragColor = Color;
    }
    else
    {
        vec4 fillColor = SampleTexture(int(FillTextureIndex), TexCoords);

        if(fillColor.rgb == vec3(1.f, 0.f, 1.f))
        {
            if (TexCoords.x <= progress)
            {
                FragColor = vec4(0.3f, 0.7f, 0.3f, 0.9f);
            }
            else
            {
                FragColor = SampleTexture(int(TextureIndex), TexCoords);
            }
        }
        else
        {
            FragColor = SampleTexture(int(TextureIndex), TexCoords);
        }

    }
}
