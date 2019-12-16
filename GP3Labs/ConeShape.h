#pragma once
#include "pch.h"
#include "CollisionShape.h"
class ConeShape : public CollisionShape
{
private:
	float height;
	float radius;
public:
	ConeShape(float radius, float height);
};
