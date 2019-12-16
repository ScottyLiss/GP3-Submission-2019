#version 420

in vec3 normal;
in vec2 TexCoord;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightDir;



void main()
{

	// Ambient Lighting
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * lightColor;


	vec3 lightDirection = normalize(lightDir - fragPos);

	// Diffuse
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse =  diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDirection + viewDir);

	//Specular
	float spec = pow(max(dot(normal, halfwayDir), 0.0),1.0);
	vec3 specular = (lightColor * spec) * 0.1;
	

	vec3 result = (ambient + (diffuse + specular));
	fragColor = texture(ourTexture, TexCoord) * vec4(result, 1);

}