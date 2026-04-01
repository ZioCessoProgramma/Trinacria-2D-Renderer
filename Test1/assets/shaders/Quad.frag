#version 330

in vec2 TexCoords;
flat in int Index;
in vec3 Color;

in vec2 FragPos;

out vec4 FragColor;

uniform sampler2D u_Textures[32]; // no tex, tex1, tex...

uniform vec2 u_LightPos;
uniform vec3 u_LightColor;
uniform vec2 u_ViewPos;

uniform float u_Attenuation;

uniform float u_AmbientStrength;

void main()
{
	float specularStrength = 0.5f;
	float diffuseStrength = 10.f;

	vec3 lightDir = normalize(vec3(u_LightPos, 0.f) - vec3(FragPos, 0.f));

	float dist = length(u_LightPos - FragPos);
	float diff = 1.0 / (1.0 + u_Attenuation * dist * dist);
	vec3 diffuse = diff * u_LightColor * diffuseStrength;

	vec3 viewDir = normalize(vec3(u_ViewPos, 0.f) - vec3(FragPos, 0.f));
	vec3 reflectDir = reflect(-lightDir, vec3(0.f, 0.f, 1.f));

	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(vec3(0.f, 0.f, 1.f), halfDir), 0.f), 32.0);
	vec3 specular = spec * specularStrength * u_LightColor;

	if(Index != 0)
	{
		vec4 color = texture(u_Textures[Index], vec2(TexCoords.x, 1 - TexCoords.y));

		if(color.a != 1.f)
		{
			discard;
		}
	
		FragColor = vec4(min(color.rgb * Color * (u_AmbientStrength + diffuse + specular), vec3(1.f)), 1.f);
	}
	else
	{
		FragColor = vec4(min(Color * (u_AmbientStrength + diffuse + specular), vec3(1.f)), 1.f);
	}

}