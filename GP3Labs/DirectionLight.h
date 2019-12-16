#pragma once
#include "pch.h"
class DirectionLight
{

public:
	glm::vec3 lightDir;
	

	void SetLightDirection(glm::vec3 lightDirection);
	glm::vec3 GetLightDir();
};

