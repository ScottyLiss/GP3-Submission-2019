#version 420

out vec4 fragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
	float Radius;
	float Constant;
};

const int NR_LIGHTS = 10;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;



void main()
{

	//Retrieve data from the Gbuffers
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
	float Specular = texture(gAlbedoSpec, TexCoords).a;

	// Calculation on lighting 

	vec3 lighting = Diffuse * 0.1f; 
	vec3 viewDir = normalize(viewPos - FragPos);
	for(int i = 0; i < NR_LIGHTS; i++)
	{
		//Calculation on distance between light source and current fragment
		float distance = length(lights[i].Position - FragPos);
		if(true) //distance < lights[i].Radius <-- Disabled due to not working at some stage
		{
			//Diffuse
			vec3 lightDir = normalize(vec3(lights[i].Position) - FragPos);
			vec3 diffuse = max(dot(Normal, lightDir),0.0) * Diffuse * lights[i].Color;

			//Specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(Normal, halfwayDir),0.0),16.0);
			vec3 specular = lights[i].Color * spec * Specular;

			//Attenuation
			float attenuation = 1.0/(1 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
			diffuse *= attenuation;
			specular *= attenuation;
			lighting += diffuse + specular;
		}
	
	}

	fragColor = vec4(lighting, 1.0);
}