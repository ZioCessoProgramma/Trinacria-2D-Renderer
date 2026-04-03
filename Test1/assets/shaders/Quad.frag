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


struct DirectionalLight
{
	vec2 u_LightDirection;
	vec2 u_ViewPos;
	vec3 u_LightColor;
};


uniform PointLight u_PointLight;
uniform SpotLight u_SpotLight;
uniform DirectionalLight u_DirectionalLight;

vec3 CalculateLight(vec2 lightPos, float attenuation, vec3 lightColor,
					float diffuseStrength, float specularStrength, vec2 viewPos, vec3 color);

vec3 CalculateDirectionalLight(vec2 lightDir, vec2 viewPos, vec3 lightColor, vec3 objColor,
							   float specStrength, float diffuseStrength);

vec3 SumAllLights(vec3 baseColor);

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

		if(u_AmbientStrength != 1.f)
		{
			FragColor = vec4(SumAllLights(color.rgb * Color), 1.f);
		}
		else
		{
			FragColor = vec4(color.rgb * Color, 1.f);
		}
	}
	else
	{
		if(u_AmbientStrength != 1.f)
		{
			FragColor = vec4(SumAllLights(Color), 1.f);
		}
		else
		{
			FragColor = vec4(Color, 1.f);
		}
	}

}

vec3 CalculateLight(vec2 lightPos, float attenuation, vec3 lightColor,
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

vec3 SumAllLights(vec3 baseColor)
{

	float specularStrength = 0.5f;
	float diffuseStrength = 10.f;

	vec3 pointLight = CalculateLight(u_PointLight.u_LightPos, u_PointLight.u_Attenuation,
									 u_PointLight.u_LightColor,
									 diffuseStrength, specularStrength, u_PointLight.u_ViewPos,
									 baseColor);

	vec3 spotLight = vec3(0.f);

	vec2 spotLightDirection = normalize(u_SpotLight.u_LightDirection);

	vec2 lightDir = vec2(normalize(vec3(u_SpotLight.u_LightPos, 0.f) - vec3(FragPos, 0.f)));

	float theta = dot(lightDir, normalize(-spotLightDirection));
	float epsilon = u_SpotLight.u_InnerCutOff - u_SpotLight.u_OuterCutOff;
	float intesity = clamp((theta - u_SpotLight.u_OuterCutOff) / epsilon, 0.f, 1.f);

	spotLight = CalculateLight(u_SpotLight.u_LightPos, u_SpotLight.u_Attenuation,
							   u_SpotLight.u_LightColor, 2.f, specularStrength,
							   u_SpotLight.u_ViewPos, baseColor);

	spotLight = spotLight * intesity;

	vec3 directionalLight = CalculateDirectionalLight(u_DirectionalLight.u_LightDirection,
													  u_DirectionalLight.u_ViewPos,
													  u_DirectionalLight.u_LightColor, baseColor,
													  specularStrength, diffuseStrength);

	return pointLight + spotLight + directionalLight;
}

vec3 CalculateDirectionalLight(vec2 lightDir, vec2 viewPos, vec3 lightColor,
							   vec3 objColor, float specStrength, float diffuseStrength)
{

	vec3 N = vec3(0.f, 0.f, 1.f);
	vec3 L = normalize(vec3(lightDir, 0.4f));
	vec3 V = vec3(0.f, 0.f, 1.f);

	float diff = max(dot(N, L), 0.0);

	vec3 H = normalize(L + V);
	float spec = 0.0;
	if (diff > 0.0) {
		spec = pow(max(dot(N, H), 0.0), 32);
	}

	vec3 ambient = 0.2 * objColor;
	vec3 diffuse = diff * objColor * lightColor;
	vec3 specular = specStrength * spec * lightColor;

	return ambient + diffuse + specular;
}