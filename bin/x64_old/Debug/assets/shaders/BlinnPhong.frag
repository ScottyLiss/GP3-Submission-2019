#version 420

in vec3 normal;
in vec2 TexCoord;
in vec3 viewPos;
in vec3 fragPos;

out vec4 fragColour;

uniform sampler2D ourTexture;
uniform vec3 lightPos;

float ambientStrength = 0.1;
vec3 lightColor = vec3(1.f);

void main()
{
	vec3 ambient = ambientStrenght * lightColor;

	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;

	float spec = pow(max(dot(normal, halfwayDir), 0.0),16.0);
	vec3 specular = lightColor * spec;

	vec3 colorResult = (ambient + diffuse + specular) * objectColor;
	FragColor = texture(thisTexture, texCoord) * vec4(colorResult, 1.0);
}