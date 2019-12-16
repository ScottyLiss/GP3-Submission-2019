#include "DirectionLight.h"

void DirectionLight::SetLightDirection(glm::vec3 lightDirection)
{
	lightDir = lightDirection;
}

glm::vec3 DirectionLight::GetLightDir()
{
	return lightDir;
}