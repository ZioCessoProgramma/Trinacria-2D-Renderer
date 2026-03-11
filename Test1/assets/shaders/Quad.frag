#version 330

in vec2 TexCoords;
flat in int Index;
in vec3 Color;

out vec4 FragColor;

uniform sampler2D u_Textures[32]; // no tex, tex1, tex...

void main()
{
	if(Index != 0)
	{
		vec4 color = texture(u_Textures[Index], vec2(TexCoords.x, 1 - TexCoords.y));

		if(color.a != 1.f)
		{
			discard;
		}
	
		//FragColor = vec4(Index / 3, 0.f, 0.f, 1.f);
		FragColor = vec4(color.rgb * Color, 1.f);
	}
	else
	{
		FragColor = vec4(Color, 1.f);
	}

}