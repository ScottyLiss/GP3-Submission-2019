#pragma once
#include "pch.h"
#include "CollisionShape.h"
class CapsuleShape : public CollisionShape
{
private:
	float radius;
	float height;
public:
	CapsuleShape(float radius, float height);
};