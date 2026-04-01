#version 330

in vec2 TexCoords;
flat in int Index;
in vec3 Color;

in vec2 FragPos;

out vec4 FragColor;

uniform sampler2D u_Textures[32]; // no tex, tex1, tex...

struct PointLight
{
	vec2 u_LightPos;
	vec3 u_LightColor;
	vec2 u_ViewPos;
	float u_Attenuation;
};

struct SpotLight
{
	vec2 u_LightPos;
	vec3 u_LightColor;
	vec2 u_ViewPos;
	float u_Attenuation;
	vec2 u_LightDirection;
	float u_InnerCutOff;
	float u_OuterCutOff;
};

uniform PointLight u_PointLight;

vec3 CalculatePointLight(vec2 lightPos, float attenuation, vec3 lightColor,
					float diffuseStrength, float ambientStrength, vec2 viewPos, vec3 color);

uniform float u_AmbientStrength;

void main()
{
	float specularStrength = 0.5f;
	float diffuseStrength = 10.f;


	if(Index != 0)
	{
		vec4 color = texture(u_Textures[Index], vec2(TexCoords.x, 1 - TexCoords.y));

		if(color.a != 1.f)
		{
			discard;
		}

		FragColor = vec4(CalculatePointLight(u_PointLight.u_LightPos, u_PointLight.u_Attenuation,
										u_PointLight.u_LightColor,
										diffuseStrength, u_AmbientStrength, u_PointLight.u_ViewPos,
										color.rgb * Color), 1.f);
	}
	else
	{
		FragColor = vec4(CalculatePointLight(u_PointLight.u_LightPos, u_PointLight.u_Attenuation,
											u_PointLight.u_LightColor,
											diffuseStrength, u_AmbientStrength, u_PointLight.u_ViewPos,
											Color), 1.f);
	}

}

vec3 CalculatePointLight(vec2 lightPos, float attenuation, vec3 lightColor,
					float diffuseStrength, float specularStrength, vec2 viewPos, vec3 color)
{
	vec3 lightDir = normalize(vec3(lightPos, 0.f) - vec3(FragPos, 0.f));

	float dist = length(lightPos - FragPos);
	float diff = 1.0 / (1.0 + attenuation * dist * dist);
	vec3 diffuse = diff * lightColor * diffuseStrength;

	vec3 viewDir = normalize(vec3(viewPos, 0.f) - vec3(FragPos, 0.f));
	vec3 reflectDir = reflect(-lightDir, vec3(0.f, 0.f, 1.f));

	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(vec3(0.f, 0.f, 1.f), halfDir), 0.f), 32.0);
	vec3 specular = spec * specularStrength * lightColor;

	return min(color * (u_AmbientStrength + diffuse + specular), vec3(1.f));
}